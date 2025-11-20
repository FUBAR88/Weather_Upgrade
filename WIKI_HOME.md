# Weather Upgrade Wiki - Home

Welcome to the **Weather Upgrade** mod documentation! This wiki contains everything you need to configure and use the mod on your DayZ server.

---

## 📚 Quick Navigation

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

### Help
- **[Full Documentation](README.md)** - Main documentation page
- **[Settings Reference](SETTINGS_REFERENCE.md)** - Complete settings guide

---

## 🎯 What is Weather Upgrade?

Weather Upgrade is a comprehensive server-side mod that gives you complete control over DayZ weather. Unlike vanilla DayZ which uses random weather from `cfgweather.xml`, this mod lets you:

- **Schedule weather** by in-game time (e.g., rain every day at 14:00)
- **Randomize weather** with configurable intervals and chances
- **Override temperatures** globally or in specific map locations
- **Control all weather parameters** including fog, wind, snow, rain, storms
- **Monitor weather** via COT's in-game weather panel
- **Complete server-side control** - uses MissionWeather API for precise weather control

---

## 📖 Documentation Structure

This wiki is organized into the following sections:

### Configuration Files

1. **[WU_Settings.json](SETTINGS_REFERENCE.md#master-settings)** - Master configuration
   - Weather mode selection (Auto vs Manual)
   - Temperature control toggles
   - Logging configuration

2. **[WU_AutoWeather.json](SETTINGS_REFERENCE.md#auto-weather-settings)** - Auto/Random mode
   - Random weather change intervals
   - Random weather chance percentage
   - Weather presets with min/max ranges

3. **[WU_ManualWeather.json](SETTINGS_REFERENCE.md#manual-weather-settings)** - Manual/Schedule mode
   - Time-based weather schedule
   - Weather presets with min/max ranges
   - Chance-based schedule entries

4. **[WU_ZoneTemperatureControl.json](SETTINGS_REFERENCE.md#zone-temperature-settings)** - Temperature zones
   - Location-based temperature overrides
   - Radius and height configurations
   - Per-zone temperature ranges

### Parameter Reference

- **[Complete Settings Reference](SETTINGS_REFERENCE.md)** - Every parameter explained
- **[Quick Reference Tables](SETTINGS_QUICK_REFERENCE.md)** - Fast lookup for all settings
- **[Changelog](CHANGELOG.md)** - Version history and release notes

---

## 🆘 Need Help?

1. **Check the [Installation Guide](INSTALLATION.md)** for setup instructions
2. **Review the [Settings Reference](SETTINGS_REFERENCE.md)** for configuration details
3. **Enable debug logging** in `WU_Settings.json` and check your logs
4. **Open an issue** on GitHub with log files

---

## 📋 Latest Version

**Current:** v2.3.4

**New in v2.3.4:**
- ✅ Manual weather control with 1-second reinforcement system
- ✅ Smooth transitions with manual value calculation
- ✅ Aggressive 0.5-second snowfall reinforcement

**v3.3.3 (2025-11-13):**
- ✅ Centralized `WeatherCheckInterval` to `WU_Settings.json` (single setting for both modes)
- ✅ Simplified configuration (removed duplicate setting from auto/manual configs)
- ✅ Better default (30s instead of 60s for more responsive monitoring)

**v3.3.2:**
- ✅ Fixed timer reset issue (presets now change correctly after duration)
- ✅ Fixed excessive logging spam (rain thresholds, temperature)
- ✅ Fixed rain sounds playing when rain is disabled (winter maps)

**v3.3.1:**
- ✅ Reduced log noise (96% reduction on config hot-reloads)
- ✅ Improved manual mode display (shows next schedule time)
- ✅ Bug fixes and code cleanup

**v3.3 Highlights:**
- ✅ Removed drift correction (performance optimization)
- ✅ Relies on `MissionWeather` API for stability
- ✅ 90% reduction in weather API calls

**v3.2 Highlights:**
- ✅ Removed `WeatherChangeInterval` (eliminated confusing double-timer)
- ✅ Each preset now controls its own duration via `m_MinDuration_Min/Max`
- ✅ Simplified auto weather system

[See Full Changelog](CHANGELOG.md)

---

## 🔗 External Links

- [GitHub Repository](https://github.com/FUBAR88/Weather_Upgrade)
- [Latest Release](https://github.com/FUBAR88/Weather_Upgrade/releases)
- [Report a Bug](https://github.com/FUBAR88/Weather_Upgrade/issues)

---

**Made with ❤️ for the DayZ Server Community**

