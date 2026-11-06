# Builds ATS_EX firmware.
#   .\build.ps1        -> build\ATS_EX.ino.hex      full version, flash with USBasp (no bootloader, 32 KB)
#   .\build.ps1 -Usb   -> build-usb\ATS_EX.ino.hex  USB version, keeps the bootloader (max 30 KB):
#                         no memory presets, no RDS clock
# Uses size-saving AVR flags (-mcall-prologues -mrelax, ~750 bytes smaller than a plain Arduino IDE build).
param([switch]$Usb)
$ErrorActionPreference = 'Stop'
$cli    = "$env:LOCALAPPDATA\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
$sketch = Join-Path $PSScriptRoot 'ATS_EX'
$flags  = '-mcall-prologues -mrelax'

if ($Usb) {
    $out = Join-Path $PSScriptRoot 'build-usb'
    $cflags = "$flags -DUSB_BUILD=1"
    $max = 30720   # fits the old Nano bootloader (2 KB) and Optiboot (512 B)
} else {
    $out = Join-Path $PSScriptRoot 'build'
    $cflags = $flags
    $max = 32768   # whole flash, ISP (USBasp) only
}

& $cli compile -b arduino:avr:uno --output-dir $out `
    --build-property "compiler.c.extra_flags=$cflags" `
    --build-property "compiler.cpp.extra_flags=$cflags" `
    --build-property "compiler.c.elf.extra_flags=$flags" `
    --build-property "upload.maximum_size=$max" `
    $sketch
if ($LASTEXITCODE -ne 0) { throw "Build failed" }
Write-Host "`nHex: $out\ATS_EX.ino.hex  (limit $max bytes)"
