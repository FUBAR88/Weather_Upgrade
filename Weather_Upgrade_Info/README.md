# Weather Upgrade for DayZ

Complete weather control system with automatic/manual scheduling, temperature zones, and drift correction.

---

## Quick Start

### Installation

1. **Copy mod to server:**
   ```
   @Weather_Upgrade\
   └── Addons\
       └── Weather_Upgrade.pbo
   ```

2. **Add to server start parameters:**
   ```
   -mod=@Weather_Upgrade
   ```

3. **Start server** - Config files generate automatically in:
   ```
   DayZServer\profiles\Weather_Upgrade\
   ├── WU_Settings.json
   ├── WU_AutoWeather.json
   ├── WU_ManualWeather.json
   └── WU_ZoneTemperatureControl.json
   ```

### Basic Configuration

Edit `WU_Settings.json`:

```json
{
    "EnableLogging": 1,
    "EnableDebugLogging": 0,
    "KeepLogsDays": 30,
    "AutoWeatherChanges": 0,
    "EnableTemperatureControl": 1,
    "EnableZoneTemperatureControl": 0,
    "DriftCorrectionTime": 60,
    "DriftCorrectionCooldown": 30,
    "DriftTolerance": 0.1
}
```

**Weather Modes:**
- `AutoWeatherChanges: 0` → Manual/Schedule mode (uses `WU_ManualWeather.json`)
- `AutoWeatherChanges: 1` → Auto/Random mode (uses `WU_AutoWeather.json`)

---

## Weather Modes

### Manual Mode (Schedule-Based)

Weather changes based on in-game time schedule.

**Example Schedule:**
```json
"WeatherSchedule": [
    { "Time": "06:00", "Preset": "clear", "Chance": 100 },
    { "Time": "12:00", "Preset": "partly_cloudy", "Chance": 100 },
    { "Time": "18:00", "Preset": "rain", "Chance": 70 }
]
```

- At 06:00 → Always clear
- At 12:00 → Always partly cloudy  
- At 18:00 → 70% chance of rain, 30% chance stays partly cloudy

### Auto Mode (Random Changes)

Weather randomly changes at intervals.

**Example Config:**
```json
{
    "WeatherChangeInterval": 1800,
    "RandomWeatherChance": 30,
    "DefaultWeatherPreset": "clear"
}
```

- Every 30 minutes (1800s)
- 30% chance to randomly change weather
- Starts with "clear" preset

---

## Weather Presets

Each preset defines min/max ranges for randomization:

```json
"snowy": {
    "m_TransitionTime_Min": 180,
    "m_TransitionTime_Max": 240,
    "m_MinDuration_Min": 1800,
    "m_MinDuration_Max": 3600,
    
    "m_Overcast_Min": 0.6,
    "m_Overcast_Max": 0.8,
    "m_Fog_Min": 0,
    "m_Fog_Max": 0,
    "m_Rain_Min": 0,
    "m_Rain_Max": 0,
    "m_Snowfall_Min": 0.5,
    "m_Snowfall_Max": 1.0,
    
    "m_WindSpeed_Min": 2,
    "m_WindSpeed_Max": 6,
    "m_WindDirection_Min": 0,
    "m_WindDirection_Max": 360,
    
    "m_WindFunc_Min": 0.1,
    "m_WindFunc_Max": 1.0,
    "m_WindFunc_Speed": 20.0,
    
    "m_SnowfallThreshold_Min": 0.5,
    "m_SnowfallThreshold_Max": 0.7,
    "m_SnowfallThreshold_Timeout": 20,
    
    "m_RainThreshold_Min": 0.3,
    "m_RainThreshold_Max": 1.0,
    "m_RainThreshold_Timeout": 30,
    
    "m_UseSnowflakeScale": 1,
    "m_SnowflakeScale_Min": 1.0,
    "m_SnowflakeScale_Max": 1.5,
    
    "m_UseDynVolFog": 0,
    "m_ThunderstormDensity": 0,
    
    "m_UseEnvironmentTemperature": 1,
    "m_EnvironmentTemperature": -6
}
```

### Key Parameters

**Transition & Duration:**
- `m_TransitionTime_Min/Max` → How long weather takes to change (seconds)
- `m_MinDuration_Min/Max` → How long weather stays active (seconds)

**Weather Phenomena (0.0-1.0):**
- `m_Overcast` → Cloud coverage (0=clear, 1=fully overcast)
- `m_Fog` → Standard fog density
- `m_Rain` → Rain intensity
- `m_Snowfall` → Snow intensity

**Wind (m/s):**
- `m_WindSpeed` → Wind speed in meters per second (0-20)
- `m_WindDirection` → Wind direction in degrees (0-360)
- `m_WindFunc_Min/Max/Speed` → Wind variation parameters (NEW v3.1)

**Thresholds (NEW v3.1):**
- `m_RainThreshold_Min/Max` → Overcast levels when rain starts/stops
- `m_RainThreshold_Timeout` → Transition time for rain thresholds
- `m_SnowfallThreshold_Min/Max` → Overcast levels when snow starts/stops
- `m_SnowfallThreshold_Timeout` → Transition time for snow thresholds

**Volumetric Fog:**
- `m_UseDynVolFog: 1` → Enable volumetric fog (requires world config support)
- `m_VolFogHeightDensity` → Vertical fog density (0.0-1.0)
- `m_VolFogDistanceDensity` → Horizontal fog density (0.0-1.0)
- `m_VolFogHeightBias` → Height offset in meters

**Temperature:**
- `m_UseEnvironmentTemperature: 1` → Override temperature
- `m_EnvironmentTemperature` → Temperature in Celsius

---

## Temperature Zones

Define location-based temperature overrides in `WU_ZoneTemperatureControl.json`:

