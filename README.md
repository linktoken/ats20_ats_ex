# ATS_EX v1.18m — modified firmware for the ATS-20 / ATS-20+ receiver

This is a fork of **[goshante/ats20_ats_ex](https://github.com/goshante/ats20_ats_ex)** (ATS_EX v1.18 by Goshante) for the ATS-20 (Si4732 + ATmega328P) receiver.
It adds bug fixes, a battery voltage display, memory presets, an FM stereo indicator, an RDS clock and more. It also includes the upstream fix from PR #33.

The original user manual still applies; see **[README_ORIGINAL.md](README_ORIGINAL.md)** ([Russian](rus/README.md)). This page lists only what's **new or different** in v1.18m.

![ATS-20](img/ats20.png)

---

## Download — two versions

Download from the **[Releases](../../releases)** page:

| File | Flash with | Size | Features |
|---|---|---|---|
| **`ATS_EX_v1.18m_USB.hex`** | **The radio's USB cable** (XLoader / avrdude), like the original firmware. No programmer needed. | 30.0 KB | Everything **except memory presets and RDS clock** |
| **`ATS_EX_v1.18m.hex`** | **USBasp** (ISP programmer). Removes the USB bootloader. | 31.3 KB | **All** features |

**Not sure which?** Take the **USB** version. You can switch to the full version later if you get a USBasp.

Both versions store settings the same way, so switching between them is safe.

You can also build them yourself (see [Building](#building)).

---

## What's new

### New features
| Feature | Description | USB | USBasp |
|---|---|:-:|:-:|
| **Battery in volts** | Lower-right corner shows battery voltage, e.g. `3.9V`. Setting **Bat** switches between `V` and `%`. | ✅ | ✅ |
| **Low battery warning** | The battery value is shown **inverted** (white box) when the battery drops below **3.30 V**. | ✅ | ✅ |
| **FM stereo indicator** | The top line shows `FM ST` while a stereo station is received. | ✅ | ✅ |
| **Signal numbers in S-meter** | The S-meter line shows bars plus numbers: `||||||  R45 SN12` = RSSI 45 dBµV, SNR 12 dB. | ✅ | ✅ |
| **Screen-off timer** | Setting **Dsp**: turn the display off after 10…120 s without use (0 = never). Any key or encoder turn wakes it up; that key does nothing else. | ✅ | ✅ |
| **More SW band jumps** | BAND+/BAND− in SW now steps through all broadcast bands (120 m … 11 m) as well as the ham bands. | ✅ | ✅ |
| **New boot screen** | Shows the version and a loading bar. | ✅ | ✅ |
| **All bug fixes** | See below. | ✅ | ✅ |
| **10 memory presets** | Store and recall favourite stations (band, mode, frequency). See [Memory presets](#memory-presets). | — | ✅ |
| **RDS clock** | A 4th RDS page shows local time `Time hh:mm` from the station's RDS time signal. | — | ✅ |

### Bug fixes
- **RDS could corrupt CPU state:** the library returns `NULL` for RDS groups it didn't decode, and the old code wrote into that pointer, which on AVR is CPU register r17. This caused random glitches on FM. RDS text is now also cut to 16 characters and cleared when you change station.
- **AGC / STEP buttons:** an uninitialized variable was passed as the button callback, so a press could jump to a random flash address.
- **BFO calibration** no longer sends its value to the RDS configuration.
- **EEPROM:** 16-bit values are read in a defined order, and a corrupted band/mode byte is rejected instead of indexing past the tables.
- **Battery** no longer shows 0 % for the first 10 seconds after power-on.
- Building with `USE_RDS 0` works again.

### Changed behaviour
- **Long-press MODE** now opens memory presets in the USBasp version. Previously it acted like a short press, and it still does in the USB version.
- **Settings page 3** has two new items, **Bat** and **Dsp**.
- The first boot after flashing resets settings to defaults, because the settings layout changed.

---

## Controls (new and changed only)

### Memory presets
*(USBasp version only)*

| Action | Result |
|---|---|
| **Long-press MODE** | Open memory presets. The lower line shows e.g. `M3  9650k AM` (or `M3 ----` if empty). |
| **Turn encoder** | Choose slot **M0 … M9**. |
| **Press encoder** | **Recall** the selected slot (band, mode and frequency). |
| **Short-press MODE** | **Store** the current station in the selected slot. |
| *(no input for 3 s)* | Close the memory line. |

Presets are stored apart from the settings, so they **survive an EEPROM reset**.

### RDS pages (FM)
Short-press **MODE** in FM to turn RDS on, then press the **encoder** to cycle through:
**Station name → Station info → Radio text → Time**. The **Time** page is in the USBasp version only.

The time shows `Time --:--` until the station sends its time signal. Stations send it once a minute, and not all stations send it. After that the radio keeps the time on its own.

### S-meter
Long-press **STEP** as before. The new layout is `||||||||R45 SN12`:
- 8 bars for 0…64 dBµV
- **R** = RSSI in dBµV
- **SN** = signal-to-noise ratio in dB

### New settings (page 3)
| Name | Values | Default | Meaning |
|---|---|---|---|
| **Bat** | `%` / `V` | `V` | Show battery as percent or volts |
| **Dsp** | 0 … 120 (s, steps of 10) | 0 | Screen-off timeout, 0 = never |

---

## Hardware: battery voltage monitor (A1)

Battery monitoring needs a small hardware mod (same as the original firmware), but this version reads **A1** instead of A2.

```
Battery + ──[10 kΩ]──┬──[10 kΩ]── GND
                     │
                     └── A1 (ATmega328P / Arduino pin A1)
```

If no divider is connected (A1 reads low at power-on), the battery indicator stays hidden.

**Calibration:** the reading assumes the ATmega runs at **3.30 V** and a **1:2** divider. If your voltage is off, measure the ATmega supply voltage with a multimeter and edit `ATS_EX/defs.h`:
```c
#define BATTERY_VREF_MV   3300   // ATmega supply voltage in mV
#define BATTERY_DIVIDER   2      // 10k/10k divider
```

---

## Flashing over USB (USB version)

This works like the original ATS_EX firmware. You only need the radio's USB cable.

1. **Switch the radio on first**, then connect it to the PC. A **USB 3.0** port helps, because it supplies more current.
2. Find the COM port: Windows **Device Manager → Ports (COM & LPT)**, e.g. `USB-SERIAL CH340 (COM5)`. If no port appears, install the **CH340** driver.
3. Flash using **one** of these:
   - **XLoader** (Windows GUI):
     1. Hex file: `ATS_EX_v1.18m_USB.hex`
     2. Device: **Uno(ATmega328)** or **Nano(ATmega328)**. Try the other one if it fails.
     3. Select your COM port, then press **Upload**.
   - **flash-usb.ps1** from this repo (uses the avrdude that comes with Arduino IDE; it tries both bootloader speeds):
     ```powershell
     .\flash-usb.ps1 -Port COM5 -Hex ATS_EX_v1.18m_USB.hex
     ```
   - **avrdude** directly. Use `-b 115200` for new bootloaders and Uno-type boards, or `-b 57600` for old Nano bootloaders:
     ```bash
     avrdude -c arduino -p m328p -P COM5 -b 115200 -U flash:w:ATS_EX_v1.18m_USB.hex:i
     ```
4. **Reset settings:** hold the **encoder button** while switching the radio on.

If flashing fails, see "My receiver fails to flash" in [README_ORIGINAL.md](README_ORIGINAL.md).

---

## Flashing with USBasp (full version)

### 1. Connect the USBasp
Connect the USBasp's 6-pin (or 10-pin) ISP cable to the ATmega328P ISP pins: the ICSP header on Nano boards, or the matching pads on Uno-type ATS-20 boards.

| ISP signal | ATmega328P pin | Arduino pin |
|---|---|---|
| MISO | PB4 | D12 |
| MOSI | PB3 | D11 |
| SCK | PB5 | D13 |
| RESET | PC6 | RESET |
| GND | GND | GND |
| VCC | VCC | see below |

> ⚠️ **Power:** many ATS-20 boards run the ATmega and the Si4732 at **3.3 V**. Either set the USBasp **voltage jumper to 3.3 V**, or **don't connect VCC** and power the radio from its own battery (switch it on) while flashing. Feeding 5 V into a 3.3 V board can damage the Si4732.

### 2. Install the driver (Windows, one time)
Plug in the USBasp, run **[Zadig](https://zadig.akeo.ie/)**, select **USBasp**, and install the **libusbK** or **WinUSB** driver.

### 3. Flash
With the ready-made hex (Arduino IDE installed, which includes avrdude):
```powershell
.\flash.ps1
```
Or with avrdude directly:
```bash
avrdude -c usbasp -p m328p -B 10 -U flash:w:ATS_EX_v1.18m.hex:i
```
`flash.ps1` expects the hex at `build\ATS_EX.ino.hex`. Run `build.ps1` first, or copy the release file there with that name.

Fuses are **not** changed. ISP flashing **erases the USB bootloader**, so later updates also need the USBasp. You can burn the bootloader back with Arduino IDE → *Burn Bootloader*, but then v1.18m no longer fits.

### 4. Reset settings
After flashing, **hold the encoder button while switching the radio on** (EEPROM reset). Memory presets are kept.

---

## Building

You need **Arduino IDE 2.x** (or arduino-cli) with:
- Board package **Arduino AVR Boards** (`arduino:avr`)
- Libraries (Library Manager): **PU2CLR SI4735** (tested with 2.1.8), **Tiny4kOLED** (2.3.0), **TinyOLED-Fonts** (1.2.0)

Then run, on Windows PowerShell:
```powershell
.\build.ps1          # full version  -> build\ATS_EX.ino.hex      (USBasp, max 32768 bytes)
.\build.ps1 -Usb     # USB version   -> build-usb\ATS_EX.ino.hex  (bootloader, max 30720 bytes)
```

The USB version is the same source built with `-DUSB_BUILD=1`, which turns off `FEATURE_MEMORY` and `FEATURE_RDS_CLOCK`. Every feature has a switch in `ATS_EX/defs.h`, so you can choose your own set:

| Switch | Flash cost |
|---|---|
| `FEATURE_MEMORY` | ~710 B |
| `FEATURE_RDS_CLOCK` | ~690 B |
| `FEATURE_SCREEN_TIMEOUT` | ~230 B |
| `FEATURE_STEREO` | ~145 B |
| `FEATURE_SMETER_NUMBERS` | ~80 B |

For example, to build with memory presets but no RDS clock:
```powershell
arduino-cli compile ... --build-property "compiler.cpp.extra_flags=-mcall-prologues -mrelax -DFEATURE_RDS_CLOCK=0"
```
To stay USB-flashable, keep the size at **30,720 bytes or less**.

`build.ps1` compiles with `-mcall-prologues -mrelax`, which saves about 750 bytes, and allows the full 32 KB.
A plain Arduino IDE build is **too big**, because the IDE doesn't use these flags and reserves space for the bootloader.

Equivalent arduino-cli command (any OS):
```bash
arduino-cli compile -b arduino:avr:uno --output-dir build \
  --build-property "compiler.c.extra_flags=-mcall-prologues -mrelax" \
  --build-property "compiler.cpp.extra_flags=-mcall-prologues -mrelax" \
  --build-property "compiler.c.elf.extra_flags=-mcall-prologues -mrelax" \
  --build-property "upload.maximum_size=32768" \
  ATS_EX
```

Options in `ATS_EX/defs.h`:
- `USE_RDS 0` builds without RDS, about 1.3 KB smaller.
- `BATTERY_*` values for calibration.
- `EEPROM_MEMORY_ADDRESS` / `MEMORY_SLOTS` for the presets.

---

## Known limitations
- With **CPU 50 %** enabled, the screen-off timer runs twice as slow, and the RDS clock compensates only roughly.
- Memory presets store SSB frequencies rounded to 1 kHz.
- Not tested on every ATS-20 hardware revision. Please report issues.

---

## Credits & license
- Original **ATS_EX** firmware © **Goshante** — https://github.com/goshante/ats20_ats_ex
- Upstream fix PR #33 by **dl9sec**
- **PU2CLR SI4735** library by Ricardo Lima Caratti
- v1.18m modifications by **linktoken**

Licensed under the **MIT License**, same as the original; see [LICENCE](LICENCE).
