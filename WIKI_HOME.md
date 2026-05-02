# Weather Upgrade Wiki - Home

Welcome to the **Weather Upgrade** mod documentation! This wiki contains everything you need to configure and use the mod on your DayZ server.

---

## Quick Navigation

### Getting Started
- **[Installation Guide](INSTALLATION.md)** - How to install the mod on your server
- **[Quick Start Examples](README.md#configuration)** - Common configurations to get started fast

### Configuration
- **[Master Settings (WU_Settings.json)](SETTINGS_REFERENCE.md#master-settings)** - Main configuration file
- **[Auto Weather Mode](SETTINGS_REFERENCE.md#auto-weather-settings)** - Random weather changes
- **[Manual Weather Mode](SETTINGS_REFERENCE.md#manual-weather-settings)** - Scheduled weather by time
- **[Temperature Zones](SETTINGS_REFERENCE.md#zone-temperature-settings)** - Location-based temperature control

### Reference
- **[All Weather Parameters](SETTINGS_REFERENCE.md#weather-preset-parameters)** - Complete list of preset options
- **[Quick Reference Table](SETTINGS_QUICK_REFERENCE.md)** - Fast lookup for all settings
- **[Changelog](CHANGELOG.md)** - Version history and updates

### Technical Documentation
- **[Weather Cycle Explanation](WEATHER_CYCLE_EXPLANATION.md)** - How auto mode works in detail
- **[Weather Chance Explanation](WEATHER_CHANCE_EXPLANATION.md)** - How weighted selection works

### Development
- **[Known Issues](KNOWN_ISSUES.md)** - Current issues and planned fixes
- **[System Flow](ExpectedWeatherPattens.md)** - Internal system flow documentation
- **[Pre-Launch Checklist](Checklist.md)** - Testing procedures

---

## What is Weather Upgrade?

Weather Upgrade is a comprehensive server-side mod that gives you complete control over DayZ weather. Unlike vanilla DayZ which uses random weather from `cfgweather.xml`, this mod lets you:

- **Schedule weather** by in-game time (e.g., rain every day at 14:00)
- **Randomize weather** with configurable intervals and chances
- **Override temperatures** globally or in specific map locations
- **Control all weather parameters** including fog, wind, snow, rain, storms
- **Monitor weather** via COT's in-game weather panel
- **Complete server-side control** - uses MissionWeather API for precise weather control

---

## Documentation Structure

### User Documentation

| Document | Purpose |
|----------|---------|
| [README.md](README.md) | Main documentation, features, quick start |
| [INSTALLATION.md](INSTALLATION.md) | Step-by-step setup guide |
| [SETTINGS_REFERENCE.md](SETTINGS_REFERENCE.md) | Complete settings guide |
| [SETTINGS_QUICK_REFERENCE.md](SETTINGS_QUICK_REFERENCE.md) | Fast lookup tables |
| [CHANGELOG.md](CHANGELOG.md) | Version history |

### Technical Documentation

| Document | Purpose |
|----------|---------|
| [WEATHER_CYCLE_EXPLANATION.md](WEATHER_CYCLE_EXPLANATION.md) | Detailed auto mode explanation |
| [WEATHER_CHANCE_EXPLANATION.md](WEATHER_CHANCE_EXPLANATION.md) | Weighted selection math |

### Development Documentation

| Document | Purpose |
|----------|---------|
| [KNOWN_ISSUES.md](KNOWN_ISSUES.md) | Bugs, issues, and planned fixes |
| [ExpectedWeatherPattens.md](ExpectedWeatherPattens.md) | Internal system flow |
| [Checklist.md](Checklist.md) | Pre-launch testing checklist |

---

## Configuration Files

| File | Location | Purpose |
|------|----------|---------|
| `WU_Settings.json` | `profiles/Weather_Upgrade/` | Master settings |
| `WU_AutoWeather.json` | `profiles/Weather_Upgrade/` | Auto mode presets |
| `WU_ManualWeather.json` | `profiles/Weather_Upgrade/` | Manual mode presets + schedule |
| `WU_ZoneTemperatureControl.json` | `profiles/Weather_Upgrade/` | Temperature zones |

---

## Need Help?

1. **Check the [Installation Guide](INSTALLATION.md)** for setup instructions
2. **Review the [Settings Reference](SETTINGS_REFERENCE.md)** for configuration details
3. **Enable debug logging** in `WU_Settings.json` and check your logs
4. **Check [Known Issues](KNOWN_ISSUES.md)** for current bugs

---

## Latest Version

**Current:** v2.4.1

**New in v2.4.1:**
- Fixed fire heat being severely reduced when temperature override presets were active
- Vanilla DayZ fireplace/bonfire heat now works correctly with cold weather presets
- Removed artificial 10°C floor that was masking cold preset behavior
- Removed "fire chills you" bug in warm presets

**v2.4.0:**
- Single-file core architecture (`WU_WeatherManager.c` + `WU_ScheduleEntry.c` in `Core/`)
- Schedule `Chance` field now functional in Manual mode
- Fixed RandomInt off-by-one bugs in auto weather selection
- Fixed dead monitoring code and external weather detection
- Player temperature zone data now stored for status logs
- Volumetric fog now uses `m_FogTransitionTime` from config
- Real-world server timestamps in log files (no longer in-game time)
- Separate `LogInterval` setting for independent status log frequency control
- Transition stability fixes (rapid-fire preset changes, log spam eliminated)

**v2.3.8:**
- Smooth snowfall transitions (snowfall no longer stops/resets)
- Snowfall reinforcement system (every 0.5s)
- All weather parameters now transition smoothly

**v2.3.6:**
- Fire heat now works correctly with temperature override

[See Full Changelog](CHANGELOG.md)

---

## External Links

- [GitHub Repository](https://github.com/FUBAR88/Weather_Upgrade)
- [Latest Release](https://github.com/FUBAR88/Weather_Upgrade/releases)
- [Report a Bug](https://github.com/FUBAR88/Weather_Upgrade/issues)

---

**Made for the DayZ Server Community**

