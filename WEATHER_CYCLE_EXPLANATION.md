# Weather_Upgrade: Complete Weather Cycle Explanation

## Overview

This document explains in detail how the automatic weather system works, from preset selection through duration and transitions, using your `WU_AutoWeather.json` configuration.

---

## 🔄 **COMPLETE CYCLE FLOW**

### **Phase 1: Server Startup & Initial Preset Selection**

#### Step 1.1: Server Starts
- Weather_Upgrade mod initializes
- Loads `WU_AutoWeather.json` configuration
- Reads all weather presets and their `m_WeatherChance` values

#### Step 1.2: Initial Preset Selection (Using `m_WeatherChance`)
- System uses **weighted random selection** based on `m_WeatherChance`
- **Example from your config:**
  - `clear`: weight 25
  - `partly_cloudy`: weight 20
  - `overcast`: weight 15
  - `rain`: weight 10
  - `stormy`: weight 5
  - `foggy`: weight 10
  - `snowy`: weight 15
  - **Total weight: 100**

**How Weighted Selection Works:**
1. System calculates total weight: `25 + 20 + 15 + 10 + 5 + 10 + 15 = 100`
2. Rolls random number from `1` to `100` (inclusive, via `RandomInt(1, totalWeight + 1)`)
3. Example roll: `37`
4. System checks accumulated weights:
   - `clear`: 0-25 (roll 37 > 25, continue)
   - `partly_cloudy`: 25-45 (roll 37 ≤ 45, **SELECTED!**)
5. `partly_cloudy` preset is selected

#### Step 1.3: Generate Random Values from Preset
- System calls `GenerateRandomValues()` on selected preset
- **Example: `partly_cloudy` preset**
  - `m_Overcast_Min: 0.2` → `m_Overcast_Max: 0.5`
  - System generates: `m_Overcast = 0.35` (random between 0.2-0.5)
  - `m_Fog_Min: 0` → `m_Fog_Max: 0.1`
  - System generates: `m_Fog = 0.05` (random between 0-0.1)
  - `m_Rain_Min: 0` → `m_Rain_Max: 0`
  - System generates: `m_Rain = 0` (no rain)
  - **Same process for all weather parameters**

#### Step 1.4: Generate Transition & Duration Times
- **Transition Time:**
  - `m_TransitionTime_Min: 180` → `m_TransitionTime_Max: 300`
  - System generates: `m_TransitionTime = 240` seconds (random between 180-300)
  - **This is how long it takes to transition FROM current weather TO new preset values**

- **Min Duration:**
  - `m_MinDuration_Min: 600` → `m_MinDuration_Max: 1200`
  - System generates: `m_MinDuration = 900` seconds (random between 600-1200)
  - **This is how long the preset will stay active AFTER transition completes**

#### Step 1.5: Apply Weather (Immediate on Startup)
- Since this is initial application, `immediate = true`
- Weather values applied instantly (no transition)
- `m_PresetApplyTime` is set to current `m_WeatherTime` (server time counter)
- Weather is now **locked** for `m_MinDuration` seconds (900s in this example)

---

### **Phase 2: Weather Duration Period**

#### Step 2.1: Weather Check & Log Intervals
- System checks for preset duration expiry every `WeatherCheckInterval` seconds (default: 30s)
- System logs weather status template every `LogInterval` seconds (default: 60s)
- **First check happens 10 seconds after startup** (special early check)

#### Step 2.2: Duration Check
- System calculates: `elapsedTime = m_WeatherTime - m_PresetApplyTime`
- **Example:**
  - `m_PresetApplyTime = 100` (when preset was applied)
  - Current `m_WeatherTime = 850`
  - `elapsedTime = 850 - 100 = 750 seconds`
  - `m_MinDuration = 900 seconds`
  - **Check: `750 < 900` → Duration NOT elapsed, keep current weather**

