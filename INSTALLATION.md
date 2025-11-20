# Installation Guide

Complete installation instructions for Weather Upgrade mod.

---

## 📋 Requirements

- **DayZ Server** version 1.25 or higher
- **Server access** to modify start parameters and configuration files
- **Text editor** (Notepad++, VS Code, or similar) for editing JSON files

---

## 📥 Installation Steps

### Step 1: Download the Mod

1. Go to [Releases](https://github.com/FUBAR88/Weather_Upgrade/releases)
2. Download the latest `Weather_Upgrade_v2.3.4.zip`
3. Extract the archive

### Step 2: Install on Server

1. **Copy** the `@Weather_Upgrade` folder to your DayZ server root:
   ```
   DayZServer/
   ├── @Weather_Upgrade/          ← New folder
   │   └── Addons/
   │       └── Weather_Upgrade.pbo
   ├── DayZServer_x64.exe
   └── ...
   ```

2. **Edit** your server start parameters (batch file or command line):

   **Before:**
   ```bat
   start DayZServer_x64.exe -config=serverDZ.cfg
   ```

   **After:**
   ```bat
   start DayZServer_x64.exe -config=serverDZ.cfg -mod=@Weather_Upgrade
   ```

   If you have other mods:
   ```bat
   -mod=@CF;@VPPAdminTools;@Weather_Upgrade
   ```

3. **Start** the server

### Step 3: Verify Installation

After server starts, check for auto-generated config files:

```
DayZServer/profiles/Weather_Upgrade/
├── WU_Settings.json                  ✅
├── WU_AutoWeather.json               ✅
├── WU_ManualWeather.json             ✅
└── WU_ZoneTemperatureControl.json    ✅
```

If these files exist, installation was successful!

### Step 4: Check Logs

Navigate to:
```
DayZServer/profiles/Weather_Upgrade/Logs/
└── Weather_Upgrade_YYYY_MM_DD_HH_MM_SS.log
```

Look for these lines:
```
[INFO] [WEATHER UPGRADE] New log file created
[INFO] [MISSION SERVER] Weather control taken - cfgweather.xml overridden
[INFO] [WEATHER MANAGER] Configuration loaded successfully
[INFO] [WEATHER MANAGER] Weather Upgrade mod initialized successfully
```

---

## ⚙️ Initial Configuration

After installation, configure the mod:

### Choose Weather Mode

Edit `profiles/Weather_Upgrade/WU_Settings.json`:

**For Auto/Random Weather:**
```json
{
    "AutoWeatherChanges": 1
}
```

**For Manual/Scheduled Weather:**
```json
{
    "AutoWeatherChanges": 0
}
```

### Configure Your Mode

**If Auto Mode (AutoWeatherChanges: 1):**
1. Edit `WU_AutoWeather.json`
2. Set `DefaultWeatherPreset` (e.g., "snowy", "clear")
3. Adjust `RandomWeatherChance` (0-100%)
4. Customize presets

**If Manual Mode (AutoWeatherChanges: 0):**
1. Edit `WU_ManualWeather.json`
2. Set up `WeatherSchedule` with times and presets
3. Customize presets

### Restart Server

Stop and start your server to apply changes.

---

## ✅ Verification Checklist

After installation and configuration:

- [ ] Server starts without errors
- [ ] Config files exist in `profiles/Weather_Upgrade/`
- [ ] Log file created in `profiles/Weather_Upgrade/Logs/`
- [ ] Log shows "Weather Upgrade mod initialized successfully"
- [ ] Weather applies as configured (check in-game or with COT)

---

## 🔄 Updating

### From v3.0 to v3.1

**Option 1: Auto-Upgrade (Recommended)**
1. Delete all JSON files in `profiles/Weather_Upgrade/`
2. Replace `@Weather_Upgrade` folder with new version
3. Restart server
4. New configs generate with v3.1 fields

**Option 2: Manual Upgrade**
1. Replace `@Weather_Upgrade` folder with new version
2. Add 6 new fields to each preset in your existing JSONs:
   ```json
   "m_WindFunc_Min": 0.1,
   "m_WindFunc_Max": 1.0,
   "m_WindFunc_Speed": 20.0,
   "m_RainThreshold_Min": 0.3,
   "m_RainThreshold_Max": 1.0,
   "m_RainThreshold_Timeout": 30,
   ```
3. Restart server

---

## 🗑️ Uninstallation

1. **Remove** from server start parameters:
   ```bat
   -mod=@Weather_Upgrade
   ```

2. **Delete** the mod folder:
   ```
   DayZServer/@Weather_Upgrade/
   ```

3. **Optionally delete** config files:
   ```
   DayZServer/profiles/Weather_Upgrade/
   ```

4. **Restart** server

---

## 🐛 Troubleshooting Installation

### Config Files Not Generated

**Cause:** Mod not loading or permission issues

**Solution:**
1. Verify `-mod=@Weather_Upgrade` in start parameters
2. Check server console for errors
3. Verify folder structure matches exactly
4. Check file permissions on `profiles/` folder

### Server Won't Start

**Cause:** Mod conflict or corrupt PBO

**Solution:**
1. Check server logs for error messages
2. Try starting server without other mods (isolate conflict)
3. Re-download and replace `Weather_Upgrade.pbo`
4. Verify PBO integrity

### Weather Not Changing

**Cause:** Configuration issue

**Solution:**
1. Check `AutoWeatherChanges` setting matches your intended mode
2. Verify correct JSON file is configured (Auto vs Manual)
3. Enable debug logging to see weather operations
4. Check logs for errors

---

## 📞 Need Help?

- [Open an Issue](https://github.com/yourusername/Weather_Upgrade/issues)
- [Check FAQ](FAQ.md)
- [Join Discussions](https://github.com/yourusername/Weather_Upgrade/discussions)

---

**Next:** [Configuration Guide](Configuration-Guide.md) | [Settings Reference](Settings-Reference.md)

