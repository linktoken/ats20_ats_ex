# Flashes .\build\ATS_EX.ino.hex to the ATS-20 (ATmega328P) with a USBasp.
# Note: ISP flashing erases the USB bootloader. Fuses are NOT changed.
$ErrorActionPreference = 'Stop'
$tools = "$env:LOCALAPPDATA\Arduino15\packages\arduino\tools\avrdude\8.0.0-arduino1"
$hex   = Join-Path $PSScriptRoot 'build\ATS_EX.ino.hex'
if (-not (Test-Path $hex)) { throw "Run build.ps1 first ($hex not found)" }

# -B 10 = slow ISP clock, works even when the chip runs at reduced speed
& "$tools\bin\avrdude.exe" -C "$tools\etc\avrdude.conf" -c usbasp -p m328p -B 10 `
    -U "flash:w:${hex}:i"
if ($LASTEXITCODE -ne 0) { throw "Flash failed (USBasp connected? driver installed with Zadig?)" }
Write-Host "`nDone. Hold the encoder button while powering on once to reset EEPROM."
