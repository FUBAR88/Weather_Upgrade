# Weather_Upgrade Mod - Changelog

## Version 2.3.4 (Current)

### Major Changes
- **Manual Weather Control System**
  - Complete manual control over all weather parameters during transitions
  - 1-second reinforcement system ensures precise weather values
  - Manual value calculation during transitions for smooth changes
  - Aggressive 0.5-second snowfall reinforcement to combat rapid decay

### Features
- **Transition Control**
  - Manual intermediate value calculation during preset transitions
  - Smooth transitions with calculated progress ratios
  - All weather parameters transitioned manually (overcast, fog, rain, snowfall, wind, volumetric fog, etc.)
  
- **Reinforcement System**
  - 1-second reinforcement loop applies all weather values with optimized transitions
  - 0.5-second aggressive snowfall reinforcement (0.0s transition) for stable snowfall
  - TIGHT_TOLERANCE (0.005) ensures values stay within acceptable range
  
- **Performance**
  - Optimized transition times (0.5s for most values, 0.0s for snowfall)
  - Efficient 1-second update cycle
  - Minimal CPU overhead

### Technical Changes
- Manual transition calculation replaces DayZ's automatic transitions
- Removed reliance on DayZ's Set() API with transition time for transitions
- Added manual intermediate value calculation in OnUpdate()
- Comprehensive reinforcement of all weather settings every second
- Dedicated snowfall reinforcement every 0.5 seconds

### Migration Guide
- **No action required** - fully backward compatible
- Existing configs work without changes
- System automatically uses manual control for all weather parameters

---

## Version 3.3.3 (2025-11-13)

### Configuration Changes
- **Centralized `WeatherCheckInterval` Setting**
  - Moved `WeatherCheckInterval` from `WU_AutoWeather.json` and `WU_ManualWeather.json` to `WU_Settings.json`
  - Single setting now controls logging/checking frequency for both Auto and Manual modes
  - Default value changed from 60s to 30s (more responsive monitoring)
  - Simplifies configuration - one place to control weather status logging and drift checks

### Benefits
- **Simplified Configuration:** One setting instead of two (less confusion for server owners)
- **Consistent Behavior:** Same logging frequency regardless of weather mode
- **Better Defaults:** 30s interval provides more responsive monitoring out of the box

### Migration Guide
- **Action Required:** 
  - Remove `"WeatherCheckInterval"` from `WU_AutoWeather.json` (if present)
  - Remove `"WeatherCheckInterval"` from `WU_ManualWeather.json` (if present)
  - Add `"WeatherCheckInterval": 30` to `WU_Settings.json` (or let mod regenerate defaults)
- **Backward Compatible:** Old configs still work (extra fields ignored), but recommended to update

### Technical Changes
- Added `WeatherCheckInterval` field to `WU_Settings` class
- Updated `WU_ConfigManager.GetWeatherCheckInterval()` to read from Settings only
- Removed `WeatherCheckInterval` from `WU_ConfigAuto` and `WU_ConfigManual` classes
- Updated default config generation to exclude `WeatherCheckInterval` from auto/manual configs

---

## Version 3.3.2 (2025-11-12)

### Bug Fixes
- **Fixed Timer Reset Issue**
  - Created `ReapplyCurrentWeatherValues()` method for drift correction that doesn't reset preset timer
  - Duration countdown now works correctly (presets change after full duration expires)
  - Previously, drift correction was resetting `m_PresetApplyTime`, preventing preset changes
  - Random weather changes now trigger correctly after preset duration completes

- **Fixed Excessive Logging**
  - Removed rain threshold log spam from drift correction (was logging every 5 seconds)
  - Fixed temperature override log throttling (was logging every 60ms instead of 60 seconds)
  - Rain threshold logs now only appear during initial preset application
  - Temperature logs now properly throttle to once per 60 seconds
  - Significantly cleaner log output during normal operation

