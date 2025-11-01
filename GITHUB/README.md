# Weather Upgrade for DayZ

[![Version](https://img.shields.io/badge/version-3.1-blue.svg)](https://github.com/FUBAR88/Weather_Upgrade)
[![DayZ](https://img.shields.io/badge/DayZ-1.25+-green.svg)](https://dayz.com)

> Complete server-side weather control system for DayZ with automatic/manual scheduling, temperature zones, and drift correction.

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
  - Production logs
  - Debug logs for troubleshooting
  - Player temperature tracking

- 🔧 **Admin Friendly**
  - Auto-generates configs
  - JSON-based (easy to edit)
  - Compatible with COT
  - Works on all maps

---

## 📥 Installation

### Quick Setup

1. Download the mod (PBO file from mod author)
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
    "WeatherChangeInterval": 1800,
    "RandomWeatherChance": 30
}
```
30% chance to change weather every 30 minutes

**Manual Mode (Scheduled):**
```json
{
    "AutoWeatherChanges": 0,
    "WeatherSchedule": [
        { "Time": "14:00", "Preset": "rain", "Chance": 100 }
    ]
}
```
Rain at 14:00 every day

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
    "DefaultWeatherPreset": "snowy",
    "RandomWeatherChance": 0
}
```

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

❌ **Not Compatible:**
- Other weather control mods

---

## 🐛 Troubleshooting

### Weather Not Changing
1. Check `AutoWeatherChanges` setting matches intended mode
2. Verify correct config file is configured
3. Check logs in `profiles/Weather_Upgrade/Logs/`

### Weather Drifts
1. Enable debug logging
2. Check drift correction settings
3. Verify no conflicting mods

### COT Shows Wrong Values
Update to v3.1+ (fixes wind function and rain threshold parameters)

**[Full Troubleshooting Guide](INSTALLATION.md#troubleshooting)**

---

## 📝 Changelog

**v3.1 (2025-11-01) - Current**
- ✅ Wind Function parameters (COT compatibility fix)
- ✅ Rain Threshold controls
- ✅ No more hardcoded values

**v3.0 (2025-10-30)**
- ✅ Enhanced logging (INFO + DEBUG)
- ✅ Temperature zone system
- ✅ Configurable drift correction

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

**Compatible with DayZ 1.25+**  
**Made for DayZ Server Owners**
