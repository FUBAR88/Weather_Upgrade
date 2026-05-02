# Weather_Upgrade: How m_WeatherChance Actually Works

## Your Example:
```
Rain: 60%
Fog: 40%
Clear: 70%
Total: 170%
```

## How Current Implementation Works:

### Step 1: Calculate Total Weight
```
Total = 60 + 40 + 70 = 170
```

### Step 2: Create Cumulative Ranges
```
Clear:  0  - 70  (range: 70)
Rain:   70 - 130 (range: 60)
Fog:    130 - 170 (range: 40)
```

### Step 3: Roll Random Number
```
Roll: RandomInt(1, 171)  // DayZ RandomInt is max-exclusive, so 1 to 170 inclusive
Example roll: 85
```

### Step 4: Find Which Range Contains Roll
```
85 falls in Rain's range (70-130)
Result: Rain selected
```

## Actual Selection Probabilities:

**With your values:**
- **Clear: 70/170 = 41.2%** chance
- **Rain: 60/170 = 35.3%** chance  
- **Fog: 40/170 = 23.5%** chance

## Does This Match Your Expectation?

**You said:** "more clear than rain, more rain than fog"

**Result:** ✅ **YES!**
- Clear (41.2%) > Rain (35.3%) > Fog (23.5%)

**The relative order is correct**, but the actual percentages are normalized to total 100%.

## How to Get Specific Percentages:

If you want **exact percentages**, set values that add to 100:

```
Clear: 50  → 50% chance
Rain:  30  → 30% chance
Fog:   20  → 20% chance
Total: 100
```

Or use proportional values:
```
Clear: 70  → 70/170 = 41.2%
Rain:  60  → 60/170 = 35.3%
Fog:   40  → 40/170 = 23.5%
```

## Key Point:

**Higher `m_WeatherChance` = More likely to be selected**

The system uses **weighted random selection** - values are relative weights, not absolute percentages. The actual probability is calculated as: `preset_chance / total_all_chances`

**Your example works correctly** - Clear will be selected most often, Rain second, Fog least.
