# Weather Upgrade for DayZ

[![Version](https://img.shields.io/badge/version-3.1-blue.svg)](https://github.com/yourusername/Weather_Upgrade)
[![DayZ](https://img.shields.io/badge/DayZ-1.25+-green.svg)](https://dayz.com)
[![License](https://img.shields.io/badge/license-MIT-orange.svg)](LICENSE)

> Complete server-side weather control system with automatic/manual scheduling, temperature zones, and drift correction for DayZ servers.

---

## 📋 Table of Contents

- [Features](#-features)
- [Installation](#-installation)
- [Quick Start](#-quick-start)
- [Configuration Guide](#-configuration-guide)
  - [Master Settings](#1-master-settings-wu_settingsjson)
  - [Auto Weather Mode](#2-auto-weather-mode-wu_autoweatherjson)
  - [Manual Weather Mode](#3-manual-weather-mode-wu_manualweatherjson)
  - [Temperature Zones](#4-temperature-zones-wu_zonetemperaturecontroljson)
- [Weather Preset Parameters](#-weather-preset-parameters)
- [Examples](#-examples)
- [Troubleshooting](#-troubleshooting)
- [Compatibility](#-compatibility)
- [Changelog](#-changelog)
- [Support](#-support)

---

## ✨ Features

- 🌦️ **Two Weather Modes**
  - **Auto Mode**: Random weather changes at configurable intervals
  - **Manual Mode**: Time-based scheduled weather (e.g., rain at 14:00)

- 🌡️ **Temperature Control**
  - Global temperature override per weather preset
  - Location-based temperature zones (hot/cold areas on the map)
  - Height-based zones for sky bunkers or underground areas

- ⚡ **Advanced Features**
  - Drift correction system (prevents DayZ from overriding your settings)
  - Volumetric fog support (realistic 3D fog banks)
  - Wind function control (natural wind variation)
  - Rain/Snow threshold configuration
  - Thunderstorm and lightning control
  - Customizable snowflake size

- 📊 **Logging System**
  - Normal logs for production monitoring
  - Debug logs for troubleshooting
  - Player temperature tracking in zones
  - Weather drift detection and correction logs

- 🔧 **Admin Friendly**
  - Automatic config generation on first start
  - JSON-based configuration (easy to edit)
  - Compatible with COT (Community Online Tools)
  - Works on all maps (Chernarus, Livonia, Namalsk, DeerIsle, etc.)

---

## 📦 Installation

### Server Setup

1. **Download** the latest release from [Releases](https://github.com/yourusername/Weather_Upgrade/releases)

2. **Copy** the mod folder to your server:
   ```
   DayZServer/
   └── @Weather_Upgrade/
       └── Addons/
           └── Weather_Upgrade.pbo
   ```

3. **Add** to your server start parameters:
   ```bat
   -mod=@Weather_Upgrade
   ```

4. **Start** the server - Configuration files generate automatically in:
   ```
   DayZServer/profiles/Weather_Upgrade/
   ├── WU_Settings.json
   ├── WU_AutoWeather.json
   ├── WU_ManualWeather.json
   └── WU_ZoneTemperatureControl.json
   ```

5. **Configure** the JSON files to your preference

6. **Restart** the server to apply changes

### Client Setup (Optional)

For temperature zone features, clients should also load the mod:
```bat
-mod=@Weather_Upgrade
```
> ⚠️ The mod works server-side only, but client-side installation enhances temperature zone functionality.

---

## 🚀 Quick Start

### Example 1: Always Snowy Weather

Edit `WU_Settings.json`:
```json
{
    "AutoWeatherChanges": 1
}
```

Edit `WU_AutoWeather.json`:
```json
{
    "DefaultWeatherPreset": "snowy",
    "WeatherChangeInterval": 1800,
    "RandomWeatherChance": 0
}
```

**Result**: Server always has snowy weather with 0% chance of random changes.

---

### Example 2: Scheduled Weather

Edit `WU_Settings.json`:
```json
{
    "AutoWeatherChanges": 0
}
```

Edit `WU_ManualWeather.json`:
```json
{
    "WeatherSchedule": [
        { "Time": "06:00", "Preset": "clear", "Chance": 100 },
        { "Time": "12:00", "Preset": "partly_cloudy", "Chance": 100 },
        { "Time": "18:00", "Preset": "rain", "Chance": 70 }
    ]
}
```

**Result**: 
- 06:00 → Always clear
- 12:00 → Always partly cloudy
- 18:00 → 70% chance rain, 30% stays cloudy

---

### Example 3: Hot Zone

Edit `WU_Settings.json`:
```json
{
    "EnableZoneTemperatureControl": 1
}
```

Edit `WU_ZoneTemperatureControl.json`:
```json
{
    "Zones": [
        {
            "Name": "Bunker Heater",
            "Position": [7291.34, 27.81, 2046.53],
            "Radius": 1200,
            "Height": 250,
            "Temperature_Min": 35,
            "Temperature_Max": 45
        }
    ]
}
```

**Result**: Players within 1200m of coordinates, up to 250m high, experience 35-45°C heat.

---

## ⚙️ Configuration Guide

### 1. Master Settings (`WU_Settings.json`)

Controls which weather mode is active and global features.

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

| Setting | Type | Default | Description |
|---------|------|---------|-------------|
| `EnableLogging` | 0/1 | 1 | Creates log files in `profiles/Weather_Upgrade/Logs/` |
| `EnableDebugLogging` | 0/1 | 0 | Creates detailed debug logs (for troubleshooting) |
| `KeepLogsDays` | number | 30 | Auto-delete logs older than X days |
| `AutoWeatherChanges` | 0/1 | 0 | **0**=Manual Mode, **1**=Auto Mode |
| `EnableTemperatureControl` | 0/1 | 1 | Allow presets to override temperature |
| `EnableZoneTemperatureControl` | 0/1 | 0 | Enable location-based temperature zones |
| `DriftCorrectionTime` | seconds | 60 | How long drift corrections take |
| `DriftCorrectionCooldown` | seconds | 30 | Minimum time between corrections |
| `DriftTolerance` | 0.0-1.0 | 0.1 | How much drift before correction triggers |

---

### 2. Auto Weather Mode (`WU_AutoWeather.json`)

Random weather changes at intervals (used when `AutoWeatherChanges: 1`).

```json
{
    "DefaultWeatherPreset": "clear",
    "WeatherCheckInterval": 60,
    "WeatherChangeInterval": 1800,
    "RandomWeatherChance": 30,
    "WeatherPresets": { ... }
}
```

| Setting | Type | Default | Description |
|---------|------|---------|-------------|
| `DefaultWeatherPreset` | string | "clear" | Preset to use on server start (or `""` for random) |
| `WeatherCheckInterval` | seconds | 60 | How often to check/correct weather |
| `WeatherChangeInterval` | seconds | 1800 | How often to roll for random weather change |
| `RandomWeatherChance` | 0-100 | 30 | % chance to change weather each interval |

**How it works:**
- Every `WeatherChangeInterval` (e.g., 1800s = 30 min)
- Roll 1-100
- If roll ≤ `RandomWeatherChance` (30%), pick random preset and apply
- Example: 30% chance every 30 min = ~1 change per 100 minutes average

---

### 3. Manual Weather Mode (`WU_ManualWeather.json`)

Scheduled weather by in-game time (used when `AutoWeatherChanges: 0`).

```json
{
    "DefaultWeatherPreset": "clear",
    "WeatherCheckInterval": 60,
    "WeatherPresets": { ... },
    "WeatherSchedule": [
        {
            "Time": "14:00",
            "Preset": "rain",
            "Chance": 100
        }
    ]
}
```

| Setting | Type | Description |
|---------|------|-------------|
| `DefaultWeatherPreset` | string | Fallback preset if no schedule matches |
| `WeatherCheckInterval` | seconds | How often to check game time vs schedule |
| `WeatherSchedule` | array | List of time-based weather entries |

**Schedule Entry:**
- `Time`: Game time in "HH:MM" format (24-hour)
- `Preset`: Name of preset to apply
- `Chance`: 0-100% chance to apply (allows randomness)

---

### 4. Temperature Zones (`WU_ZoneTemperatureControl.json`)

Location-based temperature overrides.

```json
{
    "Zones": [
        {
            "Name": "Frozen Mountain",
            "Position": [3699.30, 402.02, 5986.69],
            "Radius": 100,
            "Height": -1,
            "Temperature_Min": -15,
            "Temperature_Max": -5
        }
    ]
}
```

| Setting | Type | Description |
|---------|------|-------------|
| `Name` | string | Zone name (for logs) |
| `Position` | [X, Y, Z] | Center coordinates of zone |
| `Radius` | meters | Horizontal radius of zone |
| `Height` | meters | Vertical height of zone (`-1` = disable height check) |
| `Temperature_Min` | °C | Minimum temperature |
| `Temperature_Max` | °C | Maximum temperature (random between min/max) |

**Getting Coordinates:**
1. Use in-game admin tools (F2 in COT)
2. Stand at desired location
3. Copy position coordinates
4. Paste into JSON

---

## 🌤️ Weather Preset Parameters

Each preset in `WeatherPresets` defines min/max ranges. The mod randomly picks values between these ranges.

### Basic Structure

```json
"my_preset": {
    "m_TransitionTime_Min": 120,
    "m_TransitionTime_Max": 240,
    "m_MinDuration_Min": 600,
    "m_MinDuration_Max": 1200,
    
    "m_Overcast_Min": 0.0,
    "m_Overcast_Max": 1.0,
    "m_Fog_Min": 0.0,
    "m_Fog_Max": 0.0,
    "m_Rain_Min": 0.0,
    "m_Rain_Max": 0.5,
    "m_Snowfall_Min": 0.0,
    "m_Snowfall_Max": 0.0,
    
    "m_WindSpeed_Min": 2,
    "m_WindSpeed_Max": 6,
    "m_WindDirection_Min": 0,
    "m_WindDirection_Max": 360,
    
    "m_WindFunc_Min": 0.1,
    "m_WindFunc_Max": 1.0,
    "m_WindFunc_Speed": 20.0,
    
    "m_RainThreshold_Min": 0.3,
    "m_RainThreshold_Max": 1.0,
    "m_RainThreshold_Timeout": 30,
    
    "m_SnowfallThreshold_Min": 0.6,
    "m_SnowfallThreshold_Max": 1.0,
    "m_SnowfallThreshold_Timeout": 30,
    
    "m_UseDynVolFog": 0,
    "m_VolFogDistanceDensity_Min": 0.0,
    "m_VolFogDistanceDensity_Max": 0.0,
    "m_VolFogHeightDensity_Min": 0.0,
    "m_VolFogHeightDensity_Max": 0.0,
    "m_VolFogHeightBias": 0,
    "m_FogTransitionTime": 30,
    
    "m_UseSnowflakeScale": 0,
    "m_SnowflakeScale_Min": 1.0,
    "m_SnowflakeScale_Max": 1.0,
    
    "m_ThunderstormDensity": 0.0,
    "m_ThunderstormThreshold": 0.8,
    "m_LightningTimeout": 45,
    
    "m_UseEnvironmentTemperature": 0,
    "m_EnvironmentTemperature": 15
}
```

### Parameter Reference

#### Timing

| Parameter | Range | Description |
|-----------|-------|-------------|
| `m_TransitionTime_Min/Max` | seconds | How long weather takes to change (120-240 = 2-4 min) |
| `m_MinDuration_Min/Max` | seconds | How long weather stays stable (600-1200 = 10-20 min) |

#### Weather Effects (0.0 - 1.0)

| Parameter | Range | Description | Visual Effect |
|-----------|-------|-------------|---------------|
| `m_Overcast_Min/Max` | 0.0-1.0 | Cloud coverage | 0=clear sky, 1=fully overcast |
| `m_Fog_Min/Max` | 0.0-1.0 | Traditional fog | Usually 0 (use volumetric fog instead) |
| `m_Rain_Min/Max` | 0.0-1.0 | Rain intensity | 0=no rain, 0.5=moderate, 1=heavy |
| `m_Snowfall_Min/Max` | 0.0-1.0 | Snow intensity | 0=no snow, 0.5=moderate, 1=blizzard |

#### Wind (m/s)

| Parameter | Range | Description |
|-----------|-------|-------------|
| `m_WindSpeed_Min/Max` | 0-20 m/s | Wind speed (0.5-2=breeze, 3-7=moderate, 8-15=strong, 15+=extreme) |
| `m_WindDirection_Min/Max` | 0-360° | Wind direction (0=North, 90=East, 180=South, 270=West) |
| `m_WindFunc_Min` | 0.0-1.0 | Minimum wind variation multiplier (default: 0.1) |
| `m_WindFunc_Max` | 0.0-1.0 | Maximum wind variation multiplier (default: 1.0) |
| `m_WindFunc_Speed` | 0-100 | Wind function frequency (default: 20.0) |

> 💡 **Wind Function**: Controls natural wind variation. Default values (0.1, 1.0, 20.0) match vanilla DayZ. Visible in COT weather monitor.

#### Thresholds

| Parameter | Range | Description |
|-----------|-------|-------------|
| `m_RainThreshold_Min` | 0.0-1.0 | Minimum overcast for rain to appear (default: 0.3) |
| `m_RainThreshold_Max` | 0.0-1.0 | Maximum overcast for rain threshold (default: 1.0) |
| `m_RainThreshold_Timeout` | seconds | Rain threshold transition time (default: 30) |
| `m_SnowfallThreshold_Min` | 0.0-1.0 | Minimum overcast for snow to appear (default: 0.6) |
| `m_SnowfallThreshold_Max` | 0.0-1.0 | Maximum overcast for snow threshold (default: 1.0) |
| `m_SnowfallThreshold_Timeout` | seconds | Snow threshold transition time (default: 30) |

#### Volumetric Fog

| Parameter | Range | Description |
|-----------|-------|-------------|
| `m_UseDynVolFog` | 0/1 | Enable volumetric fog (realistic 3D fog) |
| `m_VolFogDistanceDensity_Min/Max` | 0.0-1.0 | Horizontal fog thickness (0.2-0.4=light, 0.5-0.7=moderate, 0.8-1.0=dense) |
| `m_VolFogHeightDensity_Min/Max` | 0.0-1.0 | Vertical fog thickness |
| `m_VolFogHeightBias` | -200 to 200 | Height offset in meters (negative=ground fog, positive=mountain fog) |
| `m_FogTransitionTime` | seconds | Fog transition time (default: 30) |

#### Snow

| Parameter | Range | Description |
|-----------|-------|-------------|
| `m_UseSnowflakeScale` | 0/1 | Enable custom snowflake size |
| `m_SnowflakeScale_Min/Max` | 0.5-3.0 | Snowflake size multiplier (1.0=normal, 1.5=larger) |

#### Storms

| Parameter | Range | Description |
|-----------|-------|-------------|
| `m_ThunderstormDensity` | 0.0-1.0 | Lightning intensity (0=none, 0.3=occasional, 0.7=frequent, 1.0=constant) |
| `m_ThunderstormThreshold` | 0.0-1.0 | Minimum overcast for lightning (default: 0.7-0.85) |
| `m_LightningTimeout` | seconds | Minimum time between strikes (10=frequent, 120=rare) |

#### Temperature

| Parameter | Range | Description |
|-----------|-------|-------------|
| `m_UseEnvironmentTemperature` | 0/1 | Override global temperature for this preset |
| `m_EnvironmentTemperature` | °C | Temperature value (-20 to 50°C realistic range) |

---

## 💡 Examples

### Always Snowy

```json
"snowy": {
    "m_Overcast_Min": 0.65,
    "m_Overcast_Max": 0.65,
    "m_Snowfall_Min": 0.6,
    "m_Snowfall_Max": 1.0,
    "m_WindSpeed_Min": 2,
    "m_WindSpeed_Max": 6,
    "m_SnowfallThreshold_Min": 0.5,
    "m_SnowfallThreshold_Max": 0.7,
    "m_UseEnvironmentTemperature": 1,
    "m_EnvironmentTemperature": -6
}
```

### Heavy Rain Storm

```json
"storm": {
    "m_Overcast_Min": 0.85,
    "m_Overcast_Max": 1.0,
    "m_Rain_Min": 0.6,
    "m_Rain_Max": 1.0,
    "m_WindSpeed_Min": 8,
    "m_WindSpeed_Max": 15,
    "m_RainThreshold_Min": 0.3,
    "m_RainThreshold_Max": 1.0,
    "m_ThunderstormDensity": 0.7,
    "m_ThunderstormThreshold": 0.7,
    "m_LightningTimeout": 20
}
```

### Ground Fog

```json
"foggy": {
    "m_Overcast_Min": 0.5,
    "m_Overcast_Max": 0.8,
    "m_UseDynVolFog": 1,
    "m_VolFogDistanceDensity_Min": 0.6,
    "m_VolFogDistanceDensity_Max": 0.9,
    "m_VolFogHeightDensity_Min": 0.5,
    "m_VolFogHeightDensity_Max": 0.8,
    "m_VolFogHeightBias": -30,
    "m_WindSpeed_Min": 0.3,
    "m_WindSpeed_Max": 1.5
}
```

---

## 🔧 Troubleshooting

### Weather Not Changing

**Check:**
1. Verify `AutoWeatherChanges` matches your intended mode (0=Manual, 1=Auto)
2. Check correct config file is populated (`WU_AutoWeather.json` or `WU_ManualWeather.json`)
3. Review logs in `profiles/Weather_Upgrade/Logs/`
4. Ensure `WeatherCheckInterval` is reasonable (60s recommended)

### Weather Keeps Resetting

**Cause:** DayZ's native system overriding your settings (drift)

**Solution:**
1. Enable debug logging: `"EnableDebugLogging": 1`
2. Check drift correction settings in `WU_Settings.json`
3. Increase `DriftTolerance` if corrections are too frequent
4. Check for conflicting mods (other weather mods, COT weather active)

### Temperature Zones Not Working

**Check:**
1. `"EnableZoneTemperatureControl": 1` in `WU_Settings.json`
2. Enable debug logging to see zone entry/exit events
3. Verify coordinates match your map (use COT F2 position)
4. Set `"Height": -1` to disable vertical checks if terrain varies
5. Zone logs appear in normal logs, temperature tracking in debug logs

### Snow/Rain Not Rendering

**Check:**
1. Overcast value must be ≥ threshold
   - Rain needs: `m_Overcast` ≥ `m_RainThreshold_Min` (default 0.3)
   - Snow needs: `m_Overcast` ≥ `m_SnowfallThreshold_Min` (default 0.6)
2. Verify `m_Rain` or `m_Snowfall` > 0
3. Check client graphics settings (particle effects enabled)

### COT Shows Wrong Values

**Solution:** Update to v3.1+

Wind Function and Rain Thresholds were hardcoded in v3.0 and earlier, causing COT to show incorrect values. v3.1 fixes this by making them configurable per-preset.

---

## 🔗 Compatibility

### COT (Community Online Tools)
✅ **Fully Compatible**
- COT's weather monitor displays your mod's values
- Use COT to verify wind function, rain/snow thresholds are correct
- Both mods can coexist without conflicts

### Maps
✅ **All Maps Supported**
- Chernarus
- Livonia  
- Namalsk
- DeerIsle
- Custom maps

> ⚠️ Some maps override `Environment` class. Temperature zones use a backup polling system that works regardless.

### Other Mods
❌ **Weather Mods**
- Do NOT run multiple weather mods simultaneously
- Weather Upgrade takes full control via `MissionWeather(true)`

✅ **Other Mods**
- Compatible with most gameplay mods
- Compatible with base building, zombie mods, etc.

---

## 📝 Changelog

### v3.1 (2025-11-01) - Current

**New:**
- Wind Function parameters (fixes COT compatibility)
  - `m_WindFunc_Min/Max/Speed` - configurable per preset
- Rain Threshold parameters
  - `m_RainThreshold_Min/Max/Timeout`
- Proper API calls: `SetWindFunctionParams()`, `SetRainThresholds()`

**Fixed:**
- Wind speed drift caused by hardcoded values
- COT monitor showing incorrect wind values
- Drift correction not applying wind parameters

### v3.0 (2025-10-30)

**New:**
- Enhanced logging (INFO + DEBUG modes)
- Temperature zone system with height controls
- Configurable drift correction parameters
- Player zone tracking with entry/exit logs

### v2.0 (2025-10-24)

**New:**
- Volumetric fog support
- Snowflake scale control
- Enhanced weather status logging

### v1.0

**Initial Release:**
- Manual/Auto weather modes
- 9 default presets
- Basic drift correction

[Full Changelog](Weather_Upgrade_Info/CHANGELOG.md)

---

## 📖 Documentation

- **[Complete Settings Reference](Weather_Upgrade_Info/SETTINGS_REFERENCE.md)** - Detailed parameter documentation
- **[Full Changelog](Weather_Upgrade_Info/CHANGELOG.md)** - Version history with technical details
- **[README](Weather_Upgrade_Info/README.md)** - Installation and configuration guide

---

## 💬 Support

### Getting Help

1. **Enable debug logging:**
   ```json
   "EnableDebugLogging": 1
   ```

2. **Check logs:**
   ```
   profiles/Weather_Upgrade/Logs/
   ├── Weather_Upgrade_YYYY_MM_DD_HH_MM_SS.log
   └── Debug_Weather_Upgrade_YYYY_MM_DD_HH_MM_SS.log
   ```

3. **Verify JSON syntax:**
   - Use a JSON validator (jsonlint.com)
   - Check for missing commas, brackets

4. **Open an issue:**
   - [GitHub Issues](https://github.com/yourusername/Weather_Upgrade/issues)
   - Include log files
   - Describe expected vs actual behavior

### Common Questions

**Q: Does this work on client-side?**
A: Mod is server-side, but clients can load it for enhanced temperature zone features.

**Q: Will this conflict with map weather?**
A: No, the mod overrides all default weather via `MissionWeather(true)`.

**Q: Can I add more presets?**
A: Yes! Copy an existing preset block, rename it, adjust values.

**Q: How do I reset to defaults?**
A: Delete all JSON files from `profiles/Weather_Upgrade/` and restart server.

---

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🙏 Credits

**Developed for the DayZ Server Community**

Special thanks to:
- DayZ modding community
- COT developers for weather API reference
- Server admins providing feedback

---

## ⭐ Support This Project

If you find this mod useful:
- ⭐ Star this repository
- 🐛 Report bugs via [Issues](https://github.com/yourusername/Weather_Upgrade/issues)
- 💡 Suggest features via [Discussions](https://github.com/yourusername/Weather_Upgrade/discussions)
- 📢 Share with other server owners

---

**Made with ❤️ for DayZ Server Owners**

*Compatible with DayZ 1.25+*

