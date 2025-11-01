modded class MissionServer
{
    private string m_WU_LogFileName;
    private string m_WU_SettingsRoot;
    private string m_WU_FolderPath;
    
    void MissionServer()
    {
        m_WU_SettingsRoot = "$profile:\\Weather_Upgrade\\";
        m_WU_FolderPath = m_WU_SettingsRoot + "Logs\\";
        
        WU_Logger.Initialize();
        
        WU_RotateLogFile();
        
        Weather weather = GetGame().GetWeather();
        if (weather)
        {
            weather.MissionWeather(true);
            
            weather.GetOvercast().Set(0.0, 0.0, 99999.0);
            weather.GetFog().Set(0.0, 0.0, 99999.0);
            weather.GetRain().Set(0.0, 0.0, 99999.0);
            weather.GetSnowfall().Set(0.0, 0.0, 99999.0);
            
            WU_Logger.Info("[MISSION SERVER] Weather control taken - cfgweather.xml overridden");
            WU_Logger.Info("[MISSION SERVER] Automatic weather changes disabled");
        }
        
        WU_WeatherManager weatherManager = WU_WeatherManager.GetInstance();
        if (weatherManager)
        {
            weatherManager.OnInit();
            WU_Logger.Info("[MISSION SERVER] Weather Upgrade mod initialized on server");
            WU_Logger.Info("[MISSION SERVER] Weather manager ready for commands");
        }
        
        ref WU_ConfigManager tempConfigManager = new WU_ConfigManager();
        if (tempConfigManager)
        {
            // Ensure settings are loaded before reading flags
            tempConfigManager.LoadConfig();
            bool zoneControlEnabled = tempConfigManager.GetEnableZoneTemperatureControl();
            WU_ENABLE_ZONE_TEMPERATURE_CONTROL = zoneControlEnabled;
            
            if (zoneControlEnabled)
            {
                WU_ConfigZoneTemperature zoneConfig = GetWU_ZoneTemperatureConfig();
                if (zoneConfig)
                {
                    if (!zoneConfig.Load())
                    {
                        zoneConfig.CreateDefault();
                        zoneConfig.Load();
                    }
                }
            }
            else
            {
                WU_Logger.Info("[MISSION SERVER] Zone temperature control disabled in settings");
            }
        }
    }
    
    void WU_RotateLogFile()
    {
        if (!FileExist(m_WU_SettingsRoot)) MakeDirectory(m_WU_SettingsRoot);
        if (!FileExist(m_WU_FolderPath)) MakeDirectory(m_WU_FolderPath);
        
        int year, month, day, hour, minute, second;
        GetYearMonthDay(year, month, day);
        GetHourMinuteSecond(hour, minute, second);
        m_WU_LogFileName = string.Format("Weather_Upgrade_%1_%2_%3_%4_%5_%6.log", year, month, day, hour, minute, second);
        
        WU_Logger.SetLogFileName(m_WU_LogFileName);
        
        WU_Logger.Info(string.Format("[WEATHER UPGRADE] New log file created: %1", m_WU_LogFileName));
    }
    
    override void OnInit()
    {
        super.OnInit();
        
        WU_Logger.Info("[MISSION SERVER] MissionServer OnInit completed");
    }
    
    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        
        WU_WeatherManager weatherManager = WU_WeatherManager.GetInstance();
        if (weatherManager)
        {
            weatherManager.OnUpdate(timeslice);
        }
        
        WU_PlayerZonePlugin zonePlugin = WU_PlayerZonePlugin.GetInstance();
        if (zonePlugin)
        {
            zonePlugin.Update(timeslice);
        }
    }
}