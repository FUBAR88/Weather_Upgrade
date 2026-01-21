# Weather Upgrade for DayZ

[![Version](https://img.shields.io/badge/version-2.3.8-blue.svg)](https://github.com/FUBAR88/Weather_Upgrade)
[![DayZ](https://img.shields.io/badge/DayZ-1.25+-green.svg)](https://dayz.com)

> Complete server-side weather control system for DayZ with automatic/manual scheduling and temperature zones. Performance-optimized with MissionWeather API.

---

## 📋 Features

- 🌦️ **Two Weather Modes**
  - Auto Mode: Random weather changes at intervals
  - Manual Mode: Time-based scheduled weather

- 🌡️ **Temperature Control**
  - Global temperature override per preset
  - Location-based temperature zones
  - Height-based zones for bunkers/mountains

- ⚡ **Advanced Weather**
  - Volumetric fog (realistic 3D fog)
  - Wind function control (natural variation)
  - Rain/Snow threshold configuration
  - Thunderstorm and lightning
  - Snowflake size customization

- 📊 **Logging System**
  - Production logs (minimal noise, essential info only)
  - Debug logs for troubleshooting (when enabled)
  - Reduced log spam on config hot-reloads
  - Player temperature tracking

- 🔧 **Admin Friendly**
  - Auto-generates configs
  - JSON-based (easy to edit)
  - Compatible with COT
  - Works on all maps

---

## 📥 Installation

### Quick Setup

1. Download the mod (PBO file from FUBAR)
2. Copy `@Weather_Upgrade` folder to your server
3. Add `-mod=@Weather_Upgrade` to server start parameters
4. Start server - configs auto-generate in `profiles/Weather_Upgrade/`
5. Edit JSON configs to your preference
6. Restart server

**[Detailed Installation Guide](INSTALLATION.md)**

---

## ⚙️ Configuration

### Weather Modes

**Auto Mode (Random):**
```json
{
    "AutoWeatherChanges": 1,
    "DefaultWeatherPreset": "clear"
}
```
Each preset defines its own duration and `m_WeatherChance` weight. After duration expires, weather always changes to a new preset selected by weighted random based on `m_WeatherChance` values.

**Manual Mode (Scheduled):**
```json
{
    "WeatherPresets": {
        "rain": { ... preset parameters ... }
    },
    "WeatherSchedule": [
        { "Time": "14:00", "Preset": "rain", "Chance": 100 }
    ]
}
```
Rain at 14:00 every day (100% chance = always applies). Presets must be defined in `WeatherPresets`.

**With Randomness:**
```json
{
    "WeatherSchedule": [
        { "Time": "14:00", "Preset": "rain", "Chance": 70 }
    ]
}
```
70% chance of rain at 14:00, 30% chance to skip (keeps previous weather)

---

## 📖 Documentation

- **[Quick Settings Reference](SETTINGS_QUICK_REFERENCE.md)** - Fast lookup tables
- **[Complete Settings Guide](SETTINGS_REFERENCE.md)** - All parameters explained
- **[Installation Guide](INSTALLATION.md)** - Step-by-step setup
- **[Changelog](CHANGELOG.md)** - Version history

---

## 🎯 Quick Examples

### Always Snowy

**WU_Settings.json:**
```json
{ "AutoWeatherChanges": 1 }
```

**WU_AutoWeather.json:**
```json
{
    "DefaultWeatherPreset": "snowy"
}
```
Set all other presets' `m_WeatherChance` to `0` to prevent them from being selected.

### Hot Zone

**WU_Settings.json:**
```json
{ "EnableZoneTemperatureControl": 1 }
```

**WU_ZoneTemperatureControl.json:**
```json
{
    "Zones": [{
        "Name": "Bunker",
        "Position": [7291, 27, 2046],
        "Radius": 1200,
        "Height": 250,
        "Temperature_Min": 40,
        "Temperature_Max": 50
    }]
}
```

### Ground Fog

**Preset:**
```json
"foggy": {
    "m_UseDynVolFog": 1,
    "m_VolFogDistanceDensity_Min": 0.6,
    "m_VolFogHeightDensity_Min": 0.5,
    "m_VolFogHeightBias": -30
}
```

---

## 🔗 Compatibility

✅ **Compatible:**
- All DayZ maps (Chernarus, Livonia, Namalsk, DeerIsle, custom maps)
- COT (Community Online Tools)
- Most gameplay mods
- AdminTools (when `DisableWeatherUpgrade: 1` is set)

⚠️ **Compatibility Mode:**
- **AdminTools Weather Control:** Set `DisableWeatherUpgrade: 1` in `WU_Settings.json` to allow AdminTools to control weather
  - Weather_Upgrade enters "monitoring mode" - logs weather but doesn't enforce presets
  - AdminTools weather commands will work normally
  - Useful for servers that prefer AdminTools weather commands

❌ **Not Compatible:**
- Other weather control mods (when Weather_Upgrade is enabled)
  - Only one weather control mod should be active at a time
  - Use `DisableWeatherUpgrade: 1` if you need another mod to control weather

---

## 🐛 Troubleshooting

### Weather Not Changing
1. Check `AutoWeatherChanges` setting matches intended mode
2. Verify correct config file is configured
3. Check logs in `profiles/Weather_Upgrade/Logs/`

### Weather Not Stable
1. Verify no conflicting weather mods
2. Check logs for errors
3. Ensure `MissionWeather` is active (check startup logs)

### COT Shows Wrong Values
Update to v2.3.8+ (fixes fire heat with temperature override and smooth transitions)

### Want to Use AdminTools Weather Instead?
Set `DisableWeatherUpgrade: 1` in `WU_Settings.json` to allow AdminTools to control weather while Weather_Upgrade monitors and logs changes.

**[Full Troubleshooting Guide](INSTALLATION.md#troubleshooting)**

---

## 📝 Changelog

**v2.3.8 (Current) - Latest**
- ✅ **MAJOR IMPROVEMENT:** Smooth snowfall transitions (snowfall no longer stops/resets during transitions)
- ✅ Snowfall reinforcement system (every 0.5s with immediate application) prevents DayZ override
- ✅ Snowfall thresholds set wide during transitions for smooth appearance
- ✅ Fixed snowfall values "spinning" in admin tools (now stable like rain/overcast/fog)
- ✅ All weather parameters now transition smoothly without snapping or glitches
- ✅ Snowfall transitions now match rain transition behavior exactly

**v2.3.6**
- ✅ **CRITICAL FIX:** Fire heat now works correctly with temperature override
- ✅ Players can now receive heat buffs from fires even with extreme cold overrides (-30°C)
- ✅ Temperature override now correctly applies when no fire heat is present
- ✅ Fixed players dying of cold when temperature override was active

**v2.3.5**
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

**v2.3.4**
- ✅ Manual weather control with 1-second reinforcement system
- ✅ Aggressive 0.5-second snowfall reinforcement
- ✅ Complete server-side weather control
- ✅ Centralized `WeatherCheckInterval` to `WU_Settings.json` (single setting for both modes)
- ✅ Fixed timer reset issue (presets now change correctly after duration)
- ✅ Fixed excessive logging spam (rain thresholds, temperature)
- ✅ Fixed rain sounds playing when rain is disabled (winter maps)
- ✅ Reduced log noise (96% reduction on hot-reloads)
- ✅ Improved manual mode display (shows next schedule time)
- ✅ Removed drift correction (performance optimization)
- ✅ Relies on `MissionWeather` API for stability
- ✅ 90% reduction in weather API calls
- ✅ Removed `WeatherChangeInterval` (simplified auto mode)
- ✅ Per-preset duration control only
- ✅ Wind Function parameters (COT compatibility fix)
- ✅ Rain Threshold controls
- ✅ Enhanced logging (INFO + DEBUG)
- ✅ Temperature zone system
- ✅ DisableWeatherUpgrade setting for AdminTools compatibility

**v2.0 (2025-10-24)**
- ✅ Volumetric fog support
- ✅ Snowflake scale control

**[Full Changelog](CHANGELOG.md)**

---

## 💬 Support

**Need Help?**
1. Enable debug logging
2. Check `profiles/Weather_Upgrade/Logs/`
3. Verify JSON syntax
4. Open GitHub issue with log files

**Questions?**
- Check [Settings Reference](SETTINGS_REFERENCE.md)
- Check [Installation Guide](INSTALLATION.md)
- Open a GitHub Discussion

---

## 📜 License

MIT License - See [LICENSE](LICENSE) file

---

## 👤 Author

**FUBAR** - Weather Upgrade Mod for DayZ

GitHub: [FUBAR88](https://github.com/FUBAR88)

---

**Compatible with DayZ 1.25+**  
**Made for DayZ Server Owners**
