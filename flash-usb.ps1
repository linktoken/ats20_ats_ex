# Flashes the USB version over the radio's normal USB cable (Arduino bootloader).
#   .\flash-usb.ps1 -Port COM5
#   .\flash-usb.ps1 -Port COM5 -Hex ATS_EX_v1.18m_USB.hex
# Tries the new bootloader speed (115200) first, then the old Nano one (57600).
param(
    [string]$Port,
    [string]$Hex = (Join-Path $PSScriptRoot 'build-usb\ATS_EX.ino.hex')
)
$ErrorActionPreference = 'Stop'
$tools = "$env:LOCALAPPDATA\Arduino15\packages\arduino\tools\avrdude\8.0.0-arduino1"

if (-not $Port) {
    $ports = [System.IO.Ports.SerialPort]::GetPortNames()
    throw "Give the COM port, e.g. .\flash-usb.ps1 -Port COM5   (ports found: $($ports -join ', '))"
}
if (-not (Test-Path $Hex)) { throw "$Hex not found. Run .\build.ps1 -Usb first, or pass -Hex <file>" }

foreach ($baud in 115200, 57600) {
    Write-Host "Trying $Port at $baud baud..."
    & "$tools\bin\avrdude.exe" -C "$tools\etc\avrdude.conf" -c arduino -p m328p -P $Port -b $baud `
        -U "flash:w:${Hex}:i"
    if ($LASTEXITCODE -eq 0) {
        Write-Host "`nDone. Hold the encoder button while powering on once to reset EEPROM."
        exit 0
    }
}
throw "Flash failed. Check the COM port, switch the radio on before connecting, try another USB port."
