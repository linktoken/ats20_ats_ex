#pragma once

long g_storeTime = millis();

bool g_voltagePinConnnected = false;
bool g_ssbLoaded = false;
bool g_fmStereo = false;     //FM pilot detected (shows "ST")
bool g_cmdMemory = false;    //Memory preset mode active
int8_t g_memSlot = 0;        //Selected memory slot 0..9
bool g_autoOff = false;      //Display was turned off by the screen timeout
bool g_wakeHold = false;     //Ignore the key that woke the display until it's released
uint32_t g_lastActivity = 0; //Last button/encoder activity (for screen timeout)

bool g_cmdVolume = false;
bool g_cmdStep = false;
bool g_cmdBw = false;
bool g_cmdBand = false;
bool g_settingsActive = false;
bool g_sMeterOn = false;
bool g_displayOn = true;
bool g_displayRDS = false;
bool g_rdsSwitchPressed = false;
bool g_seekStop = false;
uint32_t g_lastAdjustmentTime = 0;

uint8_t g_muteVolume = 0;
int g_currentBFO = 0;

// Encoder buttons
SimpleButton  btn_Bandwidth(BANDWIDTH_BUTTON);
SimpleButton  btn_BandUp(BAND_BUTTON);
SimpleButton  btn_BandDn(SOFTMUTE_BUTTON);
SimpleButton  btn_VolumeUp(VOLUME_BUTTON);
SimpleButton  btn_VolumeDn(AVC_BUTTON);
SimpleButton  btn_Encoder(ENCODER_BUTTON);
SimpleButton  btn_AGC(AGC_BUTTON);
SimpleButton  btn_Step(STEP_BUTTON);
SimpleButton  btn_Mode(MODE_SWITCH);

volatile int g_encoderCount = 0;

//Frequency tracking
uint16_t g_currentFrequency;
uint16_t g_previousFrequency;

enum SettingType
{
    ZeroAuto,
    Num,
    Switch,
    SwitchAuto
};

struct SettingsItem
{
    char name[5];
    int8_t param;
    uint8_t type;
    void (*manipulateCallback)(int8_t);
};

void doAttenuation(int8_t v);
void doSoftMute(int8_t v);
void doBrightness(int8_t v);
void doSSBAVC(int8_t v = 0);
void doAvc(int8_t v);
void doSync(int8_t v = 0);
void doDeEmp(int8_t v = 0);
void doSWUnits(int8_t v = 0);
void doSSBSoftMuteMode(int8_t v = 0);
void doCutoffFilter(int8_t v);
void doCPUSpeed(int8_t v = 0);
#if USE_RDS
void doRDSErrorLevel(int8_t v);
#endif
void doBFOCalibration(int8_t v);
void doUnitsSwitch(int8_t v = 0);
void doScanSwitch(int8_t v = 0);
void doCWSwitch(int8_t v = 0);
void doBatUnits(int8_t v = 0);
void doScreenTimeout(int8_t v);

SettingsItem g_Settings[] =
{
    //Page 1
    { "ATT", 0,  SettingType::ZeroAuto,     doAttenuation     },  //Attenuation
    { "SM ", 0,  SettingType::Num,          doSoftMute        },  //Soft Mute
    { "SVC", 1,  SettingType::Switch,       doSSBAVC          },  //SSB AVC Switch
    { "Syn", 0,  SettingType::Switch,       doSync            },  //SSB Sync
    { "DeE", 1,  SettingType::Switch,       doDeEmp           },  //FM DeEmphasis (0 - 50, 1 - 75)
    { "AVC", 46, SettingType::Num,          doAvc             },  //Automatic Volume Control
    //Page 2
    { "Scr", 80, SettingType::Num,          doBrightness      },  //Screen Brightness
    { "SW ", 0,  SettingType::Switch,       doSWUnits         },  //SW Units
    { "SSM", 1,  SettingType::Switch,       doSSBSoftMuteMode },  //SSB Soft Mute Mode
    { "COF", 0,  SettingType::SwitchAuto,   doCutoffFilter    },  //SSB Cutoff Filter
    { "CPU", 0,  SettingType::Switch,       doCPUSpeed        },  //CPU Frequency
#if USE_RDS
    { "RDS", 1,  SettingType::Num,          doRDSErrorLevel   },  //RDS ErrorLevel
#endif
    //Page 3
    { "BFO", 0,  SettingType::Num,          doBFOCalibration  },  //BFO Offset calibration
    { "Uni", 1,  SettingType::Switch,       doUnitsSwitch     },  //Show/Hide frequency units
    { "Sca", 1,  SettingType::Switch,       doScanSwitch      },  //AM Encoder scan switch
    { "CW ", 0,  SettingType::Switch,       doCWSwitch        },  //CW is LSB or USB
    { "Bat", 1,  SettingType::Switch,       doBatUnits        },  //Battery: 0 - percent, 1 - volts
#if FEATURE_SCREEN_TIMEOUT
    { "Dsp", 0,  SettingType::Num,          doScreenTimeout   },  //Screen off after N seconds (0 - never)
#endif
};

enum SettingsIndex
{
    ATT,
    SoftMute,
    SVC,
    Sync,
    DeEmp,
    AutoVolControl,
    Brightness,
    SWUnits,
    SSM,
    CutoffFilter,
    CPUSpeed,
#if USE_RDS
    RDSError,
#endif
    BFO,
    UnitsSwitch,
    ScanSwitch,
    CWSwitch,
    BatUnits,
#if FEATURE_SCREEN_TIMEOUT
    ScreenTimeout,
#endif
    SETTINGS_MAX
};

