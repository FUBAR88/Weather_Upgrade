[img]https://i.ibb.co/sJRCGd3Y/1-REQUIREMENTS.png[/img]
[b]Requires[/b]: [url=https://steamcommunity.com/sharedfiles/filedetails/?id=1559212036]Community Framework[/url]

[img]https://i.ibb.co/fVKCqFpw/2-DESCRIPTION.png[/img]
Complete server-side weather control system for DayZ with automatic/manual scheduling, temperature zones, and advanced weather features. Take full control over your server's weather patterns with smooth transitions, precise temperature control, and location-based zones.

[img]https://i.ibb.co/sJqGWyrZ/3-KEY-FEATURES.png[/img]
[list]
[*][b]Two Weather Modes[/b] - Auto Mode (random weather changes) or Manual Mode (time-based scheduled weather)
[*][b]Temperature Control[/b] - Global temperature override per preset + location-based temperature zones with height support
[*][b]Smooth Transitions[/b] - All weather parameters transition smoothly without snapping or glitches (rain, fog, overcast, snow)
[*][b]Advanced Weather Features[/b] - Volumetric fog, wind function control, rain/snow thresholds, thunderstorms, lightning, snowflake customization
[*][b]Fire Heat Preservation[/b] - Fires always work correctly, even with extreme temperature overrides (-30°C)
[*][b]Auto-Generated Configs[/b] - JSON-based configuration files auto-generate on first run - easy to edit
[*][b]COT Compatible[/b] - Works seamlessly with Community Offline Tools weather panel
[*][b]Performance Optimized[/b] - Uses MissionWeather API for precise server-side control with minimal overhead
[*][b]Comprehensive Logging[/b] - Production logs + optional debug logs for troubleshooting
[*][b]Works on All Maps[/b] - Compatible with Chernarus, Livonia, Deer Isle, Namalsk, and all custom maps
[/list]

[img]https://i.ibb.co/Rpr5P6w5/4-CONFIGURATION.png[/img]
All settings managed by JSON configuration files that auto-generate in [code]profiles/Weather_Upgrade/[/code]

[b]Main Configuration Files:[/b]
[list]
[*][code]WU_Settings.json[/code] - Master settings (weather mode, temperature control, logging)
[*][code]WU_AutoWeather.json[/code] - Auto/Random mode configuration
[*][code]WU_ManualWeather.json[/code] - Manual/Schedule mode configuration
[*][code]WU_ZoneTemperatureControl.json[/code] - Location-based temperature zones (optional)
[/list]

[b]Quick Start Examples:[/b]

[b]Auto Mode (Random Weather):[/b]
[code]
{
    "AutoWeatherChanges": 1,
    "DefaultWeatherPreset": "clear"
}
[/code]
Weather always changes when preset duration expires. Selection is weighted by `m_WeatherChance` values in each preset (higher = more likely).

[b]Manual Mode (Scheduled Weather):[/b]
[code]
{
    "WeatherPresets": {
        "clear": { ... },
        "rain": { ... },
        "stormy": { ... }
    },
    "WeatherSchedule": [
        { "Time": "06:00", "Preset": "clear", "Chance": 100 },
        { "Time": "14:00", "Preset": "rain", "Chance": 70 },
        { "Time": "18:00", "Preset": "stormy", "Chance": 50 }
    ]
}
[/code]
Rain at 14:00 (70% chance), storm at 18:00 (50% chance). Presets must be defined in WeatherPresets.

[b]Temperature Override:[/b]
[code]
{
    "m_UseEnvironmentTemperature": 1,
    "m_EnvironmentTemperature": -6
}
[/code]
Set temperature to -6°C for snow presets.

[b]Full Documentation:[/b]
[url=https://github.com/FUBAR88/Weather_Upgrade]GitHub Repository[/url] - Complete settings reference, installation guide, and examples

[img]https://i.ibb.co/9mG2Yj93/5-SUPPORT.png[/img]
For any information and help Please join the Discord

[img]https://i.ibb.co/x8gJvBL8/Discord.png[/img]
For any information and help Please join the Discord [url=https://discord.gg/FJa82GVfEa]ⒻⓊⒷⒶⓇ'Ⓢ ⒽⒾⒹⒺⓄⓊⓉ[/url]

[img]https://i.ibb.co/rGj6WqBp/6-NOTES.png[/img]
[b]Version:[/b] 2.3.8

[b]Latest Updates (v2.3.8):[/b]
[list]
[*]✅ MAJOR IMPROVEMENT: Smooth snowfall transitions (snowfall no longer stops/resets)
[*]✅ Snowfall reinforcement system prevents DayZ override
[*]✅ All weather parameters now transition smoothly without snapping
[*]✅ Snowfall transitions now match rain transition behavior exactly
[/list]

[b]Previous Updates (v2.3.6):[/b]
[list]
[*]✅ CRITICAL FIX: Fire heat now works correctly with temperature override
[*]✅ Players receive heat buffs from fires even with extreme cold overrides
[*]✅ Temperature override correctly applies when no fire heat is present
[/list]

[b]Previous Updates (v2.3.5):[/b]
[list]
[*]✅ Smooth rain transitions (rain no longer stops/resets)
[*]✅ Rain reinforcement system prevents DayZ override
[*]✅ Enhanced transition logging and progress tracking
[*]✅ Fixed transition values not applying in Manual mode
[/list]

[b]Installation:[/b]
[list]
[1] Download and extract the mod
[2] Copy [code]@Weather_Upgrade[/code] folder to your server root
[3] Add [code]-mod=@Weather_Upgrade[/code] to server start parameters
[4] Start server - configs auto-generate in [code]profiles/Weather_Upgrade/[/code]
[5] Edit JSON configs to your preference
[6] Restart server
[/list]

[b]Compatibility:[/b]
[list]
[*]DayZ Server 1.25+ (tested up to 1.28+)
[*]Requires Community Framework
[*]Compatible with COT (Community Offline Tools)
[*]Compatible with AdminTools and other weather mods (use DisableWeatherUpgrade setting)
[*]Works on all official and custom maps
[/list]

[b]⚠️ NO REPACKING ALLOWED[/b]

[b]GitHub:[/b] [url=https://github.com/FUBAR88/Weather_Upgrade]https://github.com/FUBAR88/Weather_Upgrade[/url]