```json
{
    "Zones": [
        {
            "Name": "Frozen Bunker",
            "Position": [7291.34, 27.81, 2046.53],
            "Radius": 1200,
            "Height": 250,
            "Temperature_Min": -15,
            "Temperature_Max": -5
        },
        {
            "Name": "Hot Spring",
            "Position": [3699.30, 402.02, 5986.69],
            "Radius": 100,
            "Height": -1,
            "Temperature_Min": 35,
            "Temperature_Max": 45
        }
    ]
}
```

**Parameters:**
- `Position` → [X, Y, Z] coordinates
- `Radius` → Zone radius in meters
- `Height` → Vertical zone height in meters (`-1` = disable height check)
- `Temperature_Min/Max` → Random temperature range in Celsius

**Enable in WU_Settings.json:**
```json
"EnableZoneTemperatureControl": 1
```

---

## Drift Correction System

DayZ's native weather system sometimes overrides your settings. The drift correction system automatically corrects this.

**How it works:**
1. Every `WeatherCheckInterval` (60s), compare current vs expected values
2. If difference > `DriftTolerance` (0.1), trigger correction
3. Smoothly transition back to expected values over `DriftCorrectionTime` (60s)
4. Wait `DriftCorrectionCooldown` (30s) before next correction

**Configuration:**
```json
"DriftCorrectionTime": 60,
"DriftCorrectionCooldown": 30,
"DriftTolerance": 0.1
```

---

## Logging System

### Normal Logs
**File:** `profiles\Weather_Upgrade\Logs\Weather_Upgrade_YYYY_MM_DD_HH_MM_SS.log`

**Contains:**
- Weather mode status
- Preset applications
- Schedule changes
- Drift corrections
- Temperature zone entries/exits
- Configuration loads
- Error messages

### Debug Logs (Optional)
**File:** `profiles\Weather_Upgrade\Logs\Debug_Weather_Upgrade_YYYY_MM_DD_HH_MM_SS.log`

**Enable:**
```json
"EnableDebugLogging": 1
```

**Contains:**
- Detailed weather state every 60s
- Current vs Expected values for all parameters
- Drift calculations
- Wind function parameters
- Volumetric fog status
- Player temperature tracking (5s intervals)

**Use when:**
- Troubleshooting weather drift issues
- Debugging temperature zones
- Verifying preset application
- Working with support

---

## Compatibility

### COT (Community Online Tools)
- Fully compatible
- COT's weather monitor displays your mod's values
- Use COT to verify wind function, rain/snow thresholds are applied correctly

### Map Mods
- Works on all maps (Chernarus, Livonia, Namalsk, DeerIsle, etc.)
- Some maps override `Environment` class - zone temperature uses backup polling system
- Volumetric fog requires world config support (map-dependent)

### Other Weather Mods
- **DO NOT** run multiple weather mods simultaneously
- Weather Upgrade takes full control via `MissionWeather(true)`
- Conflicts will cause drift and unpredictable behavior

---

## Troubleshooting

### Weather Not Changing
1. Check `AutoWeatherChanges` in `WU_Settings.json` matches your intended mode
2. Verify correct config file is populated (Auto vs Manual)
3. Check logs for errors: `profiles\Weather_Upgrade\Logs\`
4. Ensure `WeatherCheckInterval` is reasonable (60s recommended)

### Weather Drifts Back
1. Enable debug logging to see drift details
2. Increase `DriftTolerance` if corrections are too frequent
3. Check for conflicting mods (COT weather, other weather mods)
4. Verify `DriftCorrectionTime` and `DriftCorrectionCooldown` are set

### Temperature Zones Not Working
1. Verify `EnableZoneTemperatureControl: 1` in `WU_Settings.json`
2. Enable debug logging to see zone entry/exit events
3. Check zone coordinates match your map
4. Set `Height: -1` to disable vertical checks if terrain varies
5. Some maps override `Environment` - plugin handles this automatically

### Snow/Rain Not Rendering
1. Check `m_SnowfallThreshold` aligns with `m_Overcast` values
2. Snow requires overcast ≥ threshold to render visually
3. Verify `m_Snowfall` > 0 in preset
4. Check client-side rendering settings

### COT Shows Different Values
1. This was a bug in v3.0 and earlier - fixed in v3.1
2. Wind Function parameters are now configurable (no longer hardcoded)
3. Rain/Snow thresholds are now properly applied
4. Update to v3.1+ if seeing mismatched values

---

## Version History

**v3.1 (Current - 2025-11-01)**
- ✅ Added Wind Function parameters (fixes COT drift)
- ✅ Added Rain Threshold controls
- ✅ Fixed hardcoded `SetWindFunctionParams`
- ✅ Added `SetRainThresholds` API call
- ✅ Updated drift correction to apply wind params
- ✅ All presets now include 6 new fields

**v3.0**
- Enhanced logging system (INFO + DEBUG modes)
- Temperature zone system with height controls
- Configurable drift correction parameters
- Player zone tracking with entry/exit logs
- Environment override diagnostics

**v2.0**
- Volumetric fog support
- Snowflake scale control
- Enhanced weather status logging
- Improved startup behavior

**v1.0**
- Initial release
- Manual/Auto weather modes
- 9 default presets
- Basic drift correction

---

## Support & Documentation

**Full Settings Reference:** See `SETTINGS_REFERENCE.md` for complete parameter documentation

**Changelog:** See `CHANGELOG.md` for detailed version history

**Issues:**
- Enable debug logging
- Check logs in `profiles\Weather_Upgrade\Logs\`
- Verify configuration syntax (JSON validator)

---

## Credits

**Developed for DayZ Server Administration**  
**Compatible with DayZ 1.25+**

*Note: This mod is server-side with optional client-side components for temperature zones.*

