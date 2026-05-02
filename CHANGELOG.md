# Weather_Upgrade Mod - Changelog

## Version 2.4.1 (Current)

### Bug Fixes
- **Fire Heat Severely Reduced by Temperature Override**
  - `WU_Environment.GetEnvironmentTemperature()` had two interacting bugs that caused fires to give only a small heat buff
  - **Bug A: Artificial 10°C floor** - When no fire was present, a `FIRE_SAFE_MIN_TEMP = 10.0` floor forced env temp to a minimum of 10°C even in cold presets (e.g., -30°C blizzard). This compressed the fire-on vs fire-off delta from ~50°C down to ~10°C, making fires feel weak
  - **Bug B: Override discarded when fire active** - When `m_UTSAverageTemperature > 0`, the function returned only `fireHeat` (capped at 20°C by DayZ engine) and discarded `WU_GLOBAL_TEMPERATURE_OVERRIDE`. This meant in a 22°C stormy preset, walking up to a fire dropped env temp from 22°C to 20°C (fire chilled the player). At the edge of fire range, where UTS average might be 5°C, the override of 22°C was replaced by 5°C
  - **Fix:** Override now sets ambient, fire UTS layers on top via vanilla logic: `effectiveTemp = max(WU_GLOBAL_TEMPERATURE_OVERRIDE, m_UTSAverageTemperature)`. No artificial floor
  - **Result in cold presets:** Player feels full ~50°C jump when sitting by a fire (-30°C → 20°C), matching vanilla DayZ fire behavior
  - **Result in warm presets:** Fire never penalizes the player; override is preserved when fire UTS is lower than the override
- **Misleading Zone Temp Log Messages**
  - "preserving fire heat" log lines in zone temp branch were unclear (`Math.Max(zoneTemp, baseTemp)` already includes fire UTS via vanilla `super.GetEnvironmentTemperature()`)
  - Updated log format to clearly show `ZoneTemp`, `BaseTemp` (with note that it includes fire UTS), and final value

### Reference: Vanilla DayZ Fire Heat
- Fireplace UTS cap: 20°C (`PARAM_MAX_TRANSFERED_TEMPERATURE` in `fireplacebase.c`)
- Bonfire / FireBarrel / burning tirepile UTS cap: 25°C
- Full heat radius: 2m. Falloff range: 2-4m (linear). Beyond 4m: 0
- `m_UTSAverageTemperature` is a 10-tick (10s) moving average - ramps up after sitting by fire

### Bug Fixes (Additional)
- **Could not ignite fireplace / fire kits indoors or outdoors (wind false positive)**
  - Vanilla `FireplaceBase::IsWindy()` uses `GetWindSpeed() >= GetWindMaximumSpeed() * 0.8` with **no** check for being inside a building
  - After `Weather.SetWind()` with a low preset wind (~3 m/s), the engine can leave `GetWindMaximumSpeed()` at a similar few m/s, so the ignite threshold becomes ~2.4 m/s while `GetWindSpeed()` fluctuates around ~3.4 m/s - the game thinks it is always "too windy" to light a fire
  - **Fix:** `WU_WeatherManager` now calls `SetWindMaximumSpeed()` after every `SetWind()`: minimum **20 m/s** (same as Chernarus world init) when preset wind is below 20, or `presetWind * 1.15` for storm-level winds above 20, so `IsWindy()` only trips in genuinely high wind
  - Matches the pattern from older `Weather_Upgrade` backups and `chernarusplus.c` / `sakhal.c` map scripts

### Diagnostic Logging
- **Fire enter/leave events** logged at `[INFO]` level (visible in production log without `EnableDebugLogging`)
  - `[FIREPLACE] Player '<name>' entered fire heat range | FireUTS=<X>°C, Override=<Y>°C, BaseTemp=<Z>°C -> Final=<F>°C` (log code 7209)
  - `[FIREPLACE] Player '<name>' left fire heat range | Override=<Y>°C, BaseTemp=<Z>°C -> Final=<F>°C` (log code 7210)
