#requires -Version 5.1
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string] $FirmwareDirectory,
    [string] $ToolDirectory = '',
    [string] $BossacPath = '',
    [string] $DeviceSerialNumber = '',
    [string] $ManifestPath = '',
    [string] $LogDirectory = '',
    [ValidateRange(1, 2147483647)]
    [long] $MaximumApplicationBytes = 1048576,
    [ValidateRange(5, 300)]
    [int] $EnumerationTimeoutSeconds = 40,
    [ValidateRange(30, 3600)]
    [int] $FlashTimeoutSeconds = 900,
    [switch] $KeepStaging,
    [switch] $Execute
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
$script:LogFile = $null
$script:Utf8 = New-Object System.Text.UTF8Encoding($false)
$stageDirectory = $null
$stageParent = [IO.Path]::GetFullPath([IO.Path]::GetTempPath())
$runId = (Get-Date -Format 'yyyyMMdd-HHmmss') + '-' + [Guid]::NewGuid().ToString('N').Substring(0, 8)

function Write-FlashLog([string] $Message) {
    $line = '[' + (Get-Date -Format 'yyyy-MM-dd HH:mm:ss.fff') + '] ' + $Message
    Write-Host $line
    if ($script:LogFile) {
        [IO.File]::AppendAllText($script:LogFile, $line + [Environment]::NewLine, $script:Utf8)
    }
}

function Get-ObjectValue($Object, [string[]] $Names) {
    if ($null -eq $Object) { return $null }
    foreach ($name in $Names) {
        $property = $Object.PSObject.Properties[$name]
        if ($null -ne $property) { return $property.Value }
    }
    return $null
}

function Resolve-ExistingFile([string] $Path) {
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) { throw "File not found: $Path" }
    return (Get-Item -LiteralPath $Path).FullName
}

function Get-ManifestEntry($Manifest, [string] $Name) {
    $files = Get-ObjectValue $Manifest @('files', 'images', 'artifacts')
    if ($null -eq $files) { throw 'The manifest must contain files, images, or artifacts.' }
    if ($files -is [System.Array]) {
        $entries = @($files | Where-Object {
            (Get-ObjectValue $_ @('name', 'file', 'filename', 'path')) -ceq $Name
        })
        if ($entries.Count -ne 1) { throw "The manifest must contain exactly one entry for $Name." }
        return $entries[0]
    }
    $property = $files.PSObject.Properties[$Name]
    if ($null -eq $property) { throw "The manifest has no entry for $Name." }
    if ($property.Value -is [string]) {
        return [PSCustomObject]@{ sha256 = $property.Value }
    }
    return $property.Value
}

