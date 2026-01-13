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

**Current:** v2.3.6

**New in v2.3.6:**
- ✅ **CRITICAL FIX:** Fire heat now works correctly with temperature override
- ✅ Players can now receive heat buffs from fires even with extreme cold overrides (-30°C)
- ✅ Temperature override now correctly applies when no fire heat is present
- ✅ Fixed players dying of cold when temperature override was active

**v2.3.5:**
- ✅ Smooth rain transitions (rain no longer stops/resets during transitions)
- ✅ Rain reinforcement system (every 1.0s with immediate application) prevents DayZ override
- ✅ Rain thresholds set wide during transitions for smooth appearance
- ✅ Fixed rain values "spinning" in admin tools (now stable like overcast/fog)
- ✅ Smooth transitions with 1-second value updates (no snapping or glitches)
- ✅ Enhanced transition logging (every 10% progress + debug logs every second)
- ✅ Fixed transition values not applying in Manual mode
- ✅ Smooth completion at 99%+ to prevent snapping
- ✅ Removed `DefaultWeatherPreset` from Manual mode (schedule handles preset selection)
- ✅ Fixed log spam (final values log now appears once per transition)

**Previous (v2.3.4):**
- ✅ Manual weather control with 1-second reinforcement system
- ✅ Aggressive 0.5-second snowfall reinforcement
- ✅ Centralized `WeatherCheckInterval` to `WU_Settings.json`
- ✅ Fixed timer reset issue (presets now change correctly after duration)
- ✅ Fixed excessive logging spam (rain thresholds, temperature)
- ✅ Fixed rain sounds playing when rain is disabled (winter maps)
- ✅ Reduced log noise (96% reduction on hot-reloads)
- ✅ Improved manual mode display (shows next schedule time)
- ✅ Removed drift correction (performance optimization)
- ✅ Relies on `MissionWeather` API for stability
- ✅ 90% reduction in weather API calls
- ✅ Removed `WeatherChangeInterval` (simplified auto mode)
- ✅ Wind Function parameters (COT compatibility fix)
- ✅ Rain Threshold controls
- ✅ Enhanced logging (INFO + DEBUG)
- ✅ Temperature zone system
- ✅ DisableWeatherUpgrade setting for AdminTools compatibility

[See Full Changelog](CHANGELOG.md)

---

## 🔗 External Links

- [GitHub Repository](https://github.com/FUBAR88/Weather_Upgrade)
- [Latest Release](https://github.com/FUBAR88/Weather_Upgrade/releases)
- [Report a Bug](https://github.com/FUBAR88/Weather_Upgrade/issues)

---

**Made with ❤️ for the DayZ Server Community**

