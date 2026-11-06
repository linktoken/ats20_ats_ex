# Flashes the full version to the ATS-20 (ATmega328P) with a USBasp.
#   .\flash.ps1                           -> build\ATS_EX.ino.hex (your own build)
#   .\flash.ps1 -Hex ATS_EX_v1.18m.hex    -> a release file
# Note: ISP flashing erases the USB bootloader. Fuses are NOT changed.
param([string]$Hex = (Join-Path $PSScriptRoot 'build\ATS_EX.ino.hex'))
$ErrorActionPreference = 'Stop'
$tools = "$env:LOCALAPPDATA\Arduino15\packages\arduino\tools\avrdude\8.0.0-arduino1"
$hex   = $Hex
if (-not (Test-Path $hex)) { throw "$hex not found. Run .\build.ps1 first, or pass -Hex <file>" }

# -B 10 = slow ISP clock, works even when the chip runs at reduced speed
& "$tools\bin\avrdude.exe" -C "$tools\etc\avrdude.conf" -c usbasp -p m328p -B 10 `
    -U "flash:w:${hex}:i"
if ($LASTEXITCODE -ne 0) { throw "Flash failed (USBasp connected? driver installed with Zadig?)" }
Write-Host "`nDone. Hold the encoder button while powering on once to reset EEPROM."