- **Periodic fire status** logged at `[DEBUG]` level every 30 seconds while near fire (requires `EnableDebugLogging: 1`)
  - `[FIREPLACE TEMP] Player '<name>' Override=<Y>°C, FireUTS=<X>°C, BaseTemp=<Z>°C -> Final=<F>°C (fire layered | override applied)` (log code 7211)
- **Fire detection threshold:** `m_UTSAverageTemperature >= 0.5°C` (filters out residual moving-average noise)

---

## Version 2.4.0

### Architecture
- **Single-File Core Architecture**
  - Initially split `WU_WeatherManager.c` into 6 modular files using `modded class` extensions
  - Reverted: Enforce Script cannot resolve cross-file method calls between `modded class` extensions within the same mod
  - Consolidated back to single `WU_WeatherManager.c` (~1830 lines) + standalone `WU_ScheduleEntry.c` in `Core/`
  - Organized all scripts into subdirectories: `Config/`, `Core/`, `Logger/`, `Template/`, `Temperature/`

### Bug Fixes
- **Schedule Chance Now Functional**
  - `WU_WeatherSchedule.Chance` field was loaded from JSON but never used in selection
  - Schedule entries with `Chance < 100` now have a probability roll before applying
  - `Chance: 100` always applies (deterministic), `Chance: 70` has 70% chance, `Chance: 0` is disabled
  - Initial application always applies regardless of chance value
- **RandomInt Off-by-One (Critical)**
  - DayZ `Math.RandomInt(min, max)` is exclusive on upper bound
  - Fallback selection used `RandomInt(0, Count()-1)` which could never select the last preset
  - Weighted roll used `RandomInt(1, totalWeight)` which could skip the last preset
  - Fixed to `RandomInt(0, Count())` and `RandomInt(1, totalWeight + 1)`
- **HandlePendingInitialConfig Regression**
  - Used `GetGame().GetDeltaTime()` instead of `delta_time` parameter after file split
  - `GetGame()` is deprecated per style guide; now correctly passes `delta_time` from `OnUpdate`
- **Dead Monitoring Code**
  - Monitoring mode read 4 weather values into locals every 10 seconds but never used them
  - Replaced with functional external weather change detection (compare, log, and track)
- **HandleExternalWeatherCheck Dead Code**
  - Called from `OnUpdate` only when NOT disabled, but internal check required disabled = true
  - Function body never executed; logic moved into `HandleMonitoringMode` where it belongs
- **WU_PlayerTempStorage Never Populated**
  - `SetPlayerTemp()` was never called anywhere in the codebase
  - Zone temperature status log templates always showed empty player data
  - Now called from `WU_Environment.GetEnvironmentTemperature()` when zone temps are applied
- **m_FogTransitionTime Not Used**
  - Config JSON `m_FogTransitionTime` was loaded into `WU_WeatherConfig` but never referenced
  - Volumetric fog transitions used hardcoded `0.5s` instead of config value
  - Fixed in both `ApplyVolFogSettings` and `HandleLimitsReassertion`
- **Duplicate Snowfall Set Call**
  - `ApplyPostTransitionLimits` called `weather.GetSnowfall().Set()` unconditionally, then again when snowfall > 0.0
  - Removed the redundant conditional call

### Performance Optimizations
- **Buffered Logger I/O**
  - Logger now buffers log entries in memory and flushes to disk every 5 seconds or 50 entries
  - Previously opened and closed the log file for every single line (~30+ file operations per status log)
  - Reduces disk I/O by ~90% during normal operation, eliminating a major source of server lag
  - Buffer is flushed on shutdown to prevent data loss