- **Rain Sound Prevention**
  - When rain is set to 0, rain thresholds are automatically set to 1.0-1.0 (disabled)
  - Prevents DayZ from playing rain sounds when no visual rain is present
  - Fixes issue where players heard rain sounds on winter maps with rain disabled

### Technical Changes
- Added `ReapplyCurrentWeatherValues()` method that reapplies weather values without resetting timer
- Updated drift correction to use new method instead of `ApplyWeatherConfig()`
- Fixed `GetGame().GetTime()` throttling (returns milliseconds, not seconds)
- Rain threshold logic now checks `config.m_Rain <= 0.0` before applying thresholds

### Migration Guide
- **No action required** - fully backward compatible
- Existing configs work without changes
- Log improvements are automatic
- Rain threshold fix works automatically for all presets with rain=0

---

## Version 3.3.1 (2025-11-04)

### Improvements
- **Reduced Log Noise**
  - Hot-reload operations now use minimal DEBUG-level logging (1-2 lines instead of 26+)
  - Initial config load still shows full verbose logging for clarity
  - 96% reduction in log spam during runtime config reloads
  - Hot-reload messages moved to DEBUG level (only visible with `EnableDebugLogging: 1`)

- **Manual Mode Display Enhancement**
  - Status logs now show "Next schedule at XX:XX" instead of misleading duration countdown
  - Removed confusing negative duration values in manual/scheduled mode
  - Auto mode still shows duration countdown as before
  - More intuitive display for time-based weather scheduling

### Bug Fixes
- Fixed duplicate variable declaration error in weather status logging
- Fixed compilation error caused by `wu_elapsed` being declared multiple times

### Technical Changes
- Added `verboseLogging` parameter to config loading methods
- Added `GetNextScheduleTimeString()` helper function for manual mode
- Improved log output clarity between initial load and hot-reload operations

### Migration Guide
- **No action required** - fully backward compatible
- Existing configs work without changes
- Log improvements are automatic

---

## Version 3.3 (2025-11-03)

### Breaking Changes
- **Removed drift correction system** for performance optimization
  - Eliminates 90% of weather API calls during steady-state operation
  - Relies on `MissionWeather(true)` to prevent DayZ's native weather from overriding values
  - No more periodic drift checks or corrections

### Why This Change?
**User Feedback:** Players reported performance issues from constant weather corrections

**Technical Analysis:**
- DayZ's `WeatherPhenomenon.OnBeforeChange()` respects `MissionWeather(true)` flag
- When set, native weather system doesn't compute new forecasts
- Our drift correction was fighting a battle we'd already won
- Constant `.GetActual()` calls and `.Set()` corrections were unnecessary overhead

### How It Works Now
1. Mod calls `weather.MissionWeather(true)` on startup
2. DayZ's native weather system is blocked from making changes
3. Weather values stay locked to your preset configuration
4. No periodic checks or corrections needed

### Migration Guide
- **Action Required:** Remove these lines from `WU_Settings.json`:
  - `"DriftCorrectionTime"`
  - `"DriftCorrectionCooldown"`
  - `"DriftTolerance"`
- Or delete config and let mod regenerate defaults on next server start

### Performance Impact
- **Before:** 9 weather API calls every 60 seconds + corrections
- **After:** 0 API calls during steady state
- **Result:** Smoother server performance, especially with many players

### Compatibility
- No changes to preset definitions
- No changes to auto/manual weather modes
- Existing configs work (extra fields ignored)

---

## Version 3.2 (2025-11-02)

### Breaking Changes
- **Removed `WeatherChangeInterval` setting** from Auto Mode
  - Auto weather now uses per-preset `m_MinDuration_Min/Max` exclusively
  - Eliminates confusing double-timer system
  - Each preset now fully controls its own duration

### How Auto Mode Works Now
1. Server starts → Applies initial preset (e.g., `clear`)
2. Preset rolls a random duration from `m_MinDuration_Min/Max` (e.g., 750 seconds)
3. After duration expires → Rolls `RandomWeatherChance` (e.g., 30%)
4. If roll succeeds → Selects new random preset
5. If roll fails → Current preset continues with new random duration

