# Settings Quick Reference

Fast lookup table for all Weather Upgrade settings.

---

## 🎛️ Master Settings (WU_Settings.json)

| Setting | Values | Default | What It Does |
|---------|--------|---------|--------------|
| `EnableLogging` | 0/1 | 1 | Create log files |
| `EnableDebugLogging` | 0/1 | 0 | Create detailed debug logs (troubleshooting only) |
| `KeepLogsDays` | 1-365 | 30 | Auto-delete logs older than X days |
| `AutoWeatherChanges` | 0/1 | 0 | **0**=Manual/Schedule, **1**=Auto/Random |
| `EnableTemperatureControl` | 0/1 | 1 | Allow presets to override temperature |
| `EnableZoneTemperatureControl` | 0/1 | 0 | Enable location-based temperature zones |
| `WeatherCheckInterval` | seconds | 30 | How often to check/log weather status (both modes) |

---

## 🌦️ Auto Mode (WU_AutoWeather.json)

| Setting | Range | Default | What It Does |
|---------|-------|---------|--------------|
| `DefaultWeatherPreset` | string | "clear" | Starting preset (or `""` for random) |
| `RandomWeatherChance` | 0-100% | 30 | % chance to change when preset duration expires |

**How It Works:**
- Each preset controls its own duration via `m_MinDuration_Min/Max`
- Example: `clear` preset (600-900s) → rolled 750s → after 750s, 30% chance to switch presets
- `0%` = Never change (stays on preset), `100%` = Always change (deterministic rotation)

---

## 📅 Manual Mode (WU_ManualWeather.json)

| Setting | Type | What It Does |
|---------|------|--------------|
| `DefaultWeatherPreset` | string | Fallback when no schedule matches |
| `WeatherSchedule` | array | Time-based weather entries |

**Schedule Entry:**
```json
{
    "Time": "14:00",
    "Preset": "rain",
    "Chance": 70
}
```
- `Time`: Game time (24-hour format)
- `Preset`: Preset name to apply
- `Chance`: 0-100% chance (allows randomness)

---

## 🌡️ Temperature Zones (WU_ZoneTemperatureControl.json)

| Setting | Type | Example | What It Does |
|---------|------|---------|--------------|
| `Name` | string | "Bunker" | Zone name for logs |
| `Position` | [X,Y,Z] | [7291, 27, 2046] | Center coordinates |
| `Radius` | meters | 1200 | Horizontal radius |
| `Height` | meters | 250 or -1 | Vertical height (-1=disabled) |
| `Temperature_Min` | °C | -15 | Minimum temperature |
| `Temperature_Max` | °C | -5 | Maximum temperature |

---

## 🌤️ Weather Preset Parameters

### Timing

| Parameter | Range | Typical | What It Does |
|-----------|-------|---------|--------------|
| `m_TransitionTime_Min/Max` | seconds | 120-240 | How long weather change takes (2-4 min) |
| `m_MinDuration_Min/Max` | seconds | 600-1200 | How long weather stays stable (10-20 min) |

### Weather Effects (0.0 - 1.0)

| Parameter | Range | Typical | Visual Effect |
|-----------|-------|---------|---------------|
| `m_Overcast_Min/Max` | 0.0-1.0 | varies | 0=clear, 0.5=cloudy, 1=overcast |
| `m_Fog_Min/Max` | 0.0-1.0 | 0 | Traditional fog (use vol fog instead) |
| `m_Rain_Min/Max` | 0.0-1.0 | 0-0.8 | 0=none, 0.5=moderate, 1=heavy |
| `m_Snowfall_Min/Max` | 0.0-1.0 | 0-1 | 0=none, 0.5=moderate, 1=blizzard |

### Wind (m/s)

| Parameter | Range | Typical | Description |
|-----------|-------|---------|-------------|
| `m_WindSpeed_Min/Max` | 0-20 m/s | 2-6 | Wind speed |
| `m_WindDirection_Min/Max` | 0-360° | 0-360 | Wind direction (0=N, 90=E, 180=S, 270=W) |
| `m_WindFunc_Min` | 0.0-1.0 | 0.1 | Wind variation minimum multiplier |
| `m_WindFunc_Max` | 0.0-1.0 | 1.0 | Wind variation maximum multiplier |
| `m_WindFunc_Speed` | 0-100 | 20.0 | Wind function frequency |

**Wind Speed Guide:**
- `0.5-2.0` = Light breeze
- `2-6` = Moderate wind  
- `8-15` = Strong wind (storms)
- `15+` = Extreme wind

### Thresholds

