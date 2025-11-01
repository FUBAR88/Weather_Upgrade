class WU_ConfigManager
{
    private ref WU_Settings m_Settings;
    private ref WU_ConfigAuto m_AutoConfig;
    private ref WU_ConfigManual m_ManualConfig;
    
    void WU_ConfigManager()
    {
        m_Settings = new WU_Settings();
        m_AutoConfig = new WU_ConfigAuto();
        m_ManualConfig = new WU_ConfigManual();
    }
    
    bool LoadConfig()
    {
        if (!m_Settings.Load())
        {
            WU_Logger.Error("[CONFIG MANAGER] Failed to load master settings");
            return false;
        }
        
        if (m_Settings.AutoWeatherChanges == 1)
        {
            if (!m_AutoConfig.Load())
            {
                WU_Logger.Error("[CONFIG MANAGER] Failed to load auto weather config");
            return false;
        }
            WU_Logger.Info("[CONFIG MANAGER] Auto weather mode enabled");
            return true;
        }
        else
        {
            if (!m_ManualConfig.Load())
            {
                WU_Logger.Error("[CONFIG MANAGER] Failed to load manual weather config");
                return false;
            }
            WU_Logger.Info("[CONFIG MANAGER] Manual weather mode enabled");
            return true;
        }
    }
    
    void CreateDefaultConfigs()
    {
        m_Settings.CreateDefault();
        m_AutoConfig.CreateDefault();
        m_ManualConfig.CreateDefault();
        
        WU_ConfigZoneTemperature zoneConfig = GetWU_ZoneTemperatureConfig();
        if (zoneConfig)
        {
            zoneConfig.CreateDefault();
        }
        
        WU_Logger.Info("[CONFIG MANAGER] All default configuration files created");
    }
    
    bool GetEnableLogging()
    {
        return m_Settings.EnableLogging == 1;
    }
    
    int GetKeepLogsDays()
    {
        return m_Settings.KeepLogsDays;
    }
    
    bool GetAutoWeatherChanges()
    {
        return m_Settings.AutoWeatherChanges == 1;
    }
    
    string GetDefaultWeatherPreset()
    {
        if (m_Settings.AutoWeatherChanges == 1)
            return m_AutoConfig.DefaultWeatherPreset;
        else
            return m_ManualConfig.DefaultWeatherPreset;
    }
    
    int GetWeatherCheckInterval()
    {
        if (m_Settings.AutoWeatherChanges == 1)
            return m_AutoConfig.WeatherCheckInterval;
        else
            return m_ManualConfig.WeatherCheckInterval;
    }
    
    int GetWeatherChangeInterval()
    {
        if (m_Settings.AutoWeatherChanges == 1)
            return m_AutoConfig.WeatherChangeInterval;
        else
            return 1800;
    }
    
    int GetRandomWeatherChance()
    {
        if (m_Settings.AutoWeatherChanges == 1)
            return m_AutoConfig.RandomWeatherChance;
        else
            return 30;
    }
    
    map<string, ref WU_WeatherPreset> GetWeatherPresets()
    {
        if (m_Settings.AutoWeatherChanges == 1)
            return m_AutoConfig.WeatherPresets;
        else
            return m_ManualConfig.WeatherPresets;
    }
    
    array<ref WU_WeatherSchedule> GetWeatherSchedule()
    {
        if (m_Settings.AutoWeatherChanges == 1)
            return new array<ref WU_WeatherSchedule>();
        else
            return m_ManualConfig.WeatherSchedule;
    }
    
    bool GetEnableTemperatureControl()
    {
        return m_Settings.EnableTemperatureControl == 1;
    }
    
    bool GetEnableZoneTemperatureControl()
    {
        return m_Settings.EnableZoneTemperatureControl == 1;
    }
    
    bool GetEnableDebugLogging()
    {
        return m_Settings.EnableDebugLogging == 1;
    }
    
    float GetDriftCorrectionTime()
    {
        return m_Settings.DriftCorrectionTime;
    }
    
    float GetDriftCorrectionCooldown()
    {
        return m_Settings.DriftCorrectionCooldown;
    }
    
    float GetDriftTolerance()
    {
        return m_Settings.DriftTolerance;
    }
};