- **Tiered Reinforcement System**
  - Split the monolithic `HandleLimitsReassertion` (22+ API calls/sec) into fast and slow paths
  - Fast path (every 5s): Overcast and fog limits + Set() calls (4 API calls)
  - Rain reinforcement: Every 5s with limits + Set() (6 API calls per cycle)
  - Snowfall reinforcement: Every 3s with limits + Set() (6 API calls per cycle)
  - Slow path (every 30s): Wind, storm, thresholds, vol fog, snowflake, temperature
  - Total reduction from ~34 API calls/sec to ~4 API calls/sec (~88% reduction)
- **Reduced Debug Log Interval**
  - `LIMITS_DEBUG_LOG_INTERVAL` increased from 10s to 30s (reduces debug log volume)

### Bug Fixes (Additional)
- **Version String Mismatch** - `MissionServer.c` still reported `V2.3.8`, updated to `V2.4.0`
- **WU_ConfigManager Indentation** - Fixed misaligned braces/return in `LoadConfig()` auto weather path
- **Template Reinforcement Text** - Status template reported "1-second reinforcement" which was inaccurate after interval changes

### Bug Fixes (Testing Phase)
- **Enforce Script modded class compilation failure**
  - The 6-file split using `modded class WU_WeatherManager` failed because Enforce Script cannot resolve cross-file method calls between modded class extensions within the same mod
  - All 6 Core files merged back into a single `WU_WeatherManager.c` (~1830 lines); `WU_ScheduleEntry.c` remains separate (standalone class, not modded)
- **Preset rapid-fire during transitions**
  - When a preset's duration expired and a transition started, `m_PresetApplyTime` was never reset
  - The duration check kept passing every `WeatherCheckInterval` (e.g., 310s >= 300s, 320s >= 300s...), starting a new preset every 10 seconds
  - Transitions never completed; fix: added `if (m_InTransition) return;` guard in `HandleWeatherChanges` to skip duration checks during transitions
- **Log spam from transition progress**
  - `LogTransitionProgress` and `CheckTransitionFinal` fought over `m_LastTransitionProgressStep` every frame
  - One set it to 9 (at 99%), the other to 10 (for >=99%), causing ~22,000 log lines per second
  - Fix: added separate `m_FinalTransitionApplied` boolean flag so the two functions no longer interfere
- **Log timestamps showed in-game time**
  - Logger used `g_Game.GetWorld().GetDate()` which returned in-game date (e.g., `1985-9-22 9:22`)
  - Changed to `GetYearMonthDay()` and `GetHourMinuteSecond()` for real-world server timestamps (e.g., `2026-3-29 23:27:04`)
  - Added zero-padding for hours/minutes/seconds

### New Features (Testing Phase)
- **Separate LogInterval setting**
  - `WeatherCheckInterval` previously controlled both weather checking and status logging
  - Added new `LogInterval` setting to `WU_Settings.json` that independently controls how often the `[WEATHER STATUS]` template block is logged
  - `WeatherCheckInterval` now only controls how often the mod checks for preset duration expiry / schedule changes
  - Default `LogInterval` is 60 seconds

### Technical Changes
- `WU_ScheduleEntry` now includes `Chance` field for schedule probability
- `GetActiveWeatherPreset()` returns chance value via `out` parameter
- Removed dead `HandleExternalWeatherCheck` function from update loop
- New `HandleSlowReinforcement()` function handles wind, storm, thresholds, vol fog every 30s
- `WU_Logger.Update(weatherTime)` called from main update loop for buffered flush
- `WU_Logger.Flush()` called on `OnDestroy()` for clean shutdown
- Consolidated 6 modular Core files back into single `WU_WeatherManager.c` due to Enforce Script limitations
- Added `m_FinalTransitionApplied` flag to prevent log spam between `LogTransitionProgress` and `CheckTransitionFinal`
- Added `if (m_InTransition) return;` guard in `HandleWeatherChanges` to prevent rapid-fire preset changes
- Logger now uses `GetYearMonthDay()`/`GetHourMinuteSecond()` for real-world timestamps instead of in-game time
- Added `LogInterval` setting to `WU_Settings.json` for independent control of status log frequency