function Assert-Image([string] $Path, [string] $Name, [long] $MaximumBytes, [string] $HeaderHex, $Manifest) {
    $file = Get-Item -LiteralPath (Resolve-ExistingFile $Path)
    if ($file.Length -lt 32 -or $file.Length -gt $MaximumBytes) {
        throw "$Name size $($file.Length) is outside the allowed range 32..$MaximumBytes bytes."
    }
    $stream = [IO.File]::OpenRead($file.FullName)
    try {
        $header = New-Object byte[] 8
        if ($stream.Read($header, 0, 8) -ne 8) { throw "Cannot read $Name header." }
    } finally { $stream.Dispose() }
    $actualHeader = [BitConverter]::ToString($header).Replace('-', '')
    if ($actualHeader -cne $HeaderHex) { throw "$Name has an unexpected AmebaD image header: $actualHeader." }
    $hash = (Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash
    if ($null -ne $Manifest) {
        $entry = Get-ManifestEntry $Manifest $Name
        $expectedHash = Get-ObjectValue $entry @('sha256', 'hash')
        $expectedSize = Get-ObjectValue $entry @('size', 'bytes', 'length')
        if ($null -ne $expectedHash) {
            if ([string]$expectedHash -notmatch '^[0-9a-fA-F]{64}$') { throw "Invalid SHA256 in manifest for $Name." }
            if ($hash -ine [string]$expectedHash) { throw "SHA256 mismatch for $Name." }
        }
        if ($null -ne $expectedSize -and $file.Length -ne [long]$expectedSize) {
            throw "Size mismatch for $Name (expected $expectedSize, found $($file.Length))."
        }
    }
    Write-FlashLog "$Name bytes=$($file.Length) sha256=$hash"
    return [PSCustomObject]@{ name = $Name; size = $file.Length; sha256 = $hash; source = $file.FullName }
}

function ConvertTo-NativeArgument([string] $Value) {
    # Windows CommandLineToArgvW quoting; no shell is involved.
    if ($Value.Length -gt 0 -and $Value -notmatch '[\s"]') { return $Value }
    $quoted = '"'
    $slashes = 0
    foreach ($character in $Value.ToCharArray()) {
        if ($character -eq '\') { $slashes++; continue }
        if ($character -eq '"') {
            $quoted += ('\' * (2 * $slashes + 1)) + '"'
        } else {
            $quoted += ('\' * $slashes) + $character
        }
        $slashes = 0
    }
    return $quoted + ('\' * (2 * $slashes)) + '"'
}

function Invoke-LoggedNative([string] $Executable, [string[]] $Arguments, [string] $WorkingDirectory,
    [string] $Name, [int] $TimeoutSeconds, [string] $RequiredSuccessPattern) {
    $stdoutPath = Join-Path $script:RunLogDirectory ($Name + '.stdout.log')
    $stderrPath = Join-Path $script:RunLogDirectory ($Name + '.stderr.log')
    $argumentText = ($Arguments | ForEach-Object { ConvertTo-NativeArgument $_ }) -join ' '
    Write-FlashLog "Starting $Name : $Executable $argumentText"
    $process = Start-Process -FilePath $Executable -ArgumentList $argumentText -WorkingDirectory $WorkingDirectory `
        -WindowStyle Hidden -RedirectStandardOutput $stdoutPath -RedirectStandardError $stderrPath -PassThru
    $null = $process.Handle
    $timer = [Diagnostics.Stopwatch]::StartNew()
    $nextProgress = 15
    while (-not $process.WaitForExit(250)) {
        if ($timer.Elapsed.TotalSeconds -ge $TimeoutSeconds) {
            try { $process.Kill(); $process.WaitForExit() } catch { }
            throw "$Name timed out after $TimeoutSeconds seconds; see $stdoutPath and $stderrPath."
        }
        if ($timer.Elapsed.TotalSeconds -ge $nextProgress) {
            Write-FlashLog "$Name is running ($([int]$timer.Elapsed.TotalSeconds) s); native output: $stdoutPath"
            $nextProgress += 15
        }
    }
    $process.WaitForExit()
    $exitCode = $process.ExitCode
    $output = [IO.File]::ReadAllText($stdoutPath)
    $errors = [IO.File]::ReadAllText($stderrPath)
    foreach ($line in (($output + [Environment]::NewLine + $errors) -split '\r?\n')) {
        if ($line.Length -gt 0) { Write-FlashLog "${Name}: $line" }
    }
    Write-FlashLog "$Name exit_code=$exitCode"
    $combined = $output + [Environment]::NewLine + $errors
    if ($null -eq $exitCode -or $exitCode -ne 0) { throw "$Name failed with exit code $exitCode." }
    if ($combined -match '(?im)\b(error|failed|failure|fail)\b|checksum\s+error|could\s+not|\bcannot\b') {
        throw "$Name reported an error, despite its exit code; inspect the native logs."
    }
    if ($combined -notmatch $RequiredSuccessPattern) { throw "$Name did not report its required success marker." }
}

function Get-WioPorts {
    try {
        $ports = @(Get-PnpDevice -Class Ports -PresentOnly -ErrorAction Stop)
    } catch {
        # A USB mode transition can temporarily leave no present Ports-class devices.
        if ($_.CategoryInfo.Category -eq [Management.Automation.ErrorCategory]::ObjectNotFound) { return }
        throw
    }
    foreach ($port in $ports) {
        $instance = [string]$port.InstanceId
        $current = $instance
        $matchedPid = $null
        for ($level = 0; $level -lt 8 -and $current.Length -gt 0; $level++) {
            if ($current -match '^USB\\VID_2886&PID_(802D|002D)(?:&[^\\]+)?\\([^\\]+)$') {
                if ($Matches[2] -ieq $DeviceSerialNumber) { $matchedPid = $Matches[1].ToUpperInvariant(); break }
            }
            try {
                $parentProperty = Get-PnpDeviceProperty -InstanceId $current -KeyName 'DEVPKEY_Device_Parent' -ErrorAction Stop
                $parent = [string]$parentProperty.Data
            } catch { break }
            if ($parent -eq $current) { break }
            $current = $parent
        }
        if ($null -eq $matchedPid) { continue }
        $portName = $null
        try {
            $portName = [string](Get-ItemPropertyValue -LiteralPath ("Registry::HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\$instance\Device Parameters") -Name PortName -ErrorAction Stop)
        } catch {
            if ([string]$port.FriendlyName -match '\((COM[0-9]+)\)\s*$') { $portName = $Matches[1] }
        }
        if ($portName -notmatch '^COM[0-9]+$') { throw "Cannot resolve the COM port for target USB device $instance." }
        [PSCustomObject]@{ Port = $portName; Pid = $matchedPid; Serial = $DeviceSerialNumber; UsbInstance = $current }
    }
}

function Wait-WioPort([string] $ExpectedProductId) {
    $timer = [Diagnostics.Stopwatch]::StartNew()
    do {
        $devices = @(Get-WioPorts)
        if ($devices.Count -eq 1 -and $devices[0].Pid -eq $ExpectedProductId) {
            Write-FlashLog "Target enumerated: port=$($devices[0].Port) vid=2886 pid=$ExpectedProductId serial=$DeviceSerialNumber"
            return $devices[0]
        }
        Start-Sleep -Milliseconds 250
    } while ($timer.Elapsed.TotalSeconds -lt $EnumerationTimeoutSeconds)
    throw "Timed out waiting for exactly one target device with VID=2886 PID=$ExpectedProductId serial=$DeviceSerialNumber."
}

try {
    if ($env:OS -ne 'Windows_NT') { throw 'This script requires Windows.' }
    if ($Execute -and $DeviceSerialNumber -notmatch '^[A-Za-z0-9_-]+$') { throw 'Specify -DeviceSerialNumber with the target USB serial number, without path separators, when using -Execute.' }
    $firmwareRoot = (Get-Item -LiteralPath $FirmwareDirectory -ErrorAction Stop).FullName
    if (-not (Test-Path -LiteralPath $firmwareRoot -PathType Container)) { throw 'FirmwareDirectory must be a directory.' }
    $workspaceRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../..'))
    if (-not $ToolDirectory) { $ToolDirectory = Join-Path $workspaceRoot '.wio-tools/rtl-modern/flash-tool' }
    $toolRoot = (Get-Item -LiteralPath $ToolDirectory -ErrorAction Stop).FullName
    if (-not $BossacPath) { $BossacPath = Join-Path $toolRoot 'tool/windows/bossac.exe' }
    $BossacPath = Resolve-ExistingFile $BossacPath
    $flasherPath = Resolve-ExistingFile (Join-Path $toolRoot 'tool/windows/amebad_image_tool.exe')
    $null = Resolve-ExistingFile (Join-Path $toolRoot 'tool/windows/cygwin1.dll')
    $bridgePath = Resolve-ExistingFile (Join-Path $toolRoot 'firmware/WioTerminal_USB2Serial_Burn8720.ino.bin')
    $flashloaderPath = Resolve-ExistingFile (Join-Path $toolRoot 'imgtool_flashloader_amebad.bin')
    if (-not $LogDirectory) { $LogDirectory = Join-Path $stageParent 'wio-wireless-flash-logs' }
    $script:RunLogDirectory = [IO.Path]::GetFullPath((Join-Path $LogDirectory $runId))
    $null = New-Item -ItemType Directory -Path $script:RunLogDirectory -Force
    $script:LogFile = Join-Path $script:RunLogDirectory 'flash.log'
    Write-FlashLog "Mode=$(if ($Execute) { 'EXECUTE' } else { 'VALIDATE ONLY' }) target_serial=$DeviceSerialNumber"
    Write-FlashLog "Firmware directory: $firmwareRoot"
    $manifest = $null
    if (-not $ManifestPath) {
        $candidates = @('manifest.json', 'firmware-manifest.json') | ForEach-Object { Join-Path $firmwareRoot $_ } | Where-Object { Test-Path -LiteralPath $_ -PathType Leaf }
        if (@($candidates).Count -gt 1) { throw 'Multiple manifests found; provide -ManifestPath explicitly.' }
        if (@($candidates).Count -eq 1) { $ManifestPath = @($candidates)[0] }
    }
    if ($ManifestPath) {
        $ManifestPath = Resolve-ExistingFile $ManifestPath
        $manifest = Get-Content -LiteralPath $ManifestPath -Raw | ConvertFrom-Json
        Write-FlashLog "Manifest: $ManifestPath sha256=$((Get-FileHash -LiteralPath $ManifestPath -Algorithm SHA256).Hash)"
    } else { Write-FlashLog 'No manifest supplied; recording actual SHA256 values.' }
    $images = @(
        (Assert-Image (Join-Path $firmwareRoot 'km0_boot_all.bin') 'km0_boot_all.bin' 16384 '999996963FCC66FC' $manifest),
        (Assert-Image (Join-Path $firmwareRoot 'km4_boot_all.bin') 'km4_boot_all.bin' 8192 '999996963FCC66FC' $manifest),
        (Assert-Image (Join-Path $firmwareRoot 'km0_km4_image2.bin') 'km0_km4_image2.bin' $MaximumApplicationBytes '3831393538373131' $manifest)
    )
    foreach ($tool in @($BossacPath, $flasherPath, $bridgePath, $flashloaderPath)) {
        Write-FlashLog "Tool: $tool sha256=$((Get-FileHash -LiteralPath $tool -Algorithm SHA256).Hash)"
    }
    $stageDirectory = Join-Path $stageParent ('wio-wireless-flash-' + [Guid]::NewGuid().ToString('N'))
    $null = New-Item -ItemType Directory -Path $stageDirectory
    foreach ($image in $images) {
        $staged = Join-Path $stageDirectory $image.name
        Copy-Item -LiteralPath $image.source -Destination $staged
        if ((Get-FileHash -LiteralPath $staged -Algorithm SHA256).Hash -ine $image.sha256) { throw "Staged image checksum mismatch: $($image.name)" }
    }
    Copy-Item -LiteralPath $flashloaderPath -Destination (Join-Path $stageDirectory 'imgtool_flashloader_amebad.bin')
    $stagedBridge = Join-Path $stageDirectory 'WioTerminal_USB2Serial_Burn8720.ino.bin'
    Copy-Item -LiteralPath $bridgePath -Destination $stagedBridge
    foreach ($resourcePair in @(
        @($flashloaderPath, (Join-Path $stageDirectory 'imgtool_flashloader_amebad.bin')),
        @($bridgePath, $stagedBridge)
    )) {
        if ((Get-FileHash -LiteralPath $resourcePair[0] -Algorithm SHA256).Hash -ine
            (Get-FileHash -LiteralPath $resourcePair[1] -Algorithm SHA256).Hash) {
            throw 'Staged flashloader or bridge checksum mismatch.'
        }
    }
    $receipt = [PSCustomObject]@{ target_serial = $DeviceSerialNumber; firmware_directory = $firmwareRoot; images = $images; staging_directory = $stageDirectory; execute = [bool]$Execute }
    [IO.File]::WriteAllText((Join-Path $script:RunLogDirectory 'inputs.json'), ($receipt | ConvertTo-Json -Depth 6), $script:Utf8)
    Write-FlashLog "Validated and staged input snapshot: $stageDirectory"
    if (-not $Execute) {
        Write-FlashLog 'Validation completed. No serial port was opened and no program was executed. Add -Execute to flash.'
    } else {
        foreach ($command in @('Get-PnpDevice', 'Get-PnpDeviceProperty')) {
            if (-not (Get-Command $command -ErrorAction SilentlyContinue)) { throw "Required Windows command is unavailable: $command" }
        }
        $targets = @(Get-WioPorts)
        if ($targets.Count -ne 1) { throw "Expected exactly one target Wio device; found $($targets.Count)." }
        $target = $targets[0]
        Write-FlashLog "Confirmed device: port=$($target.Port) vid=2886 pid=$($target.Pid) serial=$DeviceSerialNumber"
        if ($target.Pid -eq '802D') {
            Write-FlashLog 'Opening only the matched target at 1200 baud to enter the SAMD bootloader.'
            $serial = [IO.Ports.SerialPort]::new($target.Port, 1200, [IO.Ports.Parity]::None, 8, [IO.Ports.StopBits]::One)
            try { $serial.DtrEnable = $true; $serial.Open(); Start-Sleep -Milliseconds 100; $serial.Close() } finally { $serial.Dispose() }
        }
        $boot = Wait-WioPort '002D'
        Invoke-LoggedNative $BossacPath @('-i', '-d', "--port=$($boot.Port)", '-U', '--offset=0x4000', '-w', '-v', $stagedBridge, '-R') `
            $stageDirectory 'bossac-bridge' 120 '(?im)verify\s+successful'
        $bridge = Wait-WioPort '802D'
        # The official tool locates all three images and the RAM flashloader in its current directory.
        # Never run the official Python erase command or create an all-FF image here.
        Invoke-LoggedNative $flasherPath @($bridge.Port) $stageDirectory 'rtl-flash' $FlashTimeoutSeconds `
            '(?im)All\s+images\s+are\s+sent\s+successfully!'
        Write-FlashLog 'RTL image transfer completed successfully. Re-upload the SAMD51 application, then verify the reported wireless version and TLS connection.'
    }
    Write-FlashLog "Logs: $script:RunLogDirectory"
} catch {
    Write-FlashLog "FAILED: $($_.Exception.Message)"
    if ($script:LogFile) { Write-FlashLog "Logs: $script:RunLogDirectory" }
    Write-Error $_ -ErrorAction Continue
    exit 1
} finally {
    if ($stageDirectory -and (Test-Path -LiteralPath $stageDirectory -PathType Container)) {
        if ($KeepStaging) { Write-FlashLog "Staging retained: $stageDirectory" } else {
            $resolvedStage = [IO.Path]::GetFullPath((Get-Item -LiteralPath $stageDirectory).FullName)
            $parentPrefix = $stageParent.TrimEnd('\', '/') + [IO.Path]::DirectorySeparatorChar
            if (-not $resolvedStage.StartsWith($parentPrefix, [StringComparison]::OrdinalIgnoreCase) -or
                (Split-Path -Leaf $resolvedStage) -notmatch '^wio-wireless-flash-[0-9a-f]{32}$') {
                throw "Refusing to remove unexpected staging path: $resolvedStage"
            }
            Remove-Item -LiteralPath $resolvedStage -Recurse -Force
        }
    }
}
