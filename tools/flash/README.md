# Windows wireless firmware flashing

`Flash-WioWireless.ps1` validates an AmebaD three-image firmware set and uses the official Seeed Windows tools. Python is not required. The script defaults to validation only: it does not enumerate or open serial ports until `-Execute` is supplied.

## Tools

Use a complete checkout of `https://github.com/Seeed-Studio/ambd_flash_tool` and pass its location with `-ToolDirectory`. The required resources are:

- `tool/windows/amebad_image_tool.exe` and its adjacent `cygwin1.dll`
- `tool/windows/bossac.exe`, or the executable passed with `-BossacPath`
- `firmware/WioTerminal_USB2Serial_Burn8720.ino.bin`
- `imgtool_flashloader_amebad.bin`

The script locks the board by USB serial number and VID `2886`, with application PID `802D` or SAMD bootloader PID `002D`. With `-Execute`, `-DeviceSerialNumber` is required and must identify the target board. It follows the Windows PnP parent chain so composite USB serial ports are matched to the physical USB identity. Windows `PnpDevice` commands must be available for execution.

## Validate, then execute

Close any program using the Wio serial port. Set the firmware directory to the folder containing `km0_boot_all.bin`, `km4_boot_all.bin`, and `km0_km4_image2.bin`.

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\tools\flash\Flash-WioWireless.ps1 -FirmwareDirectory "D:\path\to\firmware" -ToolDirectory "D:\path\to\ambd_flash_tool"

powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\tools\flash\Flash-WioWireless.ps1 -FirmwareDirectory "D:\path\to\firmware" -ToolDirectory "D:\path\to\ambd_flash_tool" -DeviceSerialNumber "YOUR_BOARD_USB_SERIAL" -Execute
```

Both runs verify image headers, sizes, and SHA256 hashes, then verify a temporary snapshot of the input files. KM0 boot is limited to 16 KiB, KM4 boot to 8 KiB, and the combined application to 1 MiB by default, preserving the conventional OTA1 slot boundary. Override `-MaximumApplicationBytes` only after checking the firmware's flash layout. Flash offsets are fixed by the official image tool: `0x0000`, `0x4000`, and `0x6000` respectively.

Optional `-ManifestPath` checks expected lengths and hashes. `manifest.json` or `firmware-manifest.json` in the firmware directory is detected automatically. Every image must have exactly one manifest entry; `size` and `sha256` are optional per entry. Supported forms include:

```json
{
  "files": [
    { "name": "km0_boot_all.bin", "size": 4412, "sha256": "64 hex characters" },
    { "name": "km4_boot_all.bin", "size": 4068, "sha256": "64 hex characters" },
    { "name": "km0_km4_image2.bin", "size": 876544, "sha256": "64 hex characters" }
  ]
}
```

`files` may also be an object keyed by the image filename; each value can be an object with `size`/`sha256` or a SHA256 string. `images` and `artifacts` are accepted aliases for `files`.

With `-Execute`, the matched application port receives a 1200-baud touch, the same USB serial must reappear with bootloader PID `002D`, and bossac writes and verifies the official SAMD USB-to-serial bridge. The script waits for that same device to return with PID `802D`, then launches the RTL tool in a unique temporary directory containing only the staged images, bridge, and RAM flashloader. It never invokes an erase command. The official binary may erase sectors internally while programming.

The script requires a zero native exit code, the expected success marker, and no error/failure text. Native stdout and stderr are saved without truncation. `-LogDirectory` changes the persistent log parent; the default is `%TEMP%\wio-wireless-flash-logs`. Each run records hashes and an `inputs.json` receipt. `-KeepStaging` retains the temporary image snapshot. A timeout stops the native process and reports failure; an interrupted RTL transfer requires another flash attempt.

After RTL flashing, re-upload the SAMD51 application and check the reported wireless firmware version and a real TLS connection. A successful transfer alone does not prove that the new firmware boots or that TLS works. The tools have no current-firmware backup command. Keep the known official v2.1.3 three-image set for rollback, using this same script and then restoring the SAMD51 application.