### Migration Guide
- **Action Required:** Remove `"WeatherChangeInterval"` line from `WU_AutoWeather.json`
- Or delete config and let mod regenerate defaults on next server start
- No changes to preset definitions required

### Benefits
- **Clarity:** Single duration control per preset (no conflicting timers)
- **Flexibility:** Different presets can have vastly different durations
  - Example: `clear` (10-20 min) vs `rain` (5-5 min)
- **Predictability:** Duration always respected before considering change
- **Simplicity:** Easier to understand and configure

### Example
**Before (confusing):**
```json
"WeatherChangeInterval": 240,      // Check every 4 min
"clear": {
    "m_MinDuration_Min": 600,      // Wants 10 min
    "m_MinDuration_Max": 900       // Conflict!
}
```

**After (clear):**
```json
"RandomWeatherChance": 30,         // 30% to change
"clear": {
    "m_MinDuration_Min": 600,      // 10 min minimum
    "m_MinDuration_Max": 900       // 15 min maximum
}
```

### Technical Changes
- Removed `m_WeatherChangeInterval` from `WU_WeatherManager`
- Removed `m_NextChangeTime` timer logic
- Added `m_PresetApplyTime` to track when preset was applied
- `OnUpdate()` now checks if `elapsed >= m_MinDuration` before calling `ApplyRandomWeather()`
- `ApplyRandomWeather()` validates MinDuration before allowing change
- Removed `GetWeatherChangeInterval()` from `WU_ConfigManager`
- Removed `WeatherChangeInterval` field from `WU_ConfigAuto`

---

## Version 3.1 (2025-11-01)

### New Features
- **Wind Function Parameters** (Fixes COT compatibility)
  - Added `m_WindFunc_Min` (default: 0.1): Wind variation minimum multiplier
  - Added `m_WindFunc_Max` (default: 1.0): Wind variation maximum multiplier
  - Added `m_WindFunc_Speed` (default: 20.0): Wind function frequency
  - Replaced hardcoded `SetWindFunctionParams(0.1, 1.0, 20.0)` with configurable per-preset values

- **Rain Threshold Parameters**
  - Added `m_RainThreshold_Min` (default: 0.3): Minimum overcast for rain
  - Added `m_RainThreshold_Max` (default: 1.0): Maximum overcast for rain
  - Added `m_RainThreshold_Timeout` (default: 30): Rain threshold transition time
  - Now properly calls `weather.SetRainThresholds()` API

### Bug Fixes
- Fixed COT weather monitor showing incorrect wind values (0.1/1.0/20.0)
- Fixed missing `SetRainThresholds()` API call in weather application

### Technical Changes
- Updated `WU_WeatherPreset` class with 6 new fields
- Updated `WU_WeatherConfig` class with 6 new fields
- Modified `ApplyWeatherConfig()` to use preset-specific wind function params
- Updated all default preset generation in `WU_ConfigAuto.c` and `WU_ConfigManual.c`

### Compatibility
- **Breaking:** Existing JSON configs need 6 new fields added to each preset
- Auto-upgrade: Delete old configs and restart server to regenerate with new fields
- Manual upgrade: Add the 6 new fields to each preset (see SETTINGS_REFERENCE.md)

### Configuration Updates
- All default presets now include wind function and rain threshold parameters
- Backward compatible default values match vanilla DayZ behavior

---

## Version 3.0 (2025-10-30)

### New Features
- **Enhanced Logging System**
  - Two-tier logging: INFO (production) and DEBUG (troubleshooting)
  - `EnableDebugLogging` setting creates separate debug log file
  - Debug logs include 5-second player temperature tracking
  - Normal logs show zone entry/exit events
  - Safeguard prevents debug log creation when disabled