| Parameter | Range | Default | What It Does |
|-----------|-------|---------|--------------|
| `m_RainThreshold_Min` | 0.0-1.0 | 0.3 | Minimum overcast for rain to render |
| `m_RainThreshold_Max` | 0.0-1.0 | 1.0 | Maximum overcast for rain |
| `m_RainThreshold_Timeout` | seconds | 30 | Rain threshold transition time |
| `m_SnowfallThreshold_Min` | 0.0-1.0 | 0.6 | Minimum overcast for snow to render |
| `m_SnowfallThreshold_Max` | 0.0-1.0 | 1.0 | Maximum overcast for snow |
| `m_SnowfallThreshold_Timeout` | seconds | 30 | Snow threshold transition time |

### Volumetric Fog

| Parameter | Range | Description |
|-----------|-------|-------------|
| `m_UseDynVolFog` | 0/1 | Enable 3D volumetric fog |
| `m_VolFogDistanceDensity_Min/Max` | 0.0-1.0 | Horizontal fog (0.2-0.4=light, 0.6-0.9=dense) |
| `m_VolFogHeightDensity_Min/Max` | 0.0-1.0 | Vertical fog thickness |
| `m_VolFogHeightBias` | -200 to 200 | Height offset (negative=ground, positive=mountain) |
| `m_FogTransitionTime` | seconds | Fog transition time (default: 30) |

### Snow

| Parameter | Range | Description |
|-----------|-------|-------------|
| `m_UseSnowflakeScale` | 0/1 | Enable custom snowflake size |
| `m_SnowflakeScale_Min/Max` | 0.5-3.0 | Size multiplier (1.0=normal, 1.5=larger) |

### Storms

| Parameter | Range | Description |
|-----------|-------|-------------|
| `m_ThunderstormDensity` | 0.0-1.0 | Lightning frequency (0=none, 0.7=frequent, 1=constant) |
| `m_ThunderstormThreshold` | 0.0-1.0 | Minimum overcast for lightning (default: 0.7-0.85) |
| `m_LightningTimeout` | seconds | Minimum time between strikes |

### Temperature

| Parameter | Range | Description |
|-----------|-------|-------------|
| `m_UseEnvironmentTemperature` | 0/1 | Override temperature for this preset |
| `m_EnvironmentTemperature` | -50 to 50°C | Temperature value (realistic: -20 to 40) |

---

## 🎯 Value Recommendations

### For Always Snowy
```json
"m_Overcast_Min": 0.65,
"m_Snowfall_Min": 0.6,
"m_SnowfallThreshold_Min": 0.5,
"m_UseEnvironmentTemperature": 1,
"m_EnvironmentTemperature": -6
```

### For Heavy Rain
```json
"m_Overcast_Min": 0.85,
"m_Rain_Min": 0.7,
"m_RainThreshold_Min": 0.3,
"m_WindSpeed_Min": 5
```

### For Clear Weather
```json
"m_Overcast_Max": 0.2,
"m_Rain_Max": 0,
"m_Snowfall_Max": 0,
"m_WindSpeed_Min": 0.5,
"m_WindSpeed_Max": 2
```

### For Ground Fog
```json
"m_UseDynVolFog": 1,
"m_VolFogDistanceDensity_Min": 0.6,
"m_VolFogHeightDensity_Min": 0.5,
"m_VolFogHeightBias": -30,
"m_WindSpeed_Max": 1.5
```

---

## 🔍 Quick Lookup by Feature

### "I want snow to always render"
```json
"m_Snowfall_Min": 0.6,
"m_Overcast_Min": 0.6,
"m_SnowfallThreshold_Min": 0.5
```
**Why:** Snow requires overcast ≥ threshold. Match or exceed threshold with overcast.

### "I want fixed wind direction"
```json
"m_WindDirection_Min": 90,
"m_WindDirection_Max": 90
```
**Why:** Same min/max = no randomization. 90° = wind from East.

### "I want calm wind with no variation"
```json
"m_WindSpeed_Min": 1,
"m_WindSpeed_Max": 1,
"m_WindFunc_Min": 0.0,
"m_WindFunc_Max": 0.0,
"m_WindFunc_Speed": 1.0
```
**Why:** Fixed speed + zero variation = completely calm wind.

### "I want ground-level fog"
```json
"m_UseDynVolFog": 1,
"m_VolFogHeightBias": -50,
"m_VolFogHeightDensity_Min": 0.6
```
**Why:** Negative bias pushes fog down, height density makes it thick vertically.

### "I want hot desert zone"
```json
{
    "Name": "Desert",
    "Radius": 2000,
    "Height": -1,
    "Temperature_Min": 35,
    "Temperature_Max": 45
}
```
**Why:** `Height: -1` disables vertical check (works on uneven terrain).

---

## 📖 Full Documentation

For complete parameter explanations, see:
- [Complete Settings Reference](SETTINGS_REFERENCE.md)
- [Installation Guide](INSTALLATION.md)
- [Changelog](CHANGELOG.md)
- [README](README.md)

