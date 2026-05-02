# Weather_Upgrade - Known Issues & TODO

**Last Updated:** 2026-04-30  
**Mod Version:** 2.4.1

---

## Code Issues

*No open code issues at this time.*

---

## DayZ Engine Limitations

### Limitation #001: Snowfall Rapid Decay
**Description:**
DayZ engine causes snowfall to decay faster than other weather phenomena.

**Workaround:**
- 3-second reinforcement interval with tight limits (±0.005)
- Snowfall threshold max must be >= overcast max
- Use immediate (0.0s) transitions for snowfall reinforcement

---

### Limitation #002: Rain Sound Bleeding
**Description:**
Rain sounds may play when overcast > 0.7 even with rain intensity = 0.

**Workaround:**
- Set rain thresholds to disable when rain is 0
- This is an engine limitation, not fully fixable in script

---

### Limitation #003: Weather Persistence Override
**Description:**
DayZ may restore weather from `weather.bin` on server start.

**Workaround:**
- Multi-apply on startup: 0.5s, 1.5s, 3s, 5s, 8s delays
- Forces override of persistent storage
- `MissionWeather(true)` reasserted every 60 seconds

---

## Feature Requests / Nice to Have

### Request #001: Chat Commands
**Status:** Planned  
**Priority:** Low

**Description:**
Add chat commands similar to DWP:
- `!skipweather` - Force next preset
- `!weatherstatus` - Display current preset info

---

### Request #002: Player Notifications
**Status:** Planned  
**Priority:** Low

**Description:**
Optional notifications when weather changes:
- Chat message (like DWP)
- Notification system popup

---

### Request #003: Binary Persistence
**Status:** Planned  
**Priority:** Low

