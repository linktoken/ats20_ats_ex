# Builds ATS_EX firmware into .\build\ATS_EX.ino.hex
# Uses size-saving AVR flags (-mcall-prologues -mrelax, ~750 bytes smaller than a plain Arduino IDE build).
$ErrorActionPreference = 'Stop'
$cli    = "$env:LOCALAPPDATA\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
$sketch = Join-Path $PSScriptRoot 'ATS_EX'
$out    = Join-Path $PSScriptRoot 'build'
$flags  = '-mcall-prologues -mrelax'

& $cli compile -b arduino:avr:uno --output-dir $out `
    --build-property "compiler.c.extra_flags=$flags" `
    --build-property "compiler.cpp.extra_flags=$flags" `
    --build-property "compiler.c.elf.extra_flags=$flags" `
    --build-property "upload.maximum_size=32768" `
    $sketch
if ($LASTEXITCODE -ne 0) { throw "Build failed" }
Write-Host "`nHex: $out\ATS_EX.ino.hex  (USBasp flash limit is 32768 bytes)"
