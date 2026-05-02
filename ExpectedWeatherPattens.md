# Weather_Upgrade System Flow

## Configuration Parameters

- **LogInterval** (60s): How often to log the `[WEATHER STATUS]` template block (independent from weather checks)
- **WeatherCheckInterval** (30s): How often to check for preset duration expiry (auto mode) or schedule changes (manual mode)
- **m_WeatherChance**: Weight value per preset for auto mode weighted random selection
- **m_TransitionTime_XXX** (60s): Duration of smooth manual transition between presets
- **m_MinDuration_XXX** (120s): Duration values stay LOCKED after transition completes (does NOT include transition time)
- **Chance** (0-100): Per-schedule-entry probability in manual mode

---

## Script File Structure

```
Scripts/
├── 3_Game/
│   ├── Config/         WU_Settings, WU_ConfigAuto, WU_ConfigManual, WU_ConfigManager,
│   │                   WU_WeatherConfig, WU_ZoneTemperature
│   ├── Core/           WU_WeatherManager, WU_ScheduleEntry
│   ├── Logger/         WU_Logger
│   ├── Template/       WU_Template, WU_TemplateTransition, WU_Template_TempZones
│   └── Temperature/    WU_TemperatureGlobals, WU_PlayerTempStorage
├── 4_World/            WU_Environment, WU_PlayerZonePlugin
└── 5_Mission/          MissionServer
```

---

## Complete System Flow

### 1. Server Startup - Immediate Application
- Load configuration files (`WU_Settings.json`, `WU_AutoWeather.json` or `WU_ManualWeather.json`)
- Select default preset from `DefaultWeatherPreset` (auto mode) or schedule (manual mode)
- Generate random values for preset (within Min/Max ranges)
- **Apply preset IMMEDIATELY (0.0s transition) - 100% values locked**
- Apply 5 times (0.5s, 1.5s, 3s, 5s, 8s) to override DayZ's persistent storage
- Set `m_PresetApplyTime` = current time (duration timer starts)
- **TIGHT limits applied (±0.005 tolerance) - values locked**

### 2. Locked State - Tiered Reinforcement System

**Fast Path (Every 5 Seconds):**
- Re-assert TIGHT limits (±0.005 tolerance) for overcast and fog
- Apply overcast and fog with 0.5s smooth transition

**Rain Reinforcement (Every 5 Seconds):**
- Re-assert TIGHT limits for rain
- Apply rain value with 0.0s transition (immediate)

**Snowfall Reinforcement (Every 3 Seconds):**
- Re-assert TIGHT limits for snowfall
- **Apply snowfall with 0.0s transition (immediate) - prevents drift**

**Slow Path (Every 30 Seconds):**
- Wind Speed/Direction: Apply immediately
- Wind Function Parameters: Apply immediately
- Rain/Snowfall Thresholds: Apply immediately
- Storm/Lightning: Apply immediately
- Volumetric Fog: Apply with `m_FogTransitionTime` from config
- Snowflake Scale: Apply immediately
- Temperature Override: Apply immediately

**Result:** Values stay locked - NO DRIFT. Tiered intervals reduce API calls by ~88%.

**WeatherCheckInterval Loop (Every 30s):**
- **Auto mode:** Check if `m_PresetApplyTime + m_MinDuration` has elapsed (skipped if `m_InTransition`)
- **Manual mode:** Check if in-game time has reached next schedule entry

**LogInterval Loop (Every 60s):**
- Template status log (8000-8049 codes)

### 3. Duration Expires (Auto Mode)
- Check every `WeatherCheckInterval` (30s) if `m_PresetApplyTime + m_MinDuration` has elapsed
- When elapsed: Select new preset using weighted random based on `m_WeatherChance` values
- If same preset selected: Extend duration, keep current weather
- If different preset selected: Start transition

