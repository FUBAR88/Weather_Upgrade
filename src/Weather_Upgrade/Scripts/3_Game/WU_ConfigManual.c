class WU_ConfigManual
{
    static const string CONFIG_FILE = "$profile:\\Weather_Upgrade\\WU_ManualWeather.json";
    
    string DefaultWeatherPreset;
    int WeatherCheckInterval;
    ref map<string, ref WU_WeatherPreset> WeatherPresets;
    ref array<ref WU_WeatherSchedule> WeatherSchedule;
    
    void WU_ConfigManual()
    {
        DefaultWeatherPreset = "clear";
        WeatherCheckInterval = 60;
        WeatherPresets = new map<string, ref WU_WeatherPreset>();
        WeatherSchedule = new array<ref WU_WeatherSchedule>();
    }
    
    bool Load()
    {
        if (!FileExist(CONFIG_FILE))
        {
            WU_Logger.Warning("[MANUAL CONFIG] Configuration file not found: " + CONFIG_FILE);
            return false;
        }
        
        JsonFileLoader<WU_ConfigManual>.JsonLoadFile(CONFIG_FILE, this);
        
        if (!WeatherPresets || WeatherPresets.Count() == 0)
        {
            WU_Logger.Error("[MANUAL CONFIG] No weather presets loaded");
            return false;
        }
        
        WU_Logger.Info("[MANUAL CONFIG] Manual weather configuration loaded");
        WU_Logger.Info("[MANUAL CONFIG] - DefaultWeatherPreset: " + DefaultWeatherPreset);
        WU_Logger.Info("[MANUAL CONFIG] - WeatherCheckInterval: " + WeatherCheckInterval.ToString() + "s");
        WU_Logger.Info("[MANUAL CONFIG] - Presets Loaded: " + WeatherPresets.Count().ToString());
        WU_Logger.Info("[MANUAL CONFIG] - Schedule Entries: " + WeatherSchedule.Count().ToString());
        
        for (int i = 0; i < WeatherPresets.Count(); i++)
        {
            string presetName = WeatherPresets.GetKey(i);
            ref WU_WeatherPreset preset = WeatherPresets.GetElement(i);
            
            if (preset && preset.IsValid())
            {
                WU_Logger.Info("[MANUAL CONFIG] Validated preset: " + presetName);
            }
            else
            {
                WU_Logger.Error("[MANUAL CONFIG] Invalid preset: " + presetName);
            }
        }
        
        if (WeatherSchedule)
        {
            for (int j = 0; j < WeatherSchedule.Count(); j++)
            {
                ref WU_WeatherSchedule schedule = WeatherSchedule.Get(j);
                if (schedule && schedule.IsValid())
                {
                    WU_Logger.Info("[MANUAL CONFIG] Schedule: " + schedule.Time + " -> " + schedule.Preset + " (" + schedule.Chance.ToString() + "%)");
                }
            }
        }
        
        return true;
    }
    
    void CreateDefault()
    {
        WU_Logger.Info("[MANUAL CONFIG] Generating default manual weather config with 7 presets");
        
        FileHandle file = OpenFile(CONFIG_FILE, FileMode.WRITE);
        if (file == 0)
        {
            WU_Logger.Error("[MANUAL CONFIG] Failed to create config file");
            return;
        }
        
        FPrint(file, "{\n");
        FPrint(file, "    \"DefaultWeatherPreset\": \"clear\",\n");
        FPrint(file, "    \"WeatherCheckInterval\": 60,\n");
        FPrint(file, "    \"WeatherPresets\": {\n");
        
        WritePresetClear(file);
        FPrint(file, ",\n");
        WritePresetPartlyCloudy(file);
        FPrint(file, ",\n");
        WritePresetOvercast(file);
        FPrint(file, ",\n");
        WritePresetRain(file);
        FPrint(file, ",\n");
        WritePresetStormy(file);
        FPrint(file, ",\n");
        WritePresetFoggy(file);
        FPrint(file, ",\n");
        WritePresetSnowy(file);
        
        FPrint(file, "\n    },\n");
        FPrint(file, "    \"WeatherSchedule\": [\n");
        FPrint(file, "        {\n");
        FPrint(file, "            \"Time\": \"00:00\",\n");
        FPrint(file, "            \"Preset\": \"clear\",\n");
        FPrint(file, "            \"Chance\": 100\n");
        FPrint(file, "        },\n");
        FPrint(file, "        {\n");
        FPrint(file, "            \"Time\": \"06:00\",\n");
        FPrint(file, "            \"Preset\": \"partly_cloudy\",\n");
        FPrint(file, "            \"Chance\": 100\n");
        FPrint(file, "        },\n");
        FPrint(file, "        {\n");
        FPrint(file, "            \"Time\": \"12:00\",\n");
        FPrint(file, "            \"Preset\": \"overcast\",\n");
        FPrint(file, "            \"Chance\": 100\n");
        FPrint(file, "        },\n");
        FPrint(file, "        {\n");
        FPrint(file, "            \"Time\": \"14:00\",\n");
        FPrint(file, "            \"Preset\": \"rain\",\n");
        FPrint(file, "            \"Chance\": 100\n");
        FPrint(file, "        },\n");
        FPrint(file, "        {\n");
        FPrint(file, "            \"Time\": \"16:00\",\n");
        FPrint(file, "            \"Preset\": \"overcast\",\n");
        FPrint(file, "            \"Chance\": 100\n");
        FPrint(file, "        },\n");
        FPrint(file, "        {\n");
        FPrint(file, "            \"Time\": \"18:00\",\n");
        FPrint(file, "            \"Preset\": \"clear\",\n");
        FPrint(file, "            \"Chance\": 100\n");
        FPrint(file, "        }\n");
        FPrint(file, "    ]\n");
        FPrint(file, "}\n");
        CloseFile(file);
        
        WU_Logger.Info("[MANUAL CONFIG] Default manual weather config created with 7 presets");
    }
    
    private void WritePresetClear(FileHandle file)
    {
        FPrint(file, "        \"clear\": {\n");
        FPrint(file, "            \"m_TransitionTime_Min\": 120,\n");
        FPrint(file, "            \"m_TransitionTime_Max\": 180,\n");
        FPrint(file, "            \"m_MinDuration_Min\": 600,\n");
        FPrint(file, "            \"m_MinDuration_Max\": 1200,\n");
        FPrint(file, "            \"m_Overcast_Min\": 0,\n");
        FPrint(file, "            \"m_Overcast_Max\": 0.2,\n");
        FPrint(file, "            \"m_Fog_Min\": 0,\n");
        FPrint(file, "            \"m_Fog_Max\": 0,\n");
        FPrint(file, "            \"m_Rain_Min\": 0,\n");
        FPrint(file, "            \"m_Rain_Max\": 0,\n");
        FPrint(file, "            \"m_Snowfall_Min\": 0,\n");
        FPrint(file, "            \"m_Snowfall_Max\": 0,\n");
        FPrint(file, "            \"m_WindSpeed_Min\": 0.5,\n");
        FPrint(file, "            \"m_WindSpeed_Max\": 2,\n");
        FPrint(file, "            \"m_WindDirection_Min\": 0,\n");
        FPrint(file, "            \"m_WindDirection_Max\": 360,\n");
        FPrint(file, "            \"m_WindFunc_Min\": 0.1,\n");
        FPrint(file, "            \"m_WindFunc_Max\": 1.0,\n");
        FPrint(file, "            \"m_WindFunc_Speed\": 20.0,\n");
        FPrint(file, "            \"m_UseDynVolFog\": 0,\n");
        FPrint(file, "            \"m_VolFogDistanceDensity_Min\": 0,\n");
        FPrint(file, "            \"m_VolFogDistanceDensity_Max\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightDensity_Min\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightDensity_Max\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightBias\": 0,\n");
        FPrint(file, "            \"m_FogTransitionTime\": 30,\n");
        FPrint(file, "            \"m_UseSnowflakeScale\": 0,\n");
        FPrint(file, "            \"m_SnowflakeScale_Min\": 1,\n");
        FPrint(file, "            \"m_SnowflakeScale_Max\": 1,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Min\": 0.6,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Max\": 1,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Timeout\": 30,\n");
        FPrint(file, "            \"m_RainThreshold_Min\": 0.3,\n");
        FPrint(file, "            \"m_RainThreshold_Max\": 1.0,\n");
        FPrint(file, "            \"m_RainThreshold_Timeout\": 30,\n");
        FPrint(file, "            \"m_ThunderstormDensity\": 0,\n");
        FPrint(file, "            \"m_ThunderstormThreshold\": 0.8,\n");
        FPrint(file, "            \"m_LightningTimeout\": 45,\n");
        FPrint(file, "            \"m_UseEnvironmentTemperature\": 0,\n");
        FPrint(file, "            \"m_EnvironmentTemperature\": 15\n");
        FPrint(file, "        }");
    }
    
    private void WritePresetPartlyCloudy(FileHandle file)
    {
        FPrint(file, "        \"partly_cloudy\": {\n");
        FPrint(file, "            \"m_TransitionTime_Min\": 180,\n");
        FPrint(file, "            \"m_TransitionTime_Max\": 300,\n");
        FPrint(file, "            \"m_MinDuration_Min\": 600,\n");
        FPrint(file, "            \"m_MinDuration_Max\": 1200,\n");
        FPrint(file, "            \"m_Overcast_Min\": 0.2,\n");
        FPrint(file, "            \"m_Overcast_Max\": 0.5,\n");
        FPrint(file, "            \"m_Fog_Min\": 0,\n");
        FPrint(file, "            \"m_Fog_Max\": 0.1,\n");
        FPrint(file, "            \"m_Rain_Min\": 0,\n");
        FPrint(file, "            \"m_Rain_Max\": 0,\n");
        FPrint(file, "            \"m_Snowfall_Min\": 0,\n");
        FPrint(file, "            \"m_Snowfall_Max\": 0,\n");
        FPrint(file, "            \"m_WindSpeed_Min\": 1,\n");
        FPrint(file, "            \"m_WindSpeed_Max\": 4,\n");
        FPrint(file, "            \"m_WindDirection_Min\": 0,\n");
        FPrint(file, "            \"m_WindDirection_Max\": 360,\n");
        FPrint(file, "            \"m_WindFunc_Min\": 0.1,\n");
        FPrint(file, "            \"m_WindFunc_Max\": 1.0,\n");
        FPrint(file, "            \"m_WindFunc_Speed\": 20.0,\n");
        FPrint(file, "            \"m_UseDynVolFog\": 0,\n");
        FPrint(file, "            \"m_VolFogDistanceDensity_Min\": 0,\n");
        FPrint(file, "            \"m_VolFogDistanceDensity_Max\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightDensity_Min\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightDensity_Max\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightBias\": 0,\n");
        FPrint(file, "            \"m_FogTransitionTime\": 30,\n");
        FPrint(file, "            \"m_UseSnowflakeScale\": 0,\n");
        FPrint(file, "            \"m_SnowflakeScale_Min\": 1,\n");
        FPrint(file, "            \"m_SnowflakeScale_Max\": 1,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Min\": 0.6,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Max\": 1,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Timeout\": 30,\n");
        FPrint(file, "            \"m_ThunderstormDensity\": 0.1,\n");
        FPrint(file, "            \"m_ThunderstormThreshold\": 0.85,\n");
        FPrint(file, "            \"m_LightningTimeout\": 60,\n");
        FPrint(file, "            \"m_UseEnvironmentTemperature\": 0,\n");
        FPrint(file, "            \"m_EnvironmentTemperature\": 18\n");
        FPrint(file, "        }");
    }
    
    private void WritePresetOvercast(FileHandle file)
    {
        FPrint(file, "        \"overcast\": {\n");
        FPrint(file, "            \"m_TransitionTime_Min\": 120,\n");
        FPrint(file, "            \"m_TransitionTime_Max\": 240,\n");
        FPrint(file, "            \"m_MinDuration_Min\": 600,\n");
        FPrint(file, "            \"m_MinDuration_Max\": 1200,\n");
        FPrint(file, "            \"m_Overcast_Min\": 0.7,\n");
        FPrint(file, "            \"m_Overcast_Max\": 0.9,\n");
        FPrint(file, "            \"m_Fog_Min\": 0,\n");
        FPrint(file, "            \"m_Fog_Max\": 0,\n");
        FPrint(file, "            \"m_Rain_Min\": 0,\n");
        FPrint(file, "            \"m_Rain_Max\": 0,\n");
        FPrint(file, "            \"m_Snowfall_Min\": 0,\n");
        FPrint(file, "            \"m_Snowfall_Max\": 0,\n");
        FPrint(file, "            \"m_WindSpeed_Min\": 2,\n");
        FPrint(file, "            \"m_WindSpeed_Max\": 5,\n");
        FPrint(file, "            \"m_WindDirection_Min\": 0,\n");
        FPrint(file, "            \"m_WindDirection_Max\": 360,\n");
        FPrint(file, "            \"m_WindFunc_Min\": 0.1,\n");
        FPrint(file, "            \"m_WindFunc_Max\": 1.0,\n");
        FPrint(file, "            \"m_WindFunc_Speed\": 20.0,\n");
        FPrint(file, "            \"m_UseDynVolFog\": 0,\n");
        FPrint(file, "            \"m_VolFogDistanceDensity_Min\": 0,\n");
        FPrint(file, "            \"m_VolFogDistanceDensity_Max\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightDensity_Min\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightDensity_Max\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightBias\": 0,\n");
        FPrint(file, "            \"m_FogTransitionTime\": 30,\n");
        FPrint(file, "            \"m_UseSnowflakeScale\": 0,\n");
        FPrint(file, "            \"m_SnowflakeScale_Min\": 1,\n");
        FPrint(file, "            \"m_SnowflakeScale_Max\": 1,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Min\": 0.6,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Max\": 1,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Timeout\": 30,\n");
        FPrint(file, "            \"m_RainThreshold_Min\": 0.3,\n");
        FPrint(file, "            \"m_RainThreshold_Max\": 1.0,\n");
        FPrint(file, "            \"m_RainThreshold_Timeout\": 30,\n");
        FPrint(file, "            \"m_ThunderstormDensity\": 0,\n");
        FPrint(file, "            \"m_ThunderstormThreshold\": 0.8,\n");
        FPrint(file, "            \"m_LightningTimeout\": 45,\n");
        FPrint(file, "            \"m_UseEnvironmentTemperature\": 0,\n");
        FPrint(file, "            \"m_EnvironmentTemperature\": 16\n");
        FPrint(file, "        }");
    }
    
    private void WritePresetRain(FileHandle file)
    {
        FPrint(file, "        \"rain\": {\n");
        FPrint(file, "            \"m_TransitionTime_Min\": 90,\n");
        FPrint(file, "            \"m_TransitionTime_Max\": 180,\n");
        FPrint(file, "            \"m_MinDuration_Min\": 300,\n");
        FPrint(file, "            \"m_MinDuration_Max\": 900,\n");
        FPrint(file, "            \"m_Overcast_Min\": 0.8,\n");
        FPrint(file, "            \"m_Overcast_Max\": 1,\n");
        FPrint(file, "            \"m_Fog_Min\": 0,\n");
        FPrint(file, "            \"m_Fog_Max\": 0.2,\n");
        FPrint(file, "            \"m_Rain_Min\": 0.5,\n");
        FPrint(file, "            \"m_Rain_Max\": 0.8,\n");
        FPrint(file, "            \"m_Snowfall_Min\": 0,\n");
        FPrint(file, "            \"m_Snowfall_Max\": 0,\n");
        FPrint(file, "            \"m_WindSpeed_Min\": 3,\n");
        FPrint(file, "            \"m_WindSpeed_Max\": 7,\n");
        FPrint(file, "            \"m_WindDirection_Min\": 0,\n");
        FPrint(file, "            \"m_WindDirection_Max\": 360,\n");
        FPrint(file, "            \"m_WindFunc_Min\": 0.1,\n");
        FPrint(file, "            \"m_WindFunc_Max\": 1.0,\n");
        FPrint(file, "            \"m_WindFunc_Speed\": 20.0,\n");
        FPrint(file, "            \"m_UseDynVolFog\": 0,\n");
        FPrint(file, "            \"m_VolFogDistanceDensity_Min\": 0,\n");
        FPrint(file, "            \"m_VolFogDistanceDensity_Max\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightDensity_Min\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightDensity_Max\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightBias\": 0,\n");
        FPrint(file, "            \"m_FogTransitionTime\": 30,\n");
        FPrint(file, "            \"m_UseSnowflakeScale\": 0,\n");
        FPrint(file, "            \"m_SnowflakeScale_Min\": 1,\n");
        FPrint(file, "            \"m_SnowflakeScale_Max\": 1,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Min\": 0.6,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Max\": 1,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Timeout\": 30,\n");
        FPrint(file, "            \"m_ThunderstormDensity\": 0.3,\n");
        FPrint(file, "            \"m_ThunderstormThreshold\": 0.8,\n");
        FPrint(file, "            \"m_LightningTimeout\": 30,\n");
        FPrint(file, "            \"m_UseEnvironmentTemperature\": 0,\n");
        FPrint(file, "            \"m_EnvironmentTemperature\": 18\n");
        FPrint(file, "        }");
    }
    
    private void WritePresetStormy(FileHandle file)
    {
        FPrint(file, "        \"stormy\": {\n");
        FPrint(file, "            \"m_TransitionTime_Min\": 60,\n");
        FPrint(file, "            \"m_TransitionTime_Max\": 180,\n");
        FPrint(file, "            \"m_MinDuration_Min\": 300,\n");
        FPrint(file, "            \"m_MinDuration_Max\": 900,\n");
        FPrint(file, "            \"m_Overcast_Min\": 0.85,\n");
        FPrint(file, "            \"m_Overcast_Max\": 1,\n");
        FPrint(file, "            \"m_Fog_Min\": 0,\n");
        FPrint(file, "            \"m_Fog_Max\": 0.3,\n");
        FPrint(file, "            \"m_Rain_Min\": 0.6,\n");
        FPrint(file, "            \"m_Rain_Max\": 1,\n");
        FPrint(file, "            \"m_Snowfall_Min\": 0,\n");
        FPrint(file, "            \"m_Snowfall_Max\": 0,\n");
        FPrint(file, "            \"m_WindSpeed_Min\": 8,\n");
        FPrint(file, "            \"m_WindSpeed_Max\": 15,\n");
        FPrint(file, "            \"m_WindDirection_Min\": 0,\n");
        FPrint(file, "            \"m_WindDirection_Max\": 360,\n");
        FPrint(file, "            \"m_WindFunc_Min\": 0.1,\n");
        FPrint(file, "            \"m_WindFunc_Max\": 1.0,\n");
        FPrint(file, "            \"m_WindFunc_Speed\": 20.0,\n");
        FPrint(file, "            \"m_UseDynVolFog\": 0,\n");
        FPrint(file, "            \"m_VolFogDistanceDensity_Min\": 0.1,\n");
        FPrint(file, "            \"m_VolFogDistanceDensity_Max\": 0.3,\n");
        FPrint(file, "            \"m_VolFogHeightDensity_Min\": 0.1,\n");
        FPrint(file, "            \"m_VolFogHeightDensity_Max\": 0.3,\n");
        FPrint(file, "            \"m_VolFogHeightBias\": 20,\n");
        FPrint(file, "            \"m_FogTransitionTime\": 40,\n");
        FPrint(file, "            \"m_UseSnowflakeScale\": 0,\n");
        FPrint(file, "            \"m_SnowflakeScale_Min\": 1,\n");
        FPrint(file, "            \"m_SnowflakeScale_Max\": 1,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Min\": 0.6,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Max\": 1,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Timeout\": 30,\n");
        FPrint(file, "            \"m_ThunderstormDensity\": 0.7,\n");
        FPrint(file, "            \"m_ThunderstormThreshold\": 0.7,\n");
        FPrint(file, "            \"m_LightningTimeout\": 20,\n");
        FPrint(file, "            \"m_UseEnvironmentTemperature\": 0,\n");
        FPrint(file, "            \"m_EnvironmentTemperature\": 22\n");
        FPrint(file, "        }");
    }
    
    private void WritePresetFoggy(FileHandle file)
    {
        FPrint(file, "        \"foggy\": {\n");
        FPrint(file, "            \"m_TransitionTime_Min\": 180,\n");
        FPrint(file, "            \"m_TransitionTime_Max\": 360,\n");
        FPrint(file, "            \"m_MinDuration_Min\": 900,\n");
        FPrint(file, "            \"m_MinDuration_Max\": 1800,\n");
        FPrint(file, "            \"m_Overcast_Min\": 0.5,\n");
        FPrint(file, "            \"m_Overcast_Max\": 0.8,\n");
        FPrint(file, "            \"m_Fog_Min\": 0,\n");
        FPrint(file, "            \"m_Fog_Max\": 0,\n");
        FPrint(file, "            \"m_Rain_Min\": 0,\n");
        FPrint(file, "            \"m_Rain_Max\": 0,\n");
        FPrint(file, "            \"m_Snowfall_Min\": 0,\n");
        FPrint(file, "            \"m_Snowfall_Max\": 0,\n");
        FPrint(file, "            \"m_WindSpeed_Min\": 0.3,\n");
        FPrint(file, "            \"m_WindSpeed_Max\": 1.5,\n");
        FPrint(file, "            \"m_WindDirection_Min\": 0,\n");
        FPrint(file, "            \"m_WindDirection_Max\": 360,\n");
        FPrint(file, "            \"m_UseDynVolFog\": 1,\n");
        FPrint(file, "            \"m_VolFogDistanceDensity_Min\": 0.4,\n");
        FPrint(file, "            \"m_VolFogDistanceDensity_Max\": 0.8,\n");
        FPrint(file, "            \"m_VolFogHeightDensity_Min\": 0.3,\n");
        FPrint(file, "            \"m_VolFogHeightDensity_Max\": 0.7,\n");
        FPrint(file, "            \"m_VolFogHeightBias\": 0,\n");
        FPrint(file, "            \"m_FogTransitionTime\": 60,\n");
        FPrint(file, "            \"m_UseSnowflakeScale\": 0,\n");
        FPrint(file, "            \"m_SnowflakeScale_Min\": 1,\n");
        FPrint(file, "            \"m_SnowflakeScale_Max\": 1,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Min\": 0.6,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Max\": 1,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Timeout\": 30,\n");
        FPrint(file, "            \"m_RainThreshold_Min\": 0.3,\n");
        FPrint(file, "            \"m_RainThreshold_Max\": 1.0,\n");
        FPrint(file, "            \"m_RainThreshold_Timeout\": 30,\n");
        FPrint(file, "            \"m_ThunderstormDensity\": 0,\n");
        FPrint(file, "            \"m_ThunderstormThreshold\": 0.8,\n");
        FPrint(file, "            \"m_LightningTimeout\": 45,\n");
        FPrint(file, "            \"m_UseEnvironmentTemperature\": 0,\n");
        FPrint(file, "            \"m_EnvironmentTemperature\": 14\n");
        FPrint(file, "        }");
    }
    
    private void WritePresetSnowy(FileHandle file)
    {
        FPrint(file, "        \"snowy\": {\n");
        FPrint(file, "            \"m_TransitionTime_Min\": 180,\n");
        FPrint(file, "            \"m_TransitionTime_Max\": 360,\n");
        FPrint(file, "            \"m_MinDuration_Min\": 600,\n");
        FPrint(file, "            \"m_MinDuration_Max\": 1200,\n");
        FPrint(file, "            \"m_Overcast_Min\": 0.6,\n");
        FPrint(file, "            \"m_Overcast_Max\": 0.8,\n");
        FPrint(file, "            \"m_Fog_Min\": 0,\n");
        FPrint(file, "            \"m_Fog_Max\": 0,\n");
        FPrint(file, "            \"m_Rain_Min\": 0,\n");
        FPrint(file, "            \"m_Rain_Max\": 0,\n");
        FPrint(file, "            \"m_Snowfall_Min\": 0.4,\n");
        FPrint(file, "            \"m_Snowfall_Max\": 0.8,\n");
        FPrint(file, "            \"m_WindSpeed_Min\": 2,\n");
        FPrint(file, "            \"m_WindSpeed_Max\": 6,\n");
        FPrint(file, "            \"m_WindDirection_Min\": 0,\n");
        FPrint(file, "            \"m_WindDirection_Max\": 360,\n");
        FPrint(file, "            \"m_WindFunc_Min\": 0.1,\n");
        FPrint(file, "            \"m_WindFunc_Max\": 1.0,\n");
        FPrint(file, "            \"m_WindFunc_Speed\": 20.0,\n");
        FPrint(file, "            \"m_UseDynVolFog\": 0,\n");
        FPrint(file, "            \"m_VolFogDistanceDensity_Min\": 0,\n");
        FPrint(file, "            \"m_VolFogDistanceDensity_Max\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightDensity_Min\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightDensity_Max\": 0,\n");
        FPrint(file, "            \"m_VolFogHeightBias\": 0,\n");
        FPrint(file, "            \"m_FogTransitionTime\": 30,\n");
        FPrint(file, "            \"m_UseSnowflakeScale\": 1,\n");
        FPrint(file, "            \"m_SnowflakeScale_Min\": 1,\n");
        FPrint(file, "            \"m_SnowflakeScale_Max\": 1.5,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Min\": 0.5,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Max\": 1,\n");
        FPrint(file, "            \"m_SnowfallThreshold_Timeout\": 20,\n");
        FPrint(file, "            \"m_ThunderstormDensity\": 0,\n");
        FPrint(file, "            \"m_ThunderstormThreshold\": 0.8,\n");
        FPrint(file, "            \"m_LightningTimeout\": 45,\n");
        FPrint(file, "            \"m_UseEnvironmentTemperature\": 1,\n");
        FPrint(file, "            \"m_EnvironmentTemperature\": -6\n");
        FPrint(file, "        }");
    }
};