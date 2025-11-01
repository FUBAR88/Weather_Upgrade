# Weather Upgrade Wiki - Home

Welcome to the **Weather Upgrade** mod documentation! This wiki contains everything you need to configure and use the mod on your DayZ server.

---

## 📚 Quick Navigation

### Getting Started
- **[Installation Guide](#installation)** - How to install the mod on your server
- **[Quick Start Examples](#quick-start)** - Common configurations to get started fast

### Configuration
- **[Master Settings (WU_Settings.json)](#master-settings)** - Main configuration file
- **[Auto Weather Mode](#auto-weather-mode)** - Random weather changes
- **[Manual Weather Mode](#manual-weather-mode)** - Scheduled weather by time
- **[Temperature Zones](#temperature-zones)** - Location-based temperature control

### Reference
- **[All Weather Parameters](#weather-parameters)** - Complete list of preset options
- **[Wind Settings](#wind-settings)** - Wind speed, direction, and function parameters
- **[Rain & Snow Thresholds](#thresholds)** - Control when precipitation appears
- **[Volumetric Fog](#volumetric-fog)** - Realistic 3D fog configuration
- **[Temperature Control](#temperature-control)** - Global and zone-based temperatures

### Advanced
- **[Drift Correction System](#drift-correction)** - How the mod maintains your settings
- **[Logging System](#logging)** - Normal and debug logging explained
- **[COT Compatibility](#cot-compatibility)** - Using with Community Online Tools

### Help
- **[Troubleshooting](#troubleshooting)** - Common issues and solutions
- **[FAQ](#faq)** - Frequently asked questions

---

## 🎯 What is Weather Upgrade?

Weather Upgrade is a comprehensive server-side mod that gives you complete control over DayZ weather. Unlike vanilla DayZ which uses random weather from `cfgweather.xml`, this mod lets you:

- **Schedule weather** by in-game time (e.g., rain every day at 14:00)
- **Randomize weather** with configurable intervals and chances
- **Override temperatures** globally or in specific map locations
- **Control all weather parameters** including fog, wind, snow, rain, storms
- **Monitor weather** via COT's in-game weather panel
- **Prevent drift** - automatically corrects when DayZ tries to override your settings

---

## 📖 Documentation Structure

This wiki is organized into the following sections:

### Configuration Files

1. **[WU_Settings.json](WU-Settings)** - Master configuration
   - Weather mode selection (Auto vs Manual)
   - Temperature control toggles
   - Drift correction settings
   - Logging configuration

2. **[WU_AutoWeather.json](WU-AutoWeather)** - Auto/Random mode
   - Random weather change intervals
   - Random weather chance percentage
   - Weather presets with min/max ranges

3. **[WU_ManualWeather.json](WU-ManualWeather)** - Manual/Schedule mode
   - Time-based weather schedule
   - Weather presets with min/max ranges
   - Chance-based schedule entries

4. **[WU_ZoneTemperatureControl.json](WU-ZoneTemperatureControl)** - Temperature zones
   - Location-based temperature overrides
   - Radius and height configurations
   - Per-zone temperature ranges

### Parameter Reference

- **[Complete Settings Reference](Complete-Settings-Reference)** - Every parameter explained
- **[Wind Parameters](Wind-Parameters)** - Wind speed, direction, and function
- **[Threshold Parameters](Threshold-Parameters)** - Rain and snow thresholds
- **[Volumetric Fog Parameters](Volumetric-Fog)** - 3D fog configuration
- **[Temperature Parameters](Temperature-Parameters)** - Global and zone temperatures

### Guides

- **[Creating Custom Presets](Creating-Custom-Presets)** - Step-by-step preset creation
- **[Setting Up Temperature Zones](Setting-Up-Temperature-Zones)** - Zone configuration guide
- **[Understanding Drift Correction](Understanding-Drift-Correction)** - How drift works
- **[Using Debug Logging](Using-Debug-Logging)** - Troubleshooting with logs

---

## 🆘 Need Help?

1. **Check the [Troubleshooting](Troubleshooting) page** for common issues
2. **Review the [FAQ](FAQ)** for quick answers
3. **Enable debug logging** and check your logs
4. **Open an issue** on GitHub with log files

---

## 📋 Latest Version

**Current:** v3.1 (2025-11-01)

**New in v3.1:**
- ✅ Wind Function parameters (fixes COT compatibility)
- ✅ Rain Threshold controls
- ✅ Configurable wind variation per preset
- ✅ No more hardcoded values

[See Full Changelog](Changelog)

---

## 🔗 External Links

- [GitHub Repository](https://github.com/yourusername/Weather_Upgrade)
- [Latest Release](https://github.com/yourusername/Weather_Upgrade/releases)
- [Report a Bug](https://github.com/yourusername/Weather_Upgrade/issues)
- [Request a Feature](https://github.com/yourusername/Weather_Upgrade/discussions)

---

**Made with ❤️ for the DayZ Server Community**