---

## Version 2.3.8

### Major Improvements
- **Smooth Snowfall Transitions**
  - Fixed snowfall stopping/resetting during transitions (now transitions smoothly like rain)
  - Snowfall reinforcement enabled during transitions using calculated transition values
  - Snowfall thresholds set to wide (0.0-1.0) during transitions to allow smooth appearance
  - Snowfall reinforcement uses immediate (0.0s) transition time to prevent DayZ override
  - Snowfall now stable and consistent like other weather parameters (rain, overcast, fog)
  - All weather parameters now transition smoothly without snapping or glitches

### Bug Fixes
- Fixed snowfall stopping/resetting during transitions (snowfall now transitions smoothly)
- Fixed snowfall values not being reinforced during transitions
- Fixed snowfall thresholds preventing smooth transitions
- Fixed abrupt snowfall changes at transition completion

### Technical Changes
- **Snowfall Reinforcement System**
  - Aggressive snowfall reinforcement every 0.5 seconds with immediate (0.0s) transition
  - Works during transitions (uses calculated transition values) and when not in transition (uses target values)
  - Matches rain reinforcement behavior for maximum stability
  - Prevents DayZ from overriding snowfall values between updates
- **Snowfall Thresholds During Transitions**
  - Wide thresholds (0.0-1.0) set during transitions to allow snow to appear smoothly
  - Prevents DayZ from checking thresholds and resetting snowfall during transitions
  - Configured thresholds applied after transition completes
  - Thresholds disabled (1.0-1.0) when snowfall target is 0.0
- **Transition System Parity**
  - Snowfall transitions now match rain transition behavior exactly
  - All weather parameters (overcast, fog, rain, snowfall, wind, volumetric fog) transition smoothly
  - Consistent transition value calculation and application across all parameters

### Performance
- No performance impact - same 0.5-second snowfall reinforcement cycle
- Smooth transitions prevent visual glitches and snapping
- Snowfall reinforcement prevents DayZ override without performance cost

---

## Version 2.3.7

### Technical Changes
- Updated mod version to 2.3.7

---

## Version 2.3.6

### Critical Fixes
- **Fire Heat Preservation**
  - Fixed players not receiving heat buffs from fires when temperature override is active
  - Fire heat now always takes priority when present, even with extreme cold overrides (-30°C)
  - Temperature override now correctly applies when no fire heat is present
  - Removed incorrect baseTemp preservation logic that was preventing override from working

### Bug Fixes
- Fixed temperature override not applying correctly (was preserving baseTemp instead of using override)
- Fixed players dying of cold even when temperature override was set to cold values
- Fixed fire heat not working with temperature override active

### Technical Changes
- **Temperature Override Logic**
  - Priority 1: Use fire heat if present (fires always work)
  - Priority 2: Use temperature override if no fire heat
  - Removed baseTemp preservation that was interfering with override
- **Zone Temperature Control**
  - Zone temperature still preserves fire heat correctly
  - Uses Math.Max(zoneTemp, baseTemp) to ensure fire heat is never lost

---

## Version 2.3.5

### Major Improvements
- **Smooth Rain Transitions**
  - Fixed rain stopping/resetting during transitions (now transitions smoothly like overcast/fog)
  - Rain reinforcement enabled during transitions using calculated transition values
  - Rain thresholds set to wide (0.0-1.0) during transitions to allow smooth appearance
  - Rain reinforcement uses immediate (0.0s) transition time to prevent DayZ override
  - Rain now stable and consistent like other weather parameters

- **Smooth Transitions System**
  - Fixed transition values not being applied in Manual mode (m_MinDuration = 0)
  - Transition values now update every 1 second with immediate application (0.0s transition time)
  - Prevents DayZ from overriding values during transitions
  - Smooth completion at 99%+ progress to prevent snapping at transition end