### 3b. Schedule Change (Manual Mode)
- Check every `WeatherCheckInterval` if in-game time has moved past a schedule entry
- When a new schedule entry is active and differs from current preset:
  - Roll `Chance` (0-100): if random roll < `Chance`, apply the preset
  - If `Chance = 100` (default), always apply
  - If chance roll fails, skip and keep previous preset
  - First application always applies immediately regardless of chance

### 4. Transition Start (When New Preset Selected)
- `m_TransitionStartTime` = current time
- `m_CurrentWeather` = TARGET preset config
- `m_InTransition` = true
- `m_PresetApplyTime` = NOT set yet (duration timer paused)
- Store FROM values (current actual values)
- Apply WIDE limits (0.0-1.0) to allow smooth blending

**Manual Transition Calculation - Every 1 Second:**
- Calculate progress: `progressRatio = elapsedTime / m_TransitionTime`
- Calculate intermediate values: `currentValue = FROM + (TO - FROM) * progressRatio`
- Apply calculated values with smooth transition

**Values Calculated Every 1 Second:**
- Overcast, Fog, Rain, Snowfall: Linear interpolation
- Wind Speed: Linear interpolation
- Wind Direction: Circular interpolation (shortest path 0-360)
- Snowflake Scale: Linear interpolation
- Volumetric Fog: Linear interpolation

### 5. Transition Complete
- `m_InTransition` = false
- `m_PresetApplyTime` = current time (duration timer starts NOW)
- Apply TIGHT limits (±0.005 tolerance) to lock values
- Force re-application of all target values
- Apply configured thresholds (rain/snowfall)
- Values now LOCKED for `m_MinDuration` seconds (auto mode) or until next schedule entry (manual mode)
- Return to step 2 (Locked State - Tiered Reinforcement)

---

## Key Points

1. **Immediate Application on Startup:**
   - DefaultWeatherPreset applied IMMEDIATELY (0.0s transition)
   - Applied 5 times to override DayZ's persistent storage
   - Values locked 100% from the start

2. **Tiered Reinforcement System:**
   - Overcast/Fog: Every 5s with TIGHT limits + 0.5s smooth transition
   - Rain: Every 5s with TIGHT limits + 0.0s immediate transition
   - **Snowfall: Every 3s with TIGHT limits + 0.0s immediate transition - prevents drift**
   - Wind/Storm/Thresholds/VolFog: Every 30s (slow path)
   - **Result: NO DRIFT - ~88% fewer API calls than v2.3.x**

3. **Manual Transition Control:**
   - We calculate intermediate values every 1 second
   - Formula: `currentValue = FROM + (TO - FROM) * progressRatio`
   - **We control the precise weather - DayZ only executes**

4. **Duration Timer:**
   - Starts AFTER transition completes, not when transition starts
   - `m_MinDuration` = time values are LOCKED, excluding transition time
   - `HandleWeatherChanges` has an `if (m_InTransition) return;` guard to prevent duration checks during transitions
   - Without this guard, `m_PresetApplyTime` (not reset at transition start) would cause the duration check to pass every `WeatherCheckInterval`, starting a new preset every cycle

5. **WeatherCheckInterval:**
   - Checks if preset duration has elapsed (auto mode) or schedule has changed (manual mode)
   - Does NOT control logging frequency (use `LogInterval` for that)

6. **External Weather Detection (Monitoring Mode):**
   - When `DisableWeatherUpgrade = 1`, mod enters monitoring mode
   - Detects external weather changes (AdminTools, other mods) by comparing actual vs stored values
   - Logs changes but does not override them

---

## Control Philosophy

- We pass ALL config values to DayZ (we don't rely on DayZ's defaults)
- We control the duration precisely (m_MinDuration from config)
- We control the precise weather values (from config ranges)
- We calculate and apply intermediate values every 1 second during transitions
- We reinforce values using a tiered system (5s overcast/fog/rain, 3s snowfall, 30s wind/storm)
- **DayZ only EXECUTES our commands - it does NOT control anything**
