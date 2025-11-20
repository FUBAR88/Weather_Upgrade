```
┌─────────────────────────────────────────────────────────────────────────────┐
│                         WEATHER_UPGRADE MOD FLOW                            │
└─────────────────────────────────────────────────────────────────────────────┘

SERVER STARTUP
│
├─► MissionServer Constructor
│   │
│   ├─► Initialize Logger (V2.3.4)
│   ├─► Rotate Log Files
│   ├─► UNLOCK WEATHER FIRST (MissionWeather(false), Wide Limits)
│   │
│   └─► Load WU_Settings.json
│       │
│       └─► Check DisableWeatherUpgrade
│           │
│           ├─► If DisableWeatherUpgrade = 1
│           │   └─► Keep Weather UNLOCKED
│           │
│           └─► If DisableWeatherUpgrade = 0
│               └─► Take Weather Control (MissionWeather(true))
│
├─► MissionServer OnInit
│   │
│   ├─► Initialize Weather Manager
│   ├─► Load Configs (WU_AutoWeather.json OR WU_ManualWeather.json)
│   ├─► Load Weather Presets
│   └─► Initialize Zone Temperature (if enabled)
│
└─► Weather Manager Start()
    │
    ├─► Check DisableWeatherUpgrade
    │   │
    │   ├─► If DISABLED (DisableWeatherUpgrade = 1)
    │   │   │
    │   │   └─► MONITORING MODE
    │   │       │
    │   │       ├─► Log Template (Initial State)
    │   │       ├─► Set m_Started = true
    │   │       └─► Return (No Weather Control)
    │   │
    │   └─► If ENABLED (DisableWeatherUpgrade = 0)
    │       │
    │       ├─► Load Default Preset
    │       ├─► Generate Random Values
    │       ├─► Prepare Initial Config
    │       └─► Set m_Started = true
    │
    └─► ONUPDATE LOOP (Every Frame)
        │
        ├─► Increment m_WeatherTime
        │
        ├─► Check DisableWeatherUpgrade
        │   │
        │   ├─► If DISABLED → MONITORING MODE
        │   │   │
        │   │   ├─► Every 60s: Re-unlock Weather (Prevent DataCache Lock)
        │   │   ├─► Every 10s: Check External Weather Changes
        │   │   ├─► Every WeatherCheckInterval: Log Template
        │   │   └─► Return (No Weather Control)
        │   │
        │   └─► If ENABLED → WEATHER CONTROL MODE
        │       │
        │       ├─► INITIAL APPLICATION (First 8 seconds)
        │       │   │
        │       │   ├─► 0.5s: Application 1/5 (Override DataCache)
        │       │   ├─► 1.5s: Application 2/5 (Reinforce)
        │       │   ├─► 3.0s: Application 3/5 (Ensure Values Stick)
        │       │   ├─► 5.0s: Application 4/5 (Reinforce)
        │       │   └─► 8.0s: Application 5/5 (Final - TIGHT Limits)
        │       │       │
        │       │       └─► Log Template (Initial State)
        │       │
        │       ├─► PERIODIC CHECKS
        │       │   │
        │       │   ├─► Every 30s: Reload Configs (Hot Reload)
        │       │   ├─► Every 60s: Re-assert MissionWeather Control
        │       │   └─► Every WeatherCheckInterval: Log Template
        │       │
        │       ├─► PRESET DURATION CHECK
        │       │   │
        │       │   ├─► Calculate Elapsed Time (m_WeatherTime - m_PresetApplyTime)
        │       │   ├─► Compare to m_CurrentWeather.m_MinDuration
        │       │   │
        │       │   └─► If Duration Expired
        │       │       │
        │       │       ├─► Check AutoWeatherChanges Mode
        │       │       │   │
        │       │       │   ├─► AUTO MODE
        │       │       │   │   │
        │       │       │   │   ├─► Check RandomWeatherChance (30%)
        │       │       │   │   │
        │       │       │   │   ├─► If Random Triggered
        │       │       │   │   │   │
        │       │       │   │   │   ├─► Select Random Preset
        │       │       │   │   │   ├─► Generate Random Values
        │       │       │   │   │   ├─► Start Transition
        │       │       │   │   │   └─► Apply New Preset
        │       │       │   │   │
        │       │       │   │   └─► If Random NOT Triggered
        │       │       │   │       │
        │       │       │   │       └─► Re-apply Current Preset (New Random Values)
        │       │       │   │
        │       │       │   └─► MANUAL MODE (Scheduled)
        │       │       │       │
        │       │       │       ├─► Check Current Game Time
        │       │       │       ├─► Find Active Preset from Schedule
        │       │       │       ├─► If Preset Changed
        │       │       │       │   │
        │       │       │       │   ├─► Generate Random Values
        │       │       │       │   ├─► Start Transition
        │       │       │       │   └─► Apply New Preset
        │       │       │       │
        │       │       │       └─► If Same Preset
        │       │       │           └─► Continue Current Preset
        │       │       │
        │       │       └─► If Duration NOT Expired
        │       │           └─► Continue Current Preset
        │       │
        │       └─► TRANSITION HANDLING
        │           │
        │           ├─► If In Transition
        │           │   │
        │           │   ├─► Calculate Transition Progress
        │           │   ├─► Interpolate Values (Old → New)
        │           │   ├─► Apply Interpolated Values
        │           │   │
        │           │   └─► If Transition Complete
        │           │       │
        │           │       ├─► Set m_InTransition = false
        │           │       ├─► Apply Final Values
        │           │       ├─► Set TIGHT Limits
        │           │       └─► Update m_PresetApplyTime
        │           │
        │           └─► If NOT In Transition
        │               └─► Normal Operation (Weather Status Logging)
        │
        └─► ZONE TEMPERATURE CONTROL (If Enabled)
            │
            ├─► Every ZoneCheckInterval (60s)
            │   │
            │   ├─► Check All Configured Zones
            │   ├─► Check All Players
            │   ├─► Calculate Player Distance to Zones
            │   ├─► Apply Zone Temperature Override
            │   └─► Log Zone Template
            │
            └─► Every 5s (Fast Check)
                │
                └─► Check Player Entry/Exit from Zones


┌─────────────────────────────────────────────────────────────────────────────┐
│                            WEATHER APPLICATION                              │
└─────────────────────────────────────────────────────────────────────────────┘

ApplyWeatherConfig()
│
├─► Check DisableWeatherUpgrade
│   └─► If Disabled → Return (No Application)
│
├─► Get Current Weather Values
├─► Calculate Transition (if needed)
│
├─► SET LIMITS
│   │
│   ├─► If Immediate Application OR Final Application
│   │   └─► TIGHT Limits (±0.005)
│   │
│   └─► If Transition
│       └─► WIDE Limits (0.0 - 1.0)
│
├─► APPLY VALUES
│   │
│   ├─► Overcast.Set()
│   ├─► Fog.Set()
│   ├─► Rain.Set()
│   ├─► Snowfall.Set()
│   ├─► Wind.Set()
│   ├─► WindDirection.Set()
│   ├─► WindFunctionParams.Set()
│   ├─► SnowflakeScale.Set()
│   │
│   ├─► Rain Thresholds (If Rain > 0)
│   │   └─► SetRainThresholds(Min, Max, Timeout)
│   │
│   ├─► Snow Thresholds (If Snowfall > 0)
│   │   └─► SetSnowfallThresholds(Min, Max, Timeout)
│   │
│   └─► Volumetric Fog (If Enabled)
│       └─► SetDynVolFogHeightDensity/DistanceDensity/HeightBias()
│
├─► TEMPERATURE OVERRIDE
│   └─► GetEnvironmentTemperature() → Override Value
│
└─► UPDATE STATE
    │
    ├─► m_CurrentWeather = config
    ├─► m_PresetApplyTime = m_WeatherTime
    └─► m_LastAppliedPreset = preset name


┌─────────────────────────────────────────────────────────────────────────────┐
│                              TEMPLATE LOGGING                               │
└─────────────────────────────────────────────────────────────────────────────┘

WU_WeatherTemplate.LogWeatherStatus()
│
├─► SYSTEM STATUS
│   ├─► Mode (AUTO/MANUAL/MONITORING)
│   ├─► Weather Control (ENABLED/DISABLED)
│   ├─► MissionWeather Control (ACTIVE/RELEASED)
│   ├─► Check Interval
│   └─► Random Weather Chance
│
├─► PRESET TIMING
│   ├─► Active Preset Name
│   ├─► Elapsed Time
│   ├─► Remaining Duration
│   ├─► Transition Status
│   └─► Next Schedule Time
│
├─► WEATHER VALUES (Current | Expected | Drift | Status)
│   ├─► Overcast
│   ├─► Fog
│   ├─► Rain
│   ├─► Snowfall
│   ├─► Wind Speed
│   ├─► Wind Direction
│   ├─► Temperature
│   ├─► Volumetric Fog
│   └─► Snowflake Scale
│
├─► LIMITS
│   ├─► Current Min/Max
│   └─► TIGHT_TOLERANCE (±0.005)
│
└─► CORRECTION STATUS
    └─► All Values OK / Needs Correction


┌─────────────────────────────────────────────────────────────────────────────┐
│                         ZONE TEMPERATURE LOGGING                            │
└─────────────────────────────────────────────────────────────────────────────┘

WU_ZoneTemperatureTemplate.LogZoneTemperatureStatus()
│
├─► SYSTEM STATUS
│   ├─► Zone Control Enabled/Disabled
│   ├─► Zone Check Interval
│   ├─► Last/Next Check Time
│   └─► Total Zones Configured
│
├─► ZONE CONFIGURATION
│   ├─► Zone Name
│   ├─► Position (X, Y, Z)
│   ├─► Radius
│   ├─► Height
│   ├─► Temperature Range (Min/Max)
│   ├─► Average Temperature
│   └─► Variance
│
└─► PLAYER STATUS
    ├─► Player Name
    ├─► Position
    ├─► Zone Status (Inside/Outside)
    ├─► Distance to Zone
    ├─► Base Temperature
    ├─► Zone Temperature Override
    └─► Temperature Change Applied


┌─────────────────────────────────────────────────────────────────────────────┐
│                              CONFIG FILES                                   │
└─────────────────────────────────────────────────────────────────────────────┘

WU_Settings.json
├─► DisableWeatherUpgrade (0/1)
├─► EnableLogging (0/1)
├─► EnableDebugLogging (0/1)
├─► KeepLogsDays (30)
├─► AutoWeatherChanges (0/1)
├─► EnableTemperatureControl (0/1)
└─► EnableZoneTemperatureControl (0/1)

WU_AutoWeather.json (If AutoWeatherChanges = 1)
├─► DefaultWeatherPreset
├─► WeatherCheckInterval (30s)
├─► RandomWeatherChance (30%)
└─► Presets[]
    ├─► Name
    ├─► MinDuration
    ├─► Overcast (Min/Max)
    ├─► Fog (Min/Max)
    ├─► Rain (Min/Max)
    ├─► Snowfall (Min/Max)
    └─► Wind (Speed/Direction/Function)

WU_ManualWeather.json (If AutoWeatherChanges = 0)
├─► WeatherCheckInterval (60s)
└─► Schedule[]
    ├─► Time (HH:MM)
    └─► Preset Name

WU_ZoneTemperatureControl.json (If EnableZoneTemperatureControl = 1)
├─► ZoneCheckInterval (60s)
└─► Zones[]
    ├─► Name
    ├─► Position [X, Y, Z]
    ├─► Radius
    ├─► Height
    ├─► Temperature_Min
    └─► Temperature_Max


┌─────────────────────────────────────────────────────────────────────────────┐
│                              KEY CONCEPTS                                   │
└─────────────────────────────────────────────────────────────────────────────┘

TIGHT LIMITS (±0.005)
└─► Used during stable periods to prevent drift
    └─► Applied after initial application and transitions complete

WIDE LIMITS (0.0 - 1.0)
└─► Used during transitions to allow smooth interpolation
    └─► Applied when changing presets

WEATHER STATUS MONITORING
└─► Continuous monitoring and logging of weather values
    └─► Uses MissionWeather API to maintain weather stability

TRANSITION
└─► Smooth interpolation between old and new preset values
    └─► Duration based on TransitionTime from config

MONITORING MODE
└─► When DisableWeatherUpgrade = 1
    └─► Only logs weather state, does NOT control weather
    └─► Continuously unlocks weather to prevent DataCache lock

DATA CACHE OVERRIDE
└─► 5-step application process (0.5s, 1.5s, 3s, 5s, 8s)
    └─► Forces override of DayZ's persistent weather storage
    └─► Ensures mod values take precedence

```