const uint8_t g_SettingsMaxPages = 3;
int8_t g_SettingSelected = 0;
int8_t g_SettingsPage = 1;
bool g_SettingEditing = false;

//For managing BW
struct Bandwidth
{
    uint8_t idx;      //Internal SI473X index
    const char* desc;
};

int8_t g_bwIndexSSB = 4;
Bandwidth g_bandwidthSSB[] =
{
    { 4, "0.5k" },
    { 5, "1.0k" },
    { 0, "1.2k" },
    { 1, "2.2k" },
    { 2, "3.0k" },
    { 3, "4.0k" }
};
const uint8_t g_bwSSBMaxIdx = 5;

int8_t g_bwIndexAM = 4;
const uint8_t g_maxFilterAM = 6;
Bandwidth g_bandwidthAM[] =
{
    { 4, "1.0k" }, // 0
    { 5, "1.8k" }, // 1
    { 3, "2.0k" }, // 2
    { 6, "2.5k" }, // 3
    { 2, "3.0k" }, // 4 - Default
    { 1, "4.0k" }, // 5
    { 0, "6.0k" }  // 6
};

int8_t g_bwIndexFM = 0;
char* g_bandwidthFM[] =
{
    "AUTO",
    "110k",
    " 84k",
    " 60k",
    " 40k"
};

int g_tabStep[] =
{
    // AM steps in KHz
    1,
    5,
    9,
    10,
    // Large AM steps in KHz
    50,
    100,
    1000,
    // SSB steps in Hz
    10,
    25,
    50,
    100,
    500
};
uint8_t g_amTotalSteps = 7;
uint8_t g_amTotalStepsSSB = 4; //Prevent large AM steps appear in SSB mode
uint8_t g_ssbTotalSteps = 5;
volatile int8_t g_stepIndex = 3;

int8_t g_tabStepFM[] =
{
    5,  // 50 KHz
    10, // 100 KHz
    100 // 1 MHz
};
int8_t g_FMStepIndex = 1;
const int8_t g_lastStepFM = (sizeof(g_tabStepFM) / sizeof(int8_t)) - 1;

//Band table structures
enum BandType : uint8_t
{
    LW_BAND_TYPE,
    MW_BAND_TYPE,
    SW_BAND_TYPE,
    FM_BAND_TYPE
};

struct Band
{
    uint16_t minimumFreq;
    uint16_t maximumFreq;
    uint16_t currentFreq;
    int8_t currentStepIdx;
    int8_t bandwidthIdx;     // Bandwidth table index (internal table in Si473x controller)
};

#if USE_RDS
enum RDSActiveInfo : uint8_t
{
    StationName,
    StationInfo,
    ProgramInfo,
    Clock
};
uint8_t g_rdsActiveInfo = RDSActiveInfo::StationName;
char g_rdsPrevLen = 0;
char* g_RDSCells[4];
//RDS clock: local minutes of day from the last time signal (-1: none yet),
//then kept running with millis()
int16_t g_clockMinutes = -1;
uint32_t g_clockBase = 0;
char g_clockText[11] = "Time --:--";
#endif

char _literal_EmptyLine[17] = "                ";

char* bandTags[] =
{
    "LW",
    "MW",
    "SW",
    "  ",    //It looks better
};

Band g_bandList[] =
{
    /* LW */ { LW_LIMIT_LOW, 520, 300, 0, 4 },
    /* MW */ { 520, 1710, 1476, 3, 4 },
    /* SW */ { SW_LIMIT_LOW, SW_LIMIT_HIGH, SW_LIMIT_LOW, 0, 4 },
    /* FM */ { 6400, 10800, 8400, 1, 0 },
};

uint16_t SWSubBands[] =
{
    //Must stay sorted ascending. BC = broadcast band, HAM = amateur band
    SW_LIMIT_LOW,  // 160m HAM
    2300,  // 120m BC
    3200,  // 90m BC
    3500,  // 80m HAM
    3900,  // 75m BC
    4750,  // 60m BC
    5900,  // 49m BC
    7000,  // 40m HAM
    7200,  // 41m BC
    9400,  // 31m BC
    10100, // 30m HAM
    11600, // 25m BC
    13570, // 22m BC
    14000, // 20m HAM
    15100, // 19m BC
    17480, // 16m BC
    18068, // 17m HAM
    18900, // 15m BC
    21000, // 15m HAM
    21450, // 13m BC
    24890, // 12m HAM
    25670, // 11m BC
    CB_LIMIT_LOW, // CB Band (11 Meter)
    CB_LIMIT_HIGH  // 10m HAM
};
const uint8_t g_SWSubBandCount = sizeof(SWSubBands) / sizeof(uint16_t);
const uint8_t g_lastBand = (sizeof(g_bandList) / sizeof(Band)) - 1;
int8_t g_bandIndex = 1;

// Modulation
enum Modulations : uint8_t
{
    AM,
    LSB,
    USB,
    CW,
    FM
};
volatile uint8_t g_currentMode = FM;
const char* g_bandModeDesc[] = 
{ 
    "AM ",
    "LSB",
    "USB",
    "CW ",
    "FM "
};
volatile uint8_t g_prevMode = FM;
uint8_t g_seekDirection = 1;

//Special logic for fast and responsive frequency surfing
uint32_t g_lastFreqChange = 0;
bool g_processFreqChange = 0;
uint8_t g_volume = DEFAULT_VOLUME;

Rotary g_encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);
SI4735 g_si4735;