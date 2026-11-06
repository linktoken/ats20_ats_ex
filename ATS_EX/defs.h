#pragma once

//If you set this def to 0 project will be compiled without RDS 
//and everything related to RDS will be excluded from build
#define USE_RDS 1

//Optional features (1 = on, 0 = off). The USB build (build.ps1 -Usb passes
//-DUSB_BUILD=1) turns off memory presets and S-meter numbers so the firmware
//fits next to the USB bootloader (max 30720 bytes). Each one can also be forced with -D.
#ifndef USB_BUILD
#define USB_BUILD 0
#endif
#ifndef FEATURE_MEMORY          //10 memory presets (long press MODE)
#define FEATURE_MEMORY          (!USB_BUILD)
#endif
#ifndef FEATURE_RDS_CLOCK       //RDS clock page
#define FEATURE_RDS_CLOCK       1
#endif
#ifndef FEATURE_SCREEN_TIMEOUT  //Screen-off timer (setting "Dsp")
#define FEATURE_SCREEN_TIMEOUT  1
#endif
#ifndef FEATURE_STEREO          //FM stereo indicator "ST"
#define FEATURE_STEREO          1
#endif
#ifndef FEATURE_SMETER_NUMBERS  //RSSI/SNR numbers in S-meter (USB build: bars only)
#define FEATURE_SMETER_NUMBERS  (!USB_BUILD)
#endif

#define EEPROM_APP_ID				235
#define EEPROM_DATA_START_ADDRESS	1
#define EEPROM_VERSION_ADDRESS      1000
#define EEPROM_APP_ID_ADDRESS       0
//Memory presets: 10 slots x 4 bytes (band, mode, freq hi, freq lo).
//Kept outside the settings area, so an EEPROM reset doesn't erase them.
#define EEPROM_MEMORY_ADDRESS       900
#define MEMORY_SLOTS                10

//EEPROM Settings
#define STORE_TIME 10000 // Inactive time to save our settings

// OLED Const values
#define DEFAULT_FONT FONT8X16POB
#define RST_PIN -1
#define RESET_PIN 12

//Battery charge monitoring analog pin (Voltage divider 10-10 KOhm directly from battery)
#define BATTERY_VOLTAGE_PIN A1
//Calibration: the ATmega supply voltage that the ADC uses as reference (mV)
//and the divider ratio (10k/10k = 2). If the % reads wrong, measure the
//5V/3.3V pin of the ATmega with a multimeter and put the value here.
#define BATTERY_VREF_MV   3300
#define BATTERY_DIVIDER   2

// Encoder
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3

// Buttons
#define MODE_SWITCH       4 
#define BANDWIDTH_BUTTON  5
#define VOLUME_BUTTON     6
#define AVC_BUTTON        7
#define BAND_BUTTON       8 
#define SOFTMUTE_BUTTON   9
#define AGC_BUTTON       11
#define STEP_BUTTON      10

#define ENCODER_BUTTON   14

// Default values
#define MIN_ELAPSED_TIME 100
#define MIN_ELAPSED_RSSI_TIME 150
#define DEFAULT_VOLUME 25
#define ADJUSTMENT_ACTIVE_TIMEOUT 3000

// Band settings
#define SW_LIMIT_LOW		1710
#define SW_LIMIT_HIGH		30000
#define LW_LIMIT_LOW		153
#define CB_LIMIT_LOW		26200
#define CB_LIMIT_HIGH		28000

#define BAND_DELAY                 2
#define VOLUME_DELAY               1 

#define buttonEvent                NULL