#### Step 2.3: Tiered Weather Reinforcement (During Duration)
- **Fast path (every 5s):** Overcast and fog limits + Set() calls
- **Rain reinforcement (every 5s):** Rain limits + Set() with 0.0s transition
- **Snowfall reinforcement (every 3s):** Snowfall limits + Set() with 0.0s transition (prevents drift)
- **Slow path (every 30s):** Wind, storm, thresholds, volumetric fog, snowflake scale, temperature
- **Purpose:** Prevents DayZ from overriding weather values (~88% fewer API calls than per-frame)

#### Step 2.4: Duration Elapsed
- **Example:**
  - `m_PresetApplyTime = 100`
  - Current `m_WeatherTime = 1050`
  - `elapsedTime = 1050 - 100 = 950 seconds`
  - `m_MinDuration = 900 seconds`
  - **Check: `950 >= 900` → Duration ELAPSED!**

---

### **Phase 3: New Preset Selection**

#### Step 3.1: Trigger New Selection
- System calls `ApplyRandomWeather()`
- Checks if duration elapsed (already confirmed above)

#### Step 3.2: Weighted Random Selection (Again)
- **Example:**
  - System rolls: `78`
  - Checks accumulated weights:
    - `clear`: 0-25 (78 > 25, continue)
    - `partly_cloudy`: 25-45 (78 > 45, continue)
    - `overcast`: 45-60 (78 > 60, continue)
    - `rain`: 60-70 (78 > 70, continue)
    - `stormy`: 70-75 (78 > 75, continue)
    - `foggy`: 75-85 (78 ≤ 85, **SELECTED!**)
  - `foggy` preset is selected

#### Step 3.3: Generate New Random Values
- **Example: `foggy` preset**
  - `m_Overcast_Min: 0.5` → `m_Overcast_Max: 0.8`
  - Generated: `m_Overcast = 0.65`
  - `m_Fog_Min: 0` → `m_Fog_Max: 0` (but volumetric fog is used)
  - `m_VolFogDistanceDensity_Min: 0.4` → `m_VolFogDistanceDensity_Max: 0.8`
  - Generated: `m_VolFogDistanceDensity = 0.6`
  - `m_TransitionTime_Min: 180` → `m_TransitionTime_Max: 360`
  - Generated: `m_TransitionTime = 270` seconds
  - `m_MinDuration_Min: 900` → `m_MinDuration_Max: 1800`
  - Generated: `m_MinDuration = 1350` seconds

---

### **Phase 4: Weather Transition**

#### Step 4.1: Start Transition
- System calls `ApplyWeatherConfig(config, false, "foggy")`
- `immediate = false` (transition mode)
- **Current weather state:**
  - `m_Overcast = 0.35` (from `partly_cloudy`)
  - `m_Fog = 0.05`
  - `m_Rain = 0`
  - `m_Snowfall = 0`
- **Target weather state:**
  - `m_Overcast = 0.65` (from `foggy`)
  - `m_Fog = 0.0` (but volumetric fog active)
  - `m_Rain = 0`
  - `m_Snowfall = 0`

#### Step 4.2: Store Transition Values
- System stores "FROM" values:
  - `m_TransitionFromOvercast = 0.35`
  - `m_TransitionFromFog = 0.05`
  - `m_TransitionFromRain = 0.0`
  - `m_TransitionFromSnowfall = 0.0`
- System stores transition duration:
  - `m_CurrentTransitionDuration = 270` seconds
- System sets transition start time:
  - `m_TransitionStartTime = m_WeatherTime` (e.g., 1050)
- System sets transition flags:
  - `m_InTransition = true`
  - `m_LastTransitionProgressStep = -1` (reset progress logging)
  - `m_FinalTransitionApplied = false` (reset final-values flag)
- **Transition guard:** While `m_InTransition = true`, `HandleWeatherChanges` skips all duration checks, preventing new presets from being selected mid-transition