**Description:**
Save current weather state to binary file for restart recovery (like DWP's `dynamicweatherV2.bin`).

---

## Completed Fixes (Reference)

### Fixed in v2.4.1

**Bug Fixes:**
- **Fire heat severely reduced by temperature override** - `WU_Environment.GetEnvironmentTemperature()` had two interacting bugs causing vanilla fireplace heat to feel weak when a temperature-override preset was active:
  - **Bug A: 10°C floor masked cold presets.** A `FIRE_SAFE_MIN_TEMP = 10.0` floor in the no-fire branch forced env temp to a minimum of 10°C even in -30°C blizzard presets. This compressed the fire-on vs fire-off delta from ~50°C to ~10°C.
  - **Bug B: Override discarded when fire active.** When `m_UTSAverageTemperature > 0`, the function returned only fire UTS (capped at 20°C by DayZ engine) and discarded the preset override. In a 22°C stormy preset, walking up to a fire dropped env temp to 20°C (fire chilled the player). At the edge of fire range, fires returned 5°C instead of the 22°C override.
- **Fix:** Override now sets ambient, fire UTS layers on top via vanilla logic: `effectiveTemp = max(WU_GLOBAL_TEMPERATURE_OVERRIDE, m_UTSAverageTemperature)`. No artificial floor.
- **Result:** In a -6°C snowy preset, walking up to a fireplace now correctly raises env temp to 20°C (a ~26°C boost). In a -30°C blizzard, fires raise env temp to 20°C (a ~50°C boost). In warm presets, fires never penalize the player.
- **Vanilla fire systems untouched:** Heat buffer accumulation, cooking, item heating, and clothes drying all use the same vanilla code paths and are unaffected.
- **Misleading zone temp log messages** - "preserving fire heat" log lines in zone branch were unclear. Updated log format to clearly show `ZoneTemp`, `BaseTemp` (with note that it includes fire UTS), and final value.
- **Could not ignite fireplace (even indoors with doors closed)** - Vanilla `FireplaceBase::IsWindy()` uses `GetWindSpeed() >= GetWindMaximumSpeed() * 0.8` and does **not** check for buildings. After `Weather.SetWind()` with a light breeze (~3 m/s), the engine can leave `GetWindMaximumSpeed()` near that value, so the ignite threshold becomes ~2.4 m/s while actual wind stays ~3.4 m/s - the game always thinks it is too windy. **Fix:** `WU_WeatherManager` calls `SetWindMaximumSpeed(20)` (Chernarus default) or `presetWind * 1.15` for winds above 20 m/s after every `SetWind()`.

### Fixed in v2.4.0

**Bug Fixes:**
- **Schedule Chance field now functional** - `WU_WeatherSchedule.Chance` field is now used during schedule selection. Entries with `Chance < 100` have a probability-based roll before applying.
- **RandomInt off-by-one (fallback)** - Fixed `Math.RandomInt(0, Count()-1)` to `Math.RandomInt(0, Count())`. The last preset in equal-weight fallback selection could never be picked.
- **RandomInt off-by-one (weighted roll)** - Fixed `Math.RandomInt(1, totalWeight)` to `Math.RandomInt(1, totalWeight + 1)`. The last preset in weighted selection could be skipped.
- **HandlePendingInitialConfig regression** - Fixed use of `GetGame().GetDeltaTime()` (deprecated) instead of `delta_time` parameter. Regression introduced during file split.
- **Dead monitoring code** - Monitoring mode read weather values into local variables without comparing or logging them. Replaced with functional external weather change detection.
- **HandleExternalWeatherCheck dead code** - Function was called when `DisableWeatherUpgrade` was false but internally required it to be true. Moved logic into monitoring mode where it belongs.
- **WU_PlayerTempStorage never populated** - `SetPlayerTemp()` was never called. Zone temperature status logs always showed empty player data. Now called from `WU_Environment.GetEnvironmentTemperature()`.
- **m_FogTransitionTime not used** - Volumetric fog transitions used hardcoded 0.5s instead of the `m_FogTransitionTime` config value. Now uses the config value.
- **Duplicate snowfall Set call** - `ApplyPostTransitionLimits` called `weather.GetSnowfall().Set()` twice (unconditional + conditional). Removed redundant call.

**Performance:**
- **Buffered Logger I/O** - Logger now batches writes (flush every 5s or 50 entries) instead of opening/closing the file per line. ~90% reduction in disk I/O.
- **Tiered Reinforcement** - Split `HandleLimitsReassertion` into fast/slow paths. Reduced weather API calls from ~34/sec to ~4/sec (~88% reduction). Addresses reported "server lags on preset changes."
- **Version String Mismatch** - `MissionServer.c` still reported `V2.3.8`, fixed to `V2.4.0`.
- **ConfigManager Indentation** - Fixed misaligned braces in `LoadConfig()`.
- **Template Reinforcement Text** - Updated status template to reflect new tiered intervals.

**Bug Fixes (Testing Phase):**
- **Enforce Script modded class compilation failure** - The 6-file split using `modded class WU_WeatherManager` failed; Enforce Script cannot resolve cross-file method calls between modded class extensions within the same mod. Merged back into single `WU_WeatherManager.c` (~1830 lines).
- **Preset rapid-fire during transitions** - `m_PresetApplyTime` was never reset when a transition started, causing the duration check to pass every `WeatherCheckInterval` and start a new preset every 10 seconds. Added `if (m_InTransition) return;` guard in `HandleWeatherChanges`.
- **Log spam from transition progress** - `LogTransitionProgress` and `CheckTransitionFinal` fought over `m_LastTransitionProgressStep` every frame, causing ~22,000 log lines per second. Added separate `m_FinalTransitionApplied` boolean flag.
- **Log timestamps showed in-game time** - Logger used `g_Game.GetWorld().GetDate()` returning in-game date. Changed to `GetYearMonthDay()`/`GetHourMinuteSecond()` for real-world server timestamps with zero-padding.

**New Feature (Testing Phase):**
- **Separate LogInterval setting** - Added `LogInterval` to `WU_Settings.json` for independent control of `[WEATHER STATUS]` template log frequency. `WeatherCheckInterval` now only controls preset duration expiry / schedule checks.

**Architecture:**
- Initially split `WU_WeatherManager.c` into 6 modular files; reverted due to Enforce Script limitations
- Consolidated to single `WU_WeatherManager.c` (~1830 lines) + standalone `WU_ScheduleEntry.c` in `Core/`
- Organized scripts into `Config/`, `Core/`, `Logger/`, `Template/`, `Temperature/` subdirectories
- Version string unified to 2.4.0

### Fixed in v2.3.8
- Smooth snowfall transitions (no longer stops/resets)
- Snowfall reinforcement system (0.5s interval)
- Snowfall thresholds widened during transitions

### Fixed in v2.3.6
- Fire heat works with temperature override
- Players receive heat from fires even at -30°C
- Version string updated in MissionServer.c

### Fixed in v2.3.5
- Smooth rain transitions
- Rain reinforcement system (1.0s interval)
- Fixed transition values in Manual mode

### Fixed in v2.3.4
- Multiple variable declarations (style guide compliance)
- Removed extra requiredAddons from config.cpp
- Added WU_ prefix to member variables

---

## Testing Checklist Reference

For comprehensive testing procedures, see [Checklist.md](Checklist.md).

**Quick Test Summary:**
- 8/8 test presets verified working
- All transitions timing accurate (±0.01s)
- Performance: Zero FPS impact
- Memory: Stable (~2.85 GB, no leaks)

---

## See Also

- [Checklist.md](Checklist.md) - Pre-launch test checklist
- [ExpectedWeatherPattens.md](ExpectedWeatherPattens.md) - System flow documentation

