[CmdletBinding()]
param(
    [Parameter(Mandatory=$true)][string]$ConfigFile,
    [Parameter(Mandatory=$true)][string]$CoreDirectory,
    [Parameter(Mandatory=$true)][string]$ToolchainDirectory,
    [Parameter(Mandatory=$true)][string]$ToolsDirectory,
    [string]$ArduinoCli = 'arduino-cli',
    [string]$OutputDirectory,
    [string]$TlsDirectory,
    [switch]$SkipTlsBuild
)
$ErrorActionPreference = 'Stop'
$repository = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
if (!$OutputDirectory) { $OutputDirectory = Join-Path $repository '.build/firmware' }
if (!$TlsDirectory) { $TlsDirectory = Join-Path $repository '.build/tls' }
$OutputDirectory = [IO.Path]::GetFullPath($OutputDirectory)
$TlsDirectory = [IO.Path]::GetFullPath($TlsDirectory)
$CoreDirectory = (Resolve-Path -LiteralPath $CoreDirectory).Path
$ToolchainDirectory = (Resolve-Path -LiteralPath $ToolchainDirectory).Path
$ToolsDirectory = (Resolve-Path -LiteralPath $ToolsDirectory).Path
$ConfigFile = (Resolve-Path -LiteralPath $ConfigFile).Path
$pinnedCoreMarker = Join-Path $CoreDirectory '.wio-pinned-core.json'
if (!(Test-Path -LiteralPath $pinnedCoreMarker)) {
    throw 'Prepare the pinned SDK with tools/build/Prepare-PinnedCore.mjs --apply before building.'
}
$coreIdentity = Get-Content -Raw -LiteralPath $pinnedCoreMarker | ConvertFrom-Json
if ($coreIdentity.status -ne 'verified' -or $coreIdentity.commit -ne 'f81bca75e433e35b2d27cadd178596888940fb5a') {
    throw 'The SDK marker does not identify the required verified Core commit.'
}
$gcc = Join-Path $ToolchainDirectory 'bin/arm-none-eabi-gcc.exe'
$nm = Join-Path $ToolchainDirectory 'bin/arm-none-eabi-nm.exe'
foreach ($required in @($gcc, $nm, (Join-Path $CoreDirectory 'platform.txt'))) {
    if (!(Test-Path -LiteralPath $required -PathType Leaf)) { throw "Missing build dependency: $required" }
}
New-Item -ItemType Directory -Path $OutputDirectory,$TlsDirectory -Force | Out-Null
if (!$SkipTlsBuild) {
    & node (Join-Path $repository 'tools/tls/build.mjs') --toolchain (Join-Path $ToolchainDirectory 'bin') --core $CoreDirectory --out $TlsDirectory
    if ($LASTEXITCODE -ne 0) { throw 'Private Mbed TLS build failed.' }
}
$modernArchive = Join-Path $TlsDirectory 'lib_wio_mbedtls367.a'
$modernInclude = Join-Path $TlsDirectory 'include'
if (!(Test-Path -LiteralPath $modernArchive)) { throw 'Private TLS archive is missing.' }