#### Step 4.3: Transition Progress Calculation
- **Every 1 second during transition:**
  - `elapsedTime = m_WeatherTime - m_TransitionStartTime`
  - **Example at 100 seconds into transition:**
    - `elapsedTime = 100`
    - `m_CurrentTransitionDuration = 270`
    - `progressRatio = 100 / 270 = 0.37` (37% complete)
  - **Calculate intermediate values:**
    - `currentOvercast = 0.35 + (0.65 - 0.35) * 0.37 = 0.35 + 0.11 = 0.46`
    - `currentFog = 0.05 + (0.0 - 0.05) * 0.37 = 0.05 - 0.0185 = 0.0315`
  - **Apply intermediate values immediately** (0.0s transition) to prevent DayZ override
- **Progress logging:** Logged at every 10% step (0%, 10%, 20%, ..., 100%) - only when the step actually changes, preventing log spam

#### Step 4.4: Transition Completion
- **When `elapsedTime >= m_CurrentTransitionDuration`:**
  - `elapsedTime = 270` (full transition time)
  - `progressRatio = 270 / 270 = 1.0` (100% complete)
  - **Final values applied:**
    - `currentOvercast = 0.35 + (0.65 - 0.35) * 1.0 = 0.65` ✅
    - `currentFog = 0.05 + (0.0 - 0.05) * 1.0 = 0.0` ✅
  - System sets:
    - `m_InTransition = false`
    - `m_PresetApplyTime = m_WeatherTime` (e.g., 1320)
  - **Weather is now locked for new `m_MinDuration`** (1350 seconds)

---

### **Phase 5: New Duration Period**

#### Step 5.1: Weather Locked
- Weather values are locked at target values (0.65 overcast, 0.0 fog, etc.)
- System reinforces values using tiered intervals (5s fast, 3s snowfall, 30s slow)
- **Duration timer running:**
  - `m_PresetApplyTime = 1320`
  - `m_MinDuration = 1350` seconds
  - Weather will stay until `m_WeatherTime = 2670` (1320 + 1350)

#### Step 5.2: Duration Check Loop
- Every `WeatherCheckInterval` (30s), system checks:
  - `elapsedTime = m_WeatherTime - m_PresetApplyTime`
  - If `elapsedTime < m_MinDuration` → Keep current weather
  - If `elapsedTime >= m_MinDuration` → Select new preset

#### Step 5.3: Cycle Repeats
- When duration elapses, **Phase 3** starts again
- New preset selected using `m_WeatherChance`
- New transition begins using `m_TransitionTime`
- New duration period starts using `m_MinDuration`

---

## 📊 **DETAILED PARAMETER EXPLANATION**

### **1. `m_WeatherChance` (Selection Weight)**

**Purpose:** Determines how likely each preset is to be selected.

**How it works:**
- Each preset has a `m_WeatherChance` value (0-100+)
- System calculates total weight: `sum of all m_WeatherChance values`
- Rolls random number from `1` to `totalWeight` (inclusive via `RandomInt(1, totalWeight + 1)`)
- Finds which preset's accumulated weight range contains the roll

**Example from your config:**
```
clear: 25          → Range: 1-25
partly_cloudy: 20  → Range: 26-45
overcast: 15       → Range: 46-60
rain: 10           → Range: 61-70
stormy: 5          → Range: 71-75
foggy: 10          → Range: 76-85
snowy: 15          → Range: 86-100
```

**Important Notes:**
- These are **relative weights**, not percentages - values don't need to add up to 100
- You can use any positive numbers (e.g., 30, 850, 1000 - system normalizes automatically)
- Higher values = more likely to be selected
- `0` = never selected (excluded from the pool)

---

### **2. `m_TransitionTime` (Transition Duration)**

**Purpose:** How long it takes to smoothly transition from current weather to new preset values.