### Features
- **Enhanced Transition Logging**
  - Progress logs every 10% (0%, 10%, 20%, ..., 100%) instead of every 25%
  - Debug logs every 1 second showing calculated intermediate values during transitions
  - Detailed transition progress tracking with current vs target values
  - Fixed log spam - final values log (2259) now only appears once per transition

- **Manual Mode Improvements**
  - `DefaultWeatherPreset` removed from Manual mode (not needed - schedule determines preset)
  - Schedule-based preset selection works perfectly - preset determined by in-game time
  - Empty `DefaultWeatherPreset` is valid in Manual mode (schedule handles everything)

### Bug Fixes
- Fixed rain stopping/resetting during transitions (rain now transitions smoothly)
- Fixed rain values "spinning" rapidly in admin tools (changed to 0.0s transition time)
- Fixed transition values not being applied in Manual mode (m_MinDuration = 0 check was preventing application)
- Fixed abrupt snap at transition completion (now uses smooth 0.5s transition for final values)
- Fixed log number conflict (8004 -> 8017 for FOG logs)
- Fixed transition values being calculated but not applied during transitions
- Fixed log spam - final values log (2259) now only appears once per transition completion

### Technical Changes
- **Rain Reinforcement System**
  - Aggressive rain reinforcement every 1.0 seconds with immediate (0.0s) transition
  - Works during transitions (uses calculated transition values) and when not in transition (uses target values)
  - Matches snowfall reinforcement behavior for maximum stability
  - Prevents DayZ from overriding rain values between updates

- **Rain Thresholds During Transitions**
  - Wide thresholds (0.0-1.0) set during transitions to allow rain to appear smoothly
  - Prevents DayZ from checking thresholds and resetting rain during transitions
  - Configured thresholds applied after transition completes

- **Transition System**
  - Transition values now always applied (removed m_MinDuration > 0 check)
  - Manual mode uses 99999.0 duration during transitions to prevent DayZ override
  - Auto mode uses actual m_MinDuration value during transitions
  - Final values applied smoothly at 99%+ completion (0.5s transition) instead of immediate snap (0.0s)
  - Added debug logging for transition values every 1 second (log #2258)
  - Final values logging at completion (log #2259) - only logs once per transition

- **Manual Mode Configuration**
  - `DefaultWeatherPreset` field removed from default Manual config generation
  - Manual mode uses schedule to determine preset - no fallback needed
  - Schedule lookup finds most recent entry ≤ current game time

### Performance
- No performance impact - same 1-second update cycle
- Smooth transitions prevent visual glitches and snapping
- Rain reinforcement prevents DayZ override without performance cost

---

## Version 2.3.4

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

### Additional Features in v2.3.4
- **Configuration Improvements**
  - Centralized `WeatherCheckInterval` to `WU_Settings.json` (single setting for both modes)
  - Default value changed from 60s to 30s (more responsive monitoring)
  - Simplified configuration - one place to control weather status logging

- **Bug Fixes**
  - Fixed timer reset issue (presets now change correctly after duration)
  - Fixed excessive logging spam (rain thresholds, temperature)
  - Fixed rain sounds playing when rain is disabled (winter maps)
  - Reduced log noise (96% reduction on hot-reloads)
  - Improved manual mode display (shows next schedule time)

- **Performance Optimizations**
  - Removed drift correction system (performance optimization)
  - Relies on `MissionWeather` API for stability
  - 90% reduction in weather API calls

- **System Improvements**
  - Removed `WeatherChangeInterval` (simplified auto mode)
  - Per-preset duration control only
  - Wind Function parameters (COT compatibility fix)
  - Rain Threshold controls
  - Enhanced logging (INFO + DEBUG)
  - Temperature zone system
  - DisableWeatherUpgrade setting for AdminTools compatibility

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
  - Complete weather status logged every WeatherCheckInterval (default: 30 seconds)
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

