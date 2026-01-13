# Weather_Upgrade Mod - Changelog

## Version 2.3.6 (Current)

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

