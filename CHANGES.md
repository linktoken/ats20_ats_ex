# ATS_EX v1.18m — changes from v1.18

Build: `pwsh -File .\build.ps1`   Flash (USBasp): `pwsh -File .\flash.ps1`
After the first flash, hold the **encoder button** while switching on (EEPROM reset).

## New features
| Feature | How to use |
|---|---|
| Battery in volts | Lower-right shows `3.9V`. Settings page 3 **Bat**: `V` or `%` |
| Low-battery warning | Battery value turns **inverted** (white box) below 3.30 V |
| FM stereo indicator | Top line shows `FM ST` when the station is stereo |
| Signal numbers | S-meter (long-press STEP) now shows `|||||  R45 SN12` = bars, RSSI dBµV, SNR dB |
| Screen-off timer | Settings page 3 **Dsp**: 0 (never) … 120 s. Any key/turn wakes the screen (that key does nothing else) |
| More SW bands | BAND+/- in SW jumps through broadcast bands 120m…11m and ham bands |
| Memory presets (10) | **Long-press MODE** → `M0 9650k AM`. Turn encoder = slot. **Encoder button** = recall. **Short-press MODE** = store current station. Closes after 3 s idle. Presets survive EEPROM reset |
| RDS clock | FM, RDS on (MODE): press encoder to cycle Name → Info → Text → **Time**. Shows `Time --:--` until the station sends a time signal (once per minute) |

Note: long-press MODE no longer acts like a short press (it opens memory presets).

## Bug fixes
- RDS wrote through a NULL pointer (corrupted CPU register r17) — random glitches on FM.
- AGC/STEP buttons passed an uninitialized value as a callback address.
- BFO calibration setting sent its value to the RDS config.
- EEPROM reads relied on unspecified evaluation order; corrupted band/mode bytes are rejected.
- Battery showed 0% for the first 10 s; battery pin moved A2 → A1; calibration in `defs.h`
  (`BATTERY_VREF_MV`, `BATTERY_DIVIDER`).
- `USE_RDS 0` build works again.

## Two versions
- **Full (USBasp):** 31,438 / 32,768 bytes, no bootloader. `build.ps1`, flash with `flash.ps1`.
- **USB:** 30,648 / 30,720 bytes, keeps the bootloader. Same as full, including the RDS clock, but
  without memory presets (long-press MODE = short press, as in the original) and with a bars-only
  S-meter. `build.ps1 -Usb`, flash with `flash-usb.ps1 -Port COMx` or XLoader.
- Feature switches in `defs.h`: `FEATURE_MEMORY`, `FEATURE_RDS_CLOCK`, `FEATURE_SCREEN_TIMEOUT`,
  `FEATURE_STEREO`, `FEATURE_SMETER_NUMBERS` (`USB_BUILD=1` turns off memory and S-meter numbers).
- RDS clock decodes the raw RDS group 4A (~350 bytes smaller than the library's time functions).