**How it works:**
- Generated from `m_TransitionTime_Min` and `m_TransitionTime_Max`
- **Example: `partly_cloudy`**
  - `m_TransitionTime_Min: 180` seconds (3 minutes)
  - `m_TransitionTime_Max: 300` seconds (5 minutes)
  - Generated: `240` seconds (4 minutes)
- During transition, values smoothly interpolate from "FROM" to "TO"
- **Transition happens BEFORE duration period starts**

**Timeline Example:**
```
Time 0s:    Current weather (overcast 0.7)
Time 240s:  Transition complete (overcast 0.35) ← Duration starts here
Time 1140s: Duration complete (900s after transition)
```

**Important Notes:**
- Transition time is **separate** from duration time
- Longer transitions = smoother weather changes
- Shorter transitions = more dramatic weather changes

---

### **3. `m_MinDuration` (Weather Lock Duration)**

**Purpose:** How long the preset stays active after transition completes.

**How it works:**
- Generated from `m_MinDuration_Min` and `m_MinDuration_Max`
- **Example: `partly_cloudy`**
  - `m_MinDuration_Min: 600` seconds (10 minutes)
  - `m_MinDuration_Max: 1200` seconds (20 minutes)
  - Generated: `900` seconds (15 minutes)
- **Duration starts AFTER transition completes**
- During duration, weather values are locked and reinforced

**Timeline Example:**
```
Time 0s:    Transition starts
Time 240s:  Transition complete → Duration starts
Time 1140s: Duration complete → New preset selection
```

**Important Notes:**
- Duration is the **stable period** where weather doesn't change
- Longer durations = more consistent weather
- Shorter durations = more frequent weather changes
- Duration expiry checked every `WeatherCheckInterval` (default: 30s)
- Status logged every `LogInterval` (default: 60s) - independent from duration checks

---

## 🔄 **COMPLETE CYCLE TIMELINE EXAMPLE**

Using your `partly_cloudy` preset as example:

### **Cycle Start (Server Startup)**
```
Time: 0s
Action: Weighted selection → "partly_cloudy" selected (rolled 37)
Generated Values:
  - Overcast: 0.35 (from 0.2-0.5 range)
  - Fog: 0.05 (from 0-0.1 range)
  - Rain: 0
  - TransitionTime: 240s (from 180-300 range)
  - MinDuration: 900s (from 600-1200 range)
Application: Immediate (no transition)
m_PresetApplyTime: 0s
```

### **Duration Period**
```
Time: 0s - 900s
Status: Weather locked at generated values
Reinforcement: Tiered (5s overcast/fog/rain, 3s snowfall, 30s wind/storm)
Duration checks: Every WeatherCheckInterval (default 30s)
Status logs: Every LogInterval (default 60s)
```

### **Duration Elapsed**
```
Time: 900s
Check: elapsedTime (900s) >= m_MinDuration (900s) ✅
Action: Trigger new preset selection
```

### **New Selection**
```
Time: 900s
Action: Weighted selection → "foggy" selected (rolled 78)
Generated Values:
  - Overcast: 0.65 (from 0.5-0.8 range)
  - VolFogDistanceDensity: 0.6 (from 0.4-0.8 range)
  - TransitionTime: 270s (from 180-360 range)
  - MinDuration: 1350s (from 900-1800 range)
```

### **Transition Period**
```
Time: 900s - 1170s (270s transition)
Status: m_InTransition = true
Progress: Smooth interpolation from old to new values
  - Overcast: 0.35 → 0.65 (over 270s)
  - Fog: 0.05 → 0.0 (over 270s)
Updates: Every 1s (intermediate values applied)
```

### **Transition Complete**
```
Time: 1170s
Status: m_InTransition = false
Action: Lock weather at target values
m_PresetApplyTime: 1170s (duration starts now)
```

### **New Duration Period**
```
Time: 1170s - 2520s (1350s duration)
Status: Weather locked at foggy preset values
Reinforcement: Tiered (5s/3s/30s intervals)
Duration checks: Every WeatherCheckInterval (default 30s)
```

