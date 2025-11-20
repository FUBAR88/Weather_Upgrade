# Weather Upgrade - Settings Reference Guide

Complete reference for all configuration settings in the Weather Upgrade mod.

---

## Table of Contents

1. [Master Settings (WU_Settings.json)](#master-settings)
2. [Manual Weather Settings (WU_ManualWeather.json)](#manual-weather-settings)
3. [Auto Weather Settings (WU_AutoWeather.json)](#auto-weather-settings)
4. [Weather Preset Parameters](#weather-preset-parameters)
5. [Zone Temperature Settings (WU_ZoneTemperatureControl.json)](#zone-temperature-settings)

---

## Master Settings

**File:** `WU_Settings.json`

These are the core settings that control the overall behavior of the Weather Upgrade mod.

### EnableLogging
- **Type:** Integer (0 or 1)
- **Default:** 1
- **Description:** Controls whether the mod writes log files
- **Values:**
  - `1` = Logging enabled (recommended for production)
  - `0` = Logging disabled
- **Notes:** Even with logging enabled, debug logging can be separately controlled

### EnableDebugLogging
- **Type:** Integer (0 or 1)
- **Default:** 0
- **Description:** Enables detailed debug logging for troubleshooting
- **Values:**
  - `1` = Debug logging enabled (creates separate debug log file)
  - `0` = Debug logging disabled (production default)
- **Notes:** Debug logs include drift checks, weather state details, and transition information. Only enable when troubleshooting issues.

### KeepLogsDays
- **Type:** Integer
- **Default:** 30
- **Description:** Number of days to retain log files before automatic cleanup
- **Recommended Values:** 7-30 days
- **Notes:** The mod creates timestamped log files. This setting prevents log folder from growing indefinitely.

### AutoWeatherChanges
- **Type:** Integer (0 or 1)
- **Default:** 0
- **Description:** Controls which weather mode is active
- **Values:**
  - `0` = Manual/Schedule mode (uses WU_ManualWeather.json)
  - `1` = Auto/Random mode (uses WU_AutoWeather.json)
- **Notes:** 
  - **Manual Mode:** Weather changes based on time-of-day schedule you define
  - **Auto Mode:** Weather randomly changes at set intervals

### EnableTemperatureControl
- **Type:** Integer (0 or 1)
- **Default:** 1
- **Description:** Allows weather presets to override environment temperature
- **Values:**
  - `1` = Temperature control enabled
  - `0` = Temperature control disabled (uses default DayZ temperatures)
- **Notes:** When enabled, weather presets can set custom temperatures (e.g., -6°C for snow, 22°C for storms)

### EnableZoneTemperatureControl
- **Type:** Integer (0 or 1)
- **Default:** 0
- **Description:** Enables location-based temperature zones
- **Values:**
  - `1` = Zone temperature control enabled
  - `0` = Zone temperature control disabled (production default)
- **Notes:** Allows different temperatures in specific map locations (e.g., cold mountains, warm bunkers). Requires configuration in `WU_ZoneTemperatureControl.json`

### WeatherCheckInterval
- **Type:** Integer (seconds)
- **Default:** 30
- **Description:** How often the mod checks weather status and logs status updates
- **Recommended Values:** 30-120 seconds
- **Notes:** 
  - Controls template logging frequency (weather status reports)
  - Controls schedule check frequency in manual mode
  - Lower values = more frequent checks/logs (better monitoring, slightly higher CPU usage)
  - Higher values = less frequent checks/logs (less monitoring, lower CPU usage)
  - Applies to both Auto and Manual modes (single setting for both)

---

## Manual Weather Settings

**File:** `WU_ManualWeather.json`

Used when `AutoWeatherChanges` is set to `0`. Controls time-based weather schedules.

### DefaultWeatherPreset
- **Type:** String
- **Default:** "clear"
- **Description:** Fallback preset if schedule lookup fails
- **Notes:** Should match one of your defined preset names

### WeatherPresets
- **Type:** Object/Dictionary
- **Description:** Collection of named weather configurations
- **Format:** `"preset_name": { preset parameters }`
- **Notes:** See [Weather Preset Parameters](#weather-preset-parameters) section below

### WeatherSchedule
- **Type:** Array of objects
- **Description:** Time-based schedule for when presets should be active
- **Format:**
  ```json
  {
      "Time": "HH:MM",
      "Preset": "preset_name",
      "Chance": 100
  }
  ```
- **Fields:**
  - **Time:** Game time in 24-hour format (e.g., "14:30")
  - **Preset:** Name of preset to activate (must exist in WeatherPresets)
  - **Chance:** Currently not used (always set to 100)
- **Notes:** Schedule entries are sorted by time. The active preset is the most recent entry before current game time.

---

## Auto Weather Settings

**File:** `WU_AutoWeather.json`

Used when `AutoWeatherChanges` is set to `1`. Controls random weather changes.

### DefaultWeatherPreset
- **Type:** String
- **Default:** "clear"
- **Description:** Initial weather preset when server starts
- **Notes:** Should match one of your defined preset names

### RandomWeatherChance
- **Type:** Integer (percentage)
- **Default:** 30
- **Description:** Probability of weather change when a preset's duration expires
- **Recommended Values:** 0-100%
- **How It Works:**
  1. Preset is applied with a duration from `m_MinDuration_Min/Max` (e.g., 600-900 seconds)
  2. System randomly picks a duration (e.g., 750 seconds)
  3. After 750 seconds, the system rolls the `RandomWeatherChance` dice
  4. If roll succeeds (≤ 30%), a new random preset is selected
  5. If roll fails (> 30%), the current preset continues
- **Example:** With `RandomWeatherChance: 30`:
  - `clear` preset (duration 600-900s) → rolled 750s → after 750s, 30% chance to switch to another preset
  - If chance fails, the same `clear` preset continues with a new random duration
- **Special Cases:**
  - `0` = Never change automatically (stays on current preset forever)
  - `100` = Always change when duration expires (deterministic rotation)

### WeatherPresets
- **Type:** Object/Dictionary
- **Description:** Collection of named weather configurations
- **Notes:** See [Weather Preset Parameters](#weather-preset-parameters) section below

---

## Weather Preset Parameters

These parameters define individual weather configurations. Used in both Manual and Auto modes.

### Timing Parameters

#### m_TransitionTime_Min / m_TransitionTime_Max
- **Type:** Float (seconds)
- **Range:** 0-3600
- **Description:** How long the transition to this weather takes
- **Notes:** Actual transition time is random between min and max
- **Example:** `120` to `240` = weather transitions over 2-4 minutes

#### m_MinDuration_Min / m_MinDuration_Max
- **Type:** Float (seconds)
- **Range:** 60-7200
- **Description:** How long the weather maintains stable values after transition
- **Notes:** Prevents weather from changing too quickly
- **Example:** `600` to `1200` = weather stays stable for 10-20 minutes

### Weather Effect Parameters

#### m_Overcast_Min / m_Overcast_Max
- **Type:** Float
- **Range:** 0.0 to 1.0
- **Description:** Cloud coverage amount
- **Values:**
  - `0.0` = Clear sky
  - `0.3` = Light clouds
  - `0.6` = Mostly cloudy
  - `1.0` = Completely overcast
- **Notes:** Higher values darken the environment

#### m_Fog_Min / m_Fog_Max
- **Type:** Float
- **Range:** 0.0 to 1.0
- **Description:** Traditional fog density (not volumetric fog)
- **Notes:** 
  - Usually kept at `0` unless you want heavy fog
  - For realistic fog, use volumetric fog instead
  - High fog values significantly reduce visibility

#### m_Rain_Min / m_Rain_Max
- **Type:** Float
- **Range:** 0.0 to 1.0
- **Description:** Rain intensity
- **Values:**
  - `0.0` = No rain
  - `0.3` = Light rain
  - `0.6` = Moderate rain
  - `1.0` = Heavy rain
- **Notes:** Requires `m_Overcast` of at least 0.7 for rain to appear

#### m_Snowfall_Min / m_Snowfall_Max
- **Type:** Float
- **Range:** 0.0 to 1.0
- **Description:** Snow intensity
- **Values:**
  - `0.0` = No snow
  - `0.4` = Light snow
  - `0.7` = Moderate snow
  - `1.0` = Heavy snow
- **Notes:** 
  - Requires cold temperatures to look realistic
  - Use `m_UseEnvironmentTemperature: 1` with negative temperature

### Wind Parameters

#### m_WindSpeed_Min / m_WindSpeed_Max
- **Type:** Float (meters/second)
- **Range:** 0.0 to 20.0
- **Description:** Wind speed
- **Values:**
  - `0.5-2.0` = Light breeze
  - `3.0-7.0` = Moderate wind
  - `8.0-15.0` = Strong wind (storms)
  - `15.0+` = Extreme wind
- **Notes:** Affects tree movement and particle effects

#### m_WindDirection_Min / m_WindDirection_Max
- **Type:** Float (degrees)
- **Range:** 0 to 360
- **Description:** Wind direction
- **Notes:** Usually set to `0` and `360` for full randomization

#### m_WindFunc_Min
- **Type:** Float
- **Range:** 0.0 to 1.0
- **Default:** 0.1
- **Description:** Minimum wind variation multiplier
- **Notes:**
  - Controls natural wind variation minimum
  - Works with `m_WindFunc_Speed` to determine actual range
  - Visible in COT as "Wind Function → Min"

#### m_WindFunc_Max
- **Type:** Float
- **Range:** 0.0 to 1.0
- **Default:** 1.0
- **Description:** Maximum wind variation multiplier
- **Notes:**
  - Controls natural wind variation maximum
  - Works with `m_WindFunc_Speed` to determine actual range
  - Visible in COT as "Wind Function → Max"

#### m_WindFunc_Speed
- **Type:** Float
- **Range:** 0.1 to 100.0
- **Default:** 20.0
- **Description:** Wind function frequency/speed multiplier
- **Notes:**
  - Base value for wind variation calculations
  - Default 20.0 matches vanilla DayZ
  - Visible in COT as "Wind Function → Frequency"

### Volumetric Fog Parameters

#### m_UseDynVolFog
- **Type:** Integer (0 or 1)
- **Description:** Enables volumetric fog for this preset
- **Values:**
  - `1` = Use volumetric fog
  - `0` = No volumetric fog
- **Notes:** Creates realistic 3D fog banks (recommended over traditional fog)

#### m_VolFogDistanceDensity_Min / m_VolFogDistanceDensity_Max
- **Type:** Float
- **Range:** 0.0 to 1.0
- **Description:** How thick the fog is at distance
- **Recommended Values:**
  - `0.2-0.4` = Light fog/haze
  - `0.5-0.7` = Moderate fog
  - `0.8-1.0` = Dense fog

#### m_VolFogHeightDensity_Min / m_VolFogHeightDensity_Max
- **Type:** Float
- **Range:** 0.0 to 1.0
- **Description:** How thick the fog is vertically
- **Notes:** Works with HeightBias to create ground fog or mountain fog

#### m_VolFogHeightBias
- **Type:** Float (meters)
- **Range:** -200 to 200
- **Description:** Vertical offset for fog layer
- **Values:**
  - Negative values = Fog settles low (ground fog)
  - `0` = Fog centered at sea level
  - Positive values = Fog at higher altitude (mountain fog)
- **Example:** `-50` creates low-lying ground fog

#### m_FogTransitionTime
- **Type:** Float (seconds)
- **Default:** 30-60
- **Description:** How long volumetric fog takes to transition
- **Notes:** Can be different from main transition time for effect

### Snow Parameters

#### m_UseSnowflakeScale
- **Type:** Integer (0 or 1)
- **Description:** Enables custom snowflake size
- **Values:**
  - `1` = Use custom snowflake scale
  - `0` = Use default size (1.0)

#### m_SnowflakeScale_Min / m_SnowflakeScale_Max
- **Type:** Float
- **Range:** 0.5 to 3.0
- **Description:** Size multiplier for snowflakes
- **Recommended Values:** `1.0` to `1.5`

#### m_SnowfallThreshold_Min / m_SnowfallThreshold_Max
- **Type:** Float
- **Range:** 0.0 to 1.0
- **Default:** 0.6 to 1.0
- **Description:** Snowfall value required for snow effects to appear
- **Notes:** Usually left at defaults

#### m_SnowfallThreshold_Timeout
- **Type:** Float (seconds)
- **Default:** 20-30
- **Description:** Delay before snow effects activate
- **Notes:** Usually left at defaults

### Rain Parameters

#### m_RainThreshold_Min
- **Type:** Float
- **Range:** 0.0 to 1.0
- **Default:** 0.3
- **Description:** Minimum overcast value required for rain to appear
- **Notes:**
  - Rain only renders when overcast reaches this threshold
  - Lower values = rain starts with lighter cloud cover
  - Visible in COT as "Rain Thresholds → Min"

#### m_RainThreshold_Max
- **Type:** Float
- **Range:** 0.0 to 1.0
- **Default:** 1.0
- **Description:** Maximum overcast value for rain threshold range
- **Notes:**
  - Typically left at 1.0
  - Defines upper bound of rain threshold
  - Visible in COT as "Rain Thresholds → Max"

#### m_RainThreshold_Timeout
- **Type:** Float (seconds)
- **Default:** 30
- **Description:** Transition time for rain threshold changes
- **Notes:**
  - How smoothly rain starts/stops based on overcast
  - Visible in COT as "Rain Thresholds → Transition"

### Storm Parameters

#### m_ThunderstormDensity
- **Type:** Float
- **Range:** 0.0 to 1.0
- **Description:** Intensity of thunderstorm effects
- **Values:**
  - `0.0` = No lightning
  - `0.3` = Occasional lightning
  - `0.7` = Frequent lightning
  - `1.0` = Constant lightning
- **Notes:** Requires high rain and overcast

#### m_ThunderstormThreshold
- **Type:** Float
- **Range:** 0.0 to 1.0
- **Default:** 0.7-0.85
- **Description:** Minimum overcast required for lightning
- **Notes:** Usually left at defaults

#### m_LightningTimeout
- **Type:** Float (seconds)
- **Range:** 10 to 120
- **Description:** Minimum time between lightning strikes
- **Values:**
  - `20` = Frequent strikes
  - `45` = Moderate strikes
  - `60+` = Rare strikes

### Temperature Parameters

#### m_UseEnvironmentTemperature
- **Type:** Integer (0 or 1)
- **Description:** Enables temperature override for this preset
- **Values:**
  - `1` = Override temperature
  - `0` = Use default DayZ temperature
- **Notes:** Only works if `EnableTemperatureControl` is `1` in main settings

#### m_EnvironmentTemperature
- **Type:** Float (Celsius)
- **Range:** -40 to 50
- **Description:** Temperature to set when preset is active
- **Examples:**
  - `-6` = Cold (good for snow)
  - `15` = Mild
  - `22` = Warm (good for storms)
- **Notes:** Affects player temperature and survival mechanics

---

## Zone Temperature Settings

**File:** `WU_ZoneTemperatureControl.json`

Creates location-based temperature zones (disabled by default).

### Zone Object

#### Name
- **Type:** String
- **Description:** Descriptive name for the zone
- **Example:** "Green Mountain", "Underground Bunker"

#### Position
- **Type:** Array of floats [X, Y, Z]
- **Description:** Center point of the zone
- **Format:** `[X, Altitude, Z]`
- **Notes:** 
  - X and Z are map coordinates
  - Y is altitude (height above sea level)
  - Use DayZ Editor or coordinates from in-game to find positions

#### Radius
- **Type:** Float (meters)
- **Description:** Horizontal radius of the zone
- **Example:** `1200` = zone extends 1.2km from center

#### Height
- **Type:** Float (meters)
- **Description:** Vertical height of the zone
- **Example:** `250` = zone extends 250m up from the altitude position

#### Temperature_Min / Temperature_Max
- **Type:** Float (Celsius)
- **Description:** Random temperature range for this zone
- **Example:** `-20` to `-15` for a cold mountain zone
- **Notes:** Actual temperature is randomized between min and max

### Zone Behavior
- Zones override weather preset temperatures when player is inside
- If player is in multiple zones, the first matching zone takes priority
- If player is not in any zone, weather preset temperature is used

---

## Configuration Tips

### For Production Servers
```json
{
    "EnableLogging": 1,
    "EnableDebugLogging": 0,
    "KeepLogsDays": 30,
    "AutoWeatherChanges": 0,
    "EnableTemperatureControl": 1,
    "EnableZoneTemperatureControl": 0,
    "WeatherCheckInterval": 30,
}
```

### For Testing/Debugging
```json
{
    "EnableLogging": 1,
    "EnableDebugLogging": 1,
    "KeepLogsDays": 7,
    "WeatherCheckInterval": 10
}
```

### Common Preset Patterns

**Clear Day:**
- Overcast: 0.0-0.2
- Rain/Snow: 0
- Wind: 0.5-2.0

**Rainy Day:**
- Overcast: 0.8-1.0
- Rain: 0.5-0.8
- Wind: 3.0-7.0
- ThunderstormDensity: 0.3

**Heavy Storm:**
- Overcast: 0.85-1.0
- Rain: 0.6-1.0
- Wind: 8.0-15.0
- ThunderstormDensity: 0.7

**Snowy Winter:**
- Overcast: 0.6-0.8
- Snowfall: 0.4-0.8
- UseEnvironmentTemperature: 1
- EnvironmentTemperature: -6

**Foggy Morning:**
- UseDynVolFog: 1
- VolFogDistanceDensity: 0.4-0.8
- VolFogHeightDensity: 0.3-0.7
- VolFogHeightBias: -50 (ground level)

---

## Need Help?

If you have questions about specific settings or want to create custom weather configurations, refer to the other documentation files:
- `COMPLETE_SYSTEM_v3.0.md` - System overview
- `Weather_Upgrade_Creative_Presets.md` - Example configurations
- `Weather_Upgrade_Volumetric_Fog_Update.md` - Advanced fog techniques