- **Temperature Zone System**
  - Location-based temperature overrides via `WU_ZoneTemperatureControl.json`
  - Configurable zones with Position, Radius, Height, Temperature range
  - Height check can be disabled (`Height: -1`) for ground-level zones
  - Active player tracking with entry/exit logging
  - `WU_PlayerZonePlugin` for continuous zone monitoring (works even if `Environment` override blocked)

- **Configurable Weather Monitoring**
  - Status logging every 60 seconds
  - Real-time weather value tracking

### Improvements
- Added diagnostic log confirming `Environment.GetEnvironmentTemperature()` override is active
- Zone loading logs display "Height=disabled" when `Height <= 0`
- Improved temperature zone compatibility with map mods (e.g., DeerIsle)
- Added global temperature variables to `WU_TemperatureGlobals.c` for proper load order

### Bug Fixes
- Fixed volumetric fog not clearing when `m_UseDynVolFog: 0`
- Fixed `Can't find variable 'WU_GLOBAL_TEMPERATURE_OVERRIDE'` error
- Fixed zone temperature loading before settings flag
- Fixed multiple declaration errors in zone tracking
- Fixed `Math.RandomFloat(0.0, 0.0)` returning negative values (added `RandomFloatSafe()`)

### Technical Changes
- Moved global temperature variables from `WU_Environment.c` (4_World) to `WU_TemperatureGlobals.c` (3_Game)
- Created `WU_PlayerZonePlugin` for active zone tracking independent of `Environment` override
- Modified `WU_WeatherManager.c` to load configs before checking zone flags
- Updated `GetCurrentWeatherState()` to always log volumetric fog status
- Implemented proper scope management for zone tracking variables

---

## Version 2.0 (2025-10-24)

### New Features
- **Volumetric Fog Support**
  - Added `m_VolFogHeightDensity` (0.0-1.0): Vertical fog density control
  - Added `m_VolFogDistanceDensity` (0.0-1.0): Horizontal fog density control
  - Added `m_VolFogHeightBias` (meters): Height offset for fog layer
  - Automatic detection if volumetric fog is enabled in world config

- **Snowflake Scale Control**
  - Added `m_SnowflakeScale` (0.1-3.0): Customize snowflake size during snowfall
  - Default: 1.0 (normal), larger values = bigger flakes

### Improvements
- **Enhanced Logging System**
  - Complete weather status logged every WeatherCheckInterval (60 seconds)
  - Shows Current vs Expected values for all 9 weather parameters
  - Added volumetric fog status to logs when enabled
  - Added snowflake scale to status reports
  - Time format now uses leading zeros (00:07 instead of 0:7)
  - Clear status messages: drift detection, correction, and tolerance checks

- **Better Startup Behavior**
  - Removed misleading real-world time from startup logs
  - Added note about game time synchronization
  - Clearer preset selection messages

### Bug Fixes
- Fixed time formatting to use leading zeros for consistency with JSON format
- Fixed startup time display showing real-world time instead of game time

### Technical Changes
- Added `FormatGameTime()` helper function for consistent time formatting
- Added volumetric fog API integration with transition support
- Enhanced `GetCurrentWeatherState()` with full parameter logging
- Updated all weather preset classes with new parameters

### Compatibility
- Backward compatible with existing configurations
- Missing volumetric fog parameters default to 0.0 (disabled)
- Missing snowflake scale defaults to 1.0 (normal)
- Volumetric fog gracefully skipped if not enabled in world config

### Configuration Updates
- All default presets include new parameters
- Foggy preset enhanced with volumetric fog (0.5 height, 0.3 distance)
- Snowy preset uses 1.2x snowflake scale
- Blizzard preset uses 1.5x snowflake scale

---

## Version 1.1 (Previous)

### Features
- Manual weather scheduling by game time
- Auto weather with random changes
- 9 default weather presets
- Smooth transitions between presets
- Thunderstorm and lightning control
- Wind direction and speed management
- Real-time weather monitoring
- Configurable check intervals
- Detailed logging system

### Presets
- clear
- partly_cloudy
- overcast
- stormy
- foggy
- snowy
- blizzard
- extreme_storm
- rain