### **Cycle Repeats**
```
Time: 2520s
Action: Duration elapsed → Select new preset → Repeat cycle
```

---

## 🎯 **KEY CONCEPTS**

### **1. Weighted Random Selection**
- Not equal probability for all presets
- Higher `m_WeatherChance` = more frequent selection
- System uses cumulative weight ranges for selection

### **2. Two-Stage Timing**
- **Transition Time:** Smooth change period
- **Min Duration:** Stable weather period
- **Total cycle time = TransitionTime + MinDuration**

### **3. Value Generation**
- Each preset defines **ranges** (Min-Max)
- System generates **random values** within those ranges
- Every application uses different random values (variety)

### **4. Tiered Weather Reinforcement**
- During duration, system reinforces values at tiered intervals:
  - Fast (5s): Overcast, fog, rain
  - Aggressive (3s): Snowfall (prevents engine drift)
  - Slow (30s): Wind, storm, thresholds, volumetric fog
- Prevents DayZ from overriding weather (~88% fewer API calls than per-frame)

### **5. Transition Guard**
- While `m_InTransition = true`, duration checks are skipped entirely
- Prevents rapid-fire preset changes during an active transition
- New preset selection only happens after transition completes and duration expires

### **6. Same Preset Selection**
- If same preset selected again:
  - System extends duration (doesn't re-transition)
  - Generates new random values from preset ranges
  - Applies immediately (no transition needed)

---

## 📝 **CONFIGURATION TIPS**

### **Balancing `m_WeatherChance`**
- **Common weather:** Higher values (20-30)
- **Rare weather:** Lower values (5-10)
- **Total doesn't matter:** System normalizes automatically

### **Setting `m_TransitionTime`**
- **Smooth changes:** 180-360 seconds (3-6 minutes)
- **Quick changes:** 60-120 seconds (1-2 minutes)
- **Dramatic changes:** 30-60 seconds

### **Setting `m_MinDuration`**
- **Stable weather:** 1200-1800 seconds (20-30 minutes)
- **Varied weather:** 300-600 seconds (5-10 minutes)
- **Dynamic weather:** 180-300 seconds (3-5 minutes)

### **Example: Realistic Weather Cycle**
```
clear: weight 30, 240s transition, 1200s duration
partly_cloudy: weight 25, 300s transition, 900s duration
overcast: weight 20, 180s transition, 600s duration
rain: weight 15, 120s transition, 600s duration
stormy: weight 5, 90s transition, 300s duration
foggy: weight 3, 360s transition, 1800s duration
snowy: weight 2, 300s transition, 1200s duration
```

---

## 🔍 **DEBUGGING**

### **Check Logs For:**
- `[AUTO WEATHER] Weighted selection - rolled: X / Y`
- `[AUTO WEATHER] Selected preset 'X' (chance: Y)`
- `[AUTO WEATHER] MinDuration not elapsed: Xs / Ys`
- `[AUTO WEATHER] Preset duration elapsed (Xs >= Ys)`
- `[WEATHER TRANSITION PROGRESS] X% complete`
- `[WEATHER TRANSITION PROGRESS] Transition complete - final values applied`

**Note:** Log timestamps use real-world server time (not in-game time).

### **Common Issues:**
1. **Weather changes too fast:** Increase `m_MinDuration_Min/Max`
2. **Weather never changes:** Check `m_WeatherChance` > 0
3. **Transitions too abrupt:** Increase `m_TransitionTime_Min/Max`
4. **Same preset always selected:** Check `m_WeatherChance` distribution (values are relative weights)
5. **Too many log entries:** Increase `LogInterval` in `WU_Settings.json`
6. **Duration checks too infrequent:** Decrease `WeatherCheckInterval` in `WU_Settings.json`

---

**End of Explanation**