# Keep vendor 2.4 for SDK-internal Wi-Fi/802.1X. The RPC TLS archive uses a
# separate symbol and header namespace; replacing the vendor archive breaks ABI.
$platformLines = Get-Content -LiteralPath (Join-Path $CoreDirectory 'platform.txt')
$archiveLine = $platformLines | Where-Object { $_.StartsWith('compiler.ameba.ar.list=') } | Select-Object -First 1
if (!$archiveLine) { throw 'The SDK archive list was not found.' }
$archives = $archiveLine.Substring('compiler.ameba.ar.list='.Length)
$archives += ' "' + ($modernArchive.Replace('\','/')) + '"'
$includeDirectories = @('src/easylogger','src/easylogger/inc','src/ble','src/wifi','src/esp_lib','src/erpc','src/erpc_shim','src/mDNS')
$extraFlags = ($includeDirectories | ForEach-Object { '-I"' + ((Join-Path $repository $_).Replace('\','/')) + '"' }) -join ' '
$extraFlags += ' -I"' + $modernInclude.Replace('\','/') + '"'

# The old vendor postbuild program writes intermediate images below its own
# directory. Isolate it so two builds cannot overwrite the installed SDK tools.
$localTools = Join-Path $OutputDirectory 'image-tools'
New-Item -ItemType Directory -Path $localTools -Force | Out-Null
Copy-Item -Path (Join-Path $ToolsDirectory '*') -Destination $localTools -Recurse -Force
$buildDirectory = Join-Path $OutputDirectory 'objects'
New-Item -ItemType Directory -Path $buildDirectory -Force | Out-Null
$compileArguments = @(
    'compile','--config-file',$ConfigFile,'--fqbn','realtek:AmebaD:ameba_rtl8721d',
    '--build-path',$buildDirectory,'--verbose','--clean',
    '--build-property',('build.extra_flags=' + $extraFlags),
    '--build-property',('compiler.ameba.ar.list=' + $archives),
    '--build-property',('runtime.tools.ameba_d_tools.path=' + $localTools.Replace('\','/')),
    $repository
)
$compileLog = Join-Path $OutputDirectory 'compile.log'
$previousErrorAction = $ErrorActionPreference
try {
    $ErrorActionPreference = 'Continue'
    & $ArduinoCli @compileArguments *> $compileLog
    $compileExitCode = $LASTEXITCODE
} finally { $ErrorActionPreference = $previousErrorAction }
if ($compileExitCode -ne 0) {
    Get-Content -LiteralPath $compileLog -Tail 70 | Write-Output
    throw "Firmware compilation failed. Full log: $compileLog"
}

$application = Join-Path $buildDirectory 'application.axf'
$symbols = & $nm -g $application
if ($LASTEXITCODE -ne 0) { throw 'Cannot inspect the linked firmware.' }
foreach ($symbol in @('wio_mbedtls_ssl_handshake','wio_mbedtls_gcm_crypt_and_tag','wio_tls_provision')) {
    if (!($symbols | Where-Object { $_ -match ('\b[Tt] ' + [regex]::Escape($symbol) + '$') })) {
        throw "Required linked TLS symbol missing: $symbol"
    }
}
$symbols | Set-Content -LiteralPath (Join-Path $OutputDirectory 'symbols.txt') -Encoding ASCII
$imageDirectory = Join-Path $OutputDirectory 'images'
New-Item -ItemType Directory -Path $imageDirectory -Force | Out-Null
$imageNames = @('km0_boot_all.bin','km4_boot_all.bin','km0_km4_image2.bin')
$imageEntries = @()
foreach ($name in $imageNames) {
    $sourceImage = Join-Path $localTools $name
    if (!(Test-Path -LiteralPath $sourceImage -PathType Leaf)) { throw "Postbuild image missing: $name" }
    $destination = Join-Path $imageDirectory $name
    Copy-Item -LiteralPath $sourceImage -Destination $destination -Force
    $size = (Get-Item -LiteralPath $destination).Length
    if ($size -lt 1024) { throw "Image is unexpectedly small: $name ($size bytes)." }
    if ($name -eq 'km0_km4_image2.bin' -and $size -gt (2MB-0x6000)) { throw 'Application image exceeds the 2 MiB flash layout.' }
    if ($name -eq 'km0_km4_image2.bin') {
        $imageText = [Text.Encoding]::ASCII.GetString([IO.File]::ReadAllBytes($destination))
        foreach ($requiredText in @('2.1.3-coloz-tls12.1','TLS-ECDHE-RSA-WITH-AES-128-GCM-SHA256')) {
            if (!$imageText.Contains($requiredText)) { throw "Generated image does not contain $requiredText; refusing a stale postbuild artifact." }
        }
    }
    $imageEntries += [ordered]@{ name=$name; size=$size; sha256=(Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash.ToLowerInvariant() }
}
$commit = (& git -c "safe.directory=$($repository.Replace('\','/'))" -C $repository rev-parse HEAD).Trim()
$dirty = @(& git -c "safe.directory=$($repository.Replace('\','/'))" -C $repository status --porcelain).Count -gt 0
$tlsManifest = Get-Content -Raw -LiteralPath (Join-Path $TlsDirectory 'manifest.json') | ConvertFrom-Json
$manifest = [ordered]@{
    hardware='Wio Terminal / RTL8720DN'; firmwareVersion='2.1.3-coloz-tls12.1';
    sourceCommit=$commit; sourceDirty=$dirty; builtAtUtc=[DateTime]::UtcNow.ToString('o');
    sdk='Realtek AmebaD 3.0.5 with pinned Seeed Core'; sdkCommit=$coreIdentity.commit;
    compiler=((& $gcc --version | Select-Object -First 1).Trim());
    tls=$tlsManifest; files=$imageEntries
}
$manifest | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath (Join-Path $imageDirectory 'manifest.json') -Encoding UTF8
$imageEntries | ForEach-Object { [pscustomobject]$_ } | Format-Table name,size,sha256 -AutoSize
Write-Output "Firmware images: $imageDirectory"
