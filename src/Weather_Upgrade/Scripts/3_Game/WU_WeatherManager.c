class WU_ScheduleEntry
{
    int TimeMinutes;
    string Preset;
    string OriginalTime;
};

class WU_WeatherManager
{
    static const string SETTINGS_FILE = "$profile:\\Weather_Upgrade\\WU_Settings.json";
    
    private static ref WU_WeatherManager m_Instance;
    private string m_LogFileName;
    private string m_SettingsRoot;
    private string m_FolderPath;
    
    protected bool m_Started = false;
    protected float m_WeatherTime = 0.0;
    protected float m_NextChangeTime = 0.0;
    protected ref WU_WeatherConfig m_CurrentWeather;
    
    protected float m_TransitionStartTime = 0.0;
    protected float m_CurrentTransitionDuration = 0.0;
    protected bool m_InTransition = false;
    
    protected bool m_EnableLogging = true;
    protected int m_KeepLogsDays = 30;
    protected bool m_AutoWeatherChanges = false;
    protected string m_DefaultWeatherPreset = "clear";
    protected int m_WeatherCheckInterval = 60;
    protected int m_WeatherChangeInterval = 1800;
    protected int m_RandomWeatherChance = 30;
    protected ref map<string, ref WU_WeatherPreset> m_WeatherPresets;
    protected ref array<ref WU_WeatherSchedule> m_WeatherSchedule;
    
    protected string m_LastAppliedPreset = "";
    protected float m_LastCorrectionTime = 0.0;
    protected float m_CorrectionTolerance = 0.1;
    protected float m_DriftCorrectionTime = 60.0;
    protected float m_DriftCorrectionCooldown = 30.0;
    
    void WU_WeatherManager()
    {
        m_SettingsRoot = "$profile:\\Weather_Upgrade\\";
        m_FolderPath = m_SettingsRoot + "Logs\\";
        m_WeatherPresets = new map<string, ref WU_WeatherPreset>();
        m_WeatherSchedule = new array<ref WU_WeatherSchedule>();
        
        WU_Logger.Initialize();
        WU_RotateLogFile();
    }
    
    static WU_WeatherManager GetInstance()
    {
        if (!m_Instance)
        {
            m_Instance = new WU_WeatherManager();
        }
        return m_Instance;
    }
    
    void WU_RotateLogFile()
    {
        if (!FileExist(m_SettingsRoot)) MakeDirectory(m_SettingsRoot);
        if (!FileExist(m_FolderPath)) MakeDirectory(m_FolderPath);
        
        int year, month, day, hour, minute, second;
        GetYearMonthDay(year, month, day);
        GetHourMinuteSecond(hour, minute, second);
        m_LogFileName = string.Format("Weather_Upgrade_%1_%2_%3_%4_%5_%6.log", year, month, day, hour, minute, second);
        
        WU_Logger.SetLogFileName(m_LogFileName);
        WU_Logger.Info(string.Format("[WEATHER UPGRADE] Log file created: %1", m_LogFileName));
    }
    
    string FormatGameTime(int hour, int minute)
    {
        string hourStr = hour.ToString();
        string minuteStr = minute.ToString();
        
        if (hour < 10)
            hourStr = "0" + hourStr;
        
        if (minute < 10)
            minuteStr = "0" + minuteStr;
        
        return hourStr + ":" + minuteStr;
    }
    
    void OnInit()
    {
        if (!LoadConfig())
        {
            WU_Logger.Error("[WEATHER MANAGER] Failed to load configuration - creating defaults");
            CreateDefaultConfig();
        }
        
        Weather weather = GetGame().GetWeather();
        if (weather)
        {
            weather.MissionWeather(true);
            Start();
            WU_Logger.Info("[WEATHER MANAGER] Weather Upgrade mod initialized successfully");
        }
        else
        {
            WU_Logger.Error("[WEATHER MANAGER] Failed to get weather system");
        }
    }
    
    bool LoadConfig()
    {
        ref WU_ConfigManager configManager = new WU_ConfigManager();
        if (configManager.LoadConfig())
        {
            m_EnableLogging = configManager.GetEnableLogging();
            bool enableDebug = configManager.GetEnableDebugLogging();
            
            WU_Logger.SetEnableLogging(m_EnableLogging);
            WU_Logger.SetEnableDebugLogging(enableDebug);
            
            m_KeepLogsDays = configManager.GetKeepLogsDays();
            m_CorrectionTolerance = configManager.GetDriftTolerance();
            m_DriftCorrectionTime = configManager.GetDriftCorrectionTime();
            m_DriftCorrectionCooldown = configManager.GetDriftCorrectionCooldown();
            m_AutoWeatherChanges = configManager.GetAutoWeatherChanges();
            m_DefaultWeatherPreset = configManager.GetDefaultWeatherPreset();
            m_WeatherCheckInterval = configManager.GetWeatherCheckInterval();
            m_WeatherChangeInterval = configManager.GetWeatherChangeInterval();
            m_RandomWeatherChance = configManager.GetRandomWeatherChance();
            m_WeatherPresets = configManager.GetWeatherPresets();
            m_WeatherSchedule = configManager.GetWeatherSchedule();
            
            WU_Logger.Info("[WEATHER MANAGER] Configuration loaded successfully");
            return true;
        }
        
        WU_Logger.Error("[WEATHER MANAGER] Failed to load configuration");
        return false;
    }
    
    void CreateDefaultConfig()
    {
        ref WU_ConfigManager configManager = new WU_ConfigManager();
        configManager.CreateDefaultConfigs();
        
        if (LoadConfig())
        {
            WU_Logger.Info("[WEATHER MANAGER] Default configuration created and loaded");
        }
    }
    
    protected void Start()
    {
        WU_Logger.Info("[WEATHER MANAGER] Starting weather system");
        m_Started = true;
        
        if (!m_AutoWeatherChanges && m_WeatherSchedule.Count() > 0)
        {
            WU_Logger.Info("[WEATHER MANAGER] Schedule mode active - waiting for game time synchronization");
            WU_Logger.Info("[WEATHER MANAGER] Weather will be applied on first check in " + m_WeatherCheckInterval.ToString() + " seconds");
            WU_Logger.Info("[WEATHER MANAGER] Using default preset '" + m_DefaultWeatherPreset + "' as fallback");
            m_LastAppliedPreset = "";
        }
        else if (m_AutoWeatherChanges)
        {
            string presetToApply = m_DefaultWeatherPreset;
            
            if (presetToApply == "" && m_WeatherPresets.Count() > 0)
            {
                int randomIndex = Math.RandomInt(0, m_WeatherPresets.Count());
                presetToApply = m_WeatherPresets.GetKey(randomIndex);
                WU_Logger.Info("[WEATHER MANAGER] Auto weather enabled, no default preset - randomly selected '" + presetToApply + "'");
            }
            else
            {
                WU_Logger.Info("[WEATHER MANAGER] Auto weather enabled, applying default preset '" + presetToApply + "'");
            }
            
            if (m_WeatherPresets.Contains(presetToApply))
            {
                ref WU_WeatherPreset preset = m_WeatherPresets.Get(presetToApply);
                ref WU_WeatherConfig initialConfig = preset.GenerateRandomValues();
                
                ApplyWeatherConfig(initialConfig, true);
                m_LastAppliedPreset = presetToApply;
                
                WU_Logger.Info("[WEATHER MANAGER] Initial preset '" + presetToApply + "' applied");
            }
            else
            {
                WU_Logger.Error("[WEATHER MANAGER] Default preset '" + presetToApply + "' not found in config");
            }
            
            m_NextChangeTime = m_WeatherChangeInterval;
            WU_Logger.Info("[AUTO WEATHER] Random changes every " + m_WeatherChangeInterval.ToString() + "s (" + m_RandomWeatherChance.ToString() + "% chance)");
            WU_Logger.Info("[AUTO WEATHER] Available presets: " + m_WeatherPresets.Count().ToString());
        }
        else
        {
            WU_Logger.Info("[MANUAL WEATHER] Schedule mode enabled");
            WU_Logger.Info("[MANUAL WEATHER] Schedule entries loaded: " + m_WeatherSchedule.Count().ToString());
            WU_Logger.Info("[MANUAL WEATHER] Check interval: " + m_WeatherCheckInterval.ToString() + "s");
        }
    }
    
    protected void Stop()
    {
        WU_Logger.Info("[WEATHER MANAGER] Stopping weather system");
        m_Started = false;
    }
    
    void ApplyWeatherConfig(WU_WeatherConfig config, bool immediate)
    {
        if (!config || !config.IsValid())
        {
            WU_Logger.Error("[WEATHER MANAGER] Invalid weather config provided");
            return;
        }
        
        WU_Logger.Debug("[WEATHER CONFIG] Applying: O=" + config.m_Overcast.ToString() + " F=" + config.m_Fog.ToString() + " R=" + config.m_Rain.ToString() + " S=" + config.m_Snowfall.ToString());
        WU_Logger.Debug("[WEATHER CONFIG] Wind=" + config.m_WindSpeed.ToString() + " Dir=" + config.m_WindDirection.ToString() + " Trans=" + config.m_TransitionTime.ToString() + "s Dur=" + config.m_MinDuration.ToString() + "s");
        
        Weather weather = GetGame().GetWeather();
        if (!weather)
        {
            WU_Logger.Error("[WEATHER MANAGER] Failed to get weather system");
            return;
        }
        
        weather.GetOvercast().SetLimits(0.0, 1.0);
        weather.GetOvercast().SetForecastChangeLimits(0.0, 1.0);
        weather.GetOvercast().SetForecastTimeLimits(0.0, 99999.0);
        weather.GetFog().SetLimits(0.0, 1.0);
        weather.GetFog().SetForecastChangeLimits(0.0, 1.0);
        weather.GetFog().SetForecastTimeLimits(0.0, 99999.0);
        weather.GetRain().SetLimits(0.0, 1.0);
        weather.GetRain().SetForecastChangeLimits(0.0, 1.0);
        weather.GetRain().SetForecastTimeLimits(0.0, 99999.0);
        weather.GetSnowfall().SetLimits(0.0, 1.0);
        weather.GetSnowfall().SetForecastChangeLimits(0.0, 1.0);
        weather.GetSnowfall().SetForecastTimeLimits(0.0, 99999.0);
        
        float transTime = config.m_TransitionTime;
        if (immediate)
            transTime = 0.0;
        
        weather.GetOvercast().Set(config.m_Overcast, transTime, config.m_MinDuration);
        weather.GetOvercast().SetForecastChangeLimits(config.m_Overcast - 0.05, config.m_Overcast + 0.05);
        
        weather.GetFog().Set(config.m_Fog, transTime, config.m_MinDuration);
        weather.GetFog().SetForecastChangeLimits(config.m_Fog - 0.05, config.m_Fog + 0.05);
        
        weather.GetRain().Set(config.m_Rain, transTime, config.m_MinDuration);
        weather.GetRain().SetForecastChangeLimits(config.m_Rain - 0.05, config.m_Rain + 0.05);
        
        weather.GetSnowfall().Set(config.m_Snowfall, transTime, config.m_MinDuration);
        weather.GetSnowfall().SetForecastChangeLimits(config.m_Snowfall - 0.05, config.m_Snowfall + 0.05);
        
        weather.SetSnowfallThresholds(config.m_SnowfallThreshold_Min, config.m_SnowfallThreshold_Max, config.m_SnowfallThreshold_Timeout);
        weather.SetRainThresholds(config.m_RainThreshold_Min, config.m_RainThreshold_Max, config.m_RainThreshold_Timeout);
        
        float angleRad = Math.DEG2RAD * config.m_WindDirection;
        vector dir = Weather.AngleToWindDirection(angleRad);
        weather.SetWind(dir * config.m_WindSpeed);
        weather.SetWindMaximumSpeed(config.m_WindSpeed);
        weather.SetWindFunctionParams(config.m_WindFunc_Min, config.m_WindFunc_Max, config.m_WindFunc_Speed);
        
        if (config.m_ThunderstormDensity > 0.0)
        {
            weather.SetStorm(config.m_ThunderstormDensity, config.m_ThunderstormThreshold, config.m_LightningTimeout);
            weather.SuppressLightningSimulation(false);
        }
        else
        {
            weather.SetStorm(0.0, 1.0, 99999.0);
            weather.SuppressLightningSimulation(true);
        }
        
        if (weather.IsDynVolFogEnabled())
        {
            float fogTransTime = config.m_FogTransitionTime;
            if (immediate)
                fogTransTime = 0.0;
            
            weather.SetDynVolFogHeightDensity(config.m_VolFogHeightDensity, fogTransTime);
            weather.SetDynVolFogDistanceDensity(config.m_VolFogDistanceDensity, fogTransTime);
            weather.SetDynVolFogHeightBias(config.m_VolFogHeightBias, fogTransTime);
            
            if (config.m_VolFogHeightDensity > 0.0 || config.m_VolFogDistanceDensity > 0.0)
            {
                WU_Logger.Info("[WEATHER] Volumetric fog applied: Height=" + config.m_VolFogHeightDensity.ToString() + " Distance=" + config.m_VolFogDistanceDensity.ToString() + " Bias=" + config.m_VolFogHeightBias.ToString());
            }
            else
            {
                WU_Logger.Info("[WEATHER] Volumetric fog cleared (set to 0)");
            }
        }
        else
        {
            if (config.m_VolFogHeightDensity > 0.0 || config.m_VolFogDistanceDensity > 0.0)
            {
                WU_Logger.Warning("[WEATHER] Volumetric fog NOT available in world config");
            }
        }
        
        weather.SetSnowflakeScale(config.m_SnowflakeScale);
        
        WU_GLOBAL_TEMPERATURE_OVERRIDE = config.m_EnvironmentTemperature;
        
        ref WU_ConfigManager configManager = new WU_ConfigManager();
        bool tempControlEnabled = configManager.GetEnableTemperatureControl();
        
        WU_USE_TEMPERATURE_OVERRIDE = tempControlEnabled && config.m_UseEnvironmentTemperature == 1;
        
        if (WU_USE_TEMPERATURE_OVERRIDE)
        {
            WU_Logger.Info("[WEATHER] Environment temperature override: " + config.m_EnvironmentTemperature.ToString() + "°C");
        }
        
        m_CurrentWeather = config;
        
        if (immediate)
        {
            m_InTransition = false;
            WU_Logger.Info("[WEATHER] Applied immediately, maintaining for " + config.m_MinDuration.ToString() + "s");
        }
        else
        {
            m_InTransition = true;
            m_TransitionStartTime = GetGame().GetTime();
            m_CurrentTransitionDuration = config.m_TransitionTime;
            WU_Logger.Info("[WEATHER] Transition starting over " + config.m_TransitionTime.ToString() + "s, maintaining for " + config.m_MinDuration.ToString() + "s");
        }
        
        WU_Logger.Info("[WEATHER] Values: O=" + config.m_Overcast.ToString() + " F=" + config.m_Fog.ToString() + " R=" + config.m_Rain.ToString() + " S=" + config.m_Snowfall.ToString() + " W=" + config.m_WindSpeed.ToString() + "m/s");
    }
    
    void GetCurrentWeatherState()
    {
        Weather weather = GetGame().GetWeather();
        if (!weather)
        {
            WU_Logger.Error("[WEATHER] Failed to get weather system");
            return;
        }
        
        int currentYear, currentMonth, currentDay, currentHour, currentMinute;
        GetGame().GetWorld().GetDate(currentYear, currentMonth, currentDay, currentHour, currentMinute);
        
        float currentOvercast = weather.GetOvercast().GetActual();
        float currentFog = weather.GetFog().GetActual();
        float currentRain = weather.GetRain().GetActual();
        float currentSnowfall = weather.GetSnowfall().GetActual();
        float windSpeed = weather.GetWindSpeed();
        float currentSnowflakeScale = weather.GetSnowflakeScale();
        
        float currentVolFogHeight = 0.0;
        float currentVolFogDist = 0.0;
        float currentVolFogBias = 0.0;
        
        if (weather.IsDynVolFogEnabled())
        {
            currentVolFogHeight = weather.GetDynVolFogHeightDensity();
            currentVolFogDist = weather.GetDynVolFogDistanceDensity();
            currentVolFogBias = weather.GetDynVolFogHeightBias();
        }
        
        WU_Logger.Info("========================================");
        WU_Logger.Info("[WEATHER STATUS] Time: " + FormatGameTime(currentHour, currentMinute) + " | Active Preset: '" + m_LastAppliedPreset + "'");
        WU_Logger.Info("========================================");
        
        if (m_CurrentWeather)
        {
            WU_Logger.Info("[OVERCAST]     Current: " + currentOvercast.ToString() + " | Expected: " + m_CurrentWeather.m_Overcast.ToString());
            WU_Logger.Info("[FOG]          Current: " + currentFog.ToString() + " | Expected: " + m_CurrentWeather.m_Fog.ToString());
            WU_Logger.Info("[RAIN]         Current: " + currentRain.ToString() + " | Expected: " + m_CurrentWeather.m_Rain.ToString());
            WU_Logger.Info("[SNOWFALL]     Current: " + currentSnowfall.ToString() + " | Expected: " + m_CurrentWeather.m_Snowfall.ToString());
            WU_Logger.Info("[WIND SPEED]   Current: " + windSpeed.ToString() + " | Expected: " + m_CurrentWeather.m_WindSpeed.ToString());
            
            if (weather.IsDynVolFogEnabled())
            {
                WU_Logger.Info("[VOL FOG HEIGHT]   Current: " + currentVolFogHeight.ToString() + " | Expected: " + m_CurrentWeather.m_VolFogHeightDensity.ToString());
                WU_Logger.Info("[VOL FOG DISTANCE] Current: " + currentVolFogDist.ToString() + " | Expected: " + m_CurrentWeather.m_VolFogDistanceDensity.ToString());
                WU_Logger.Info("[VOL FOG BIAS]     Current: " + currentVolFogBias.ToString() + " | Expected: " + m_CurrentWeather.m_VolFogHeightBias.ToString());
            }
            
            WU_Logger.Info("[SNOWFLAKE SCALE]  Current: " + currentSnowflakeScale.ToString() + " | Expected: " + m_CurrentWeather.m_SnowflakeScale.ToString());
            
            if (m_CurrentWeather.m_UseEnvironmentTemperature == 1)
            {
                WU_Logger.Info("[TEMPERATURE]      Override: " + m_CurrentWeather.m_EnvironmentTemperature.ToString() + "°C");
            }
            
            WU_Logger.Info("========================================");
            
            bool needsCorrection = false;
            float overcastDrift = Math.AbsFloat(currentOvercast - m_CurrentWeather.m_Overcast);
            float fogDrift = Math.AbsFloat(currentFog - m_CurrentWeather.m_Fog);
            float rainDrift = Math.AbsFloat(currentRain - m_CurrentWeather.m_Rain);
            float snowfallDrift = Math.AbsFloat(currentSnowfall - m_CurrentWeather.m_Snowfall);
            float windDrift = Math.AbsFloat(windSpeed - m_CurrentWeather.m_WindSpeed);
            
            if (overcastDrift > m_CorrectionTolerance) needsCorrection = true;
            if (fogDrift > m_CorrectionTolerance) needsCorrection = true;
            if (rainDrift > m_CorrectionTolerance) needsCorrection = true;
            if (snowfallDrift > m_CorrectionTolerance) needsCorrection = true;
            if (windDrift > m_CorrectionTolerance * 10.0) needsCorrection = true;
            
            WU_Logger.Debug("[DRIFT CHECK] O=" + overcastDrift.ToString() + " F=" + fogDrift.ToString() + " R=" + rainDrift.ToString() + " S=" + snowfallDrift.ToString() + " W=" + windDrift.ToString());
            
            if (needsCorrection)
            {
                float currentTime = GetGame().GetTime();
                float timeSinceLastCorrection = currentTime - m_LastCorrectionTime;
                
                if (timeSinceLastCorrection >= m_DriftCorrectionCooldown)
                {
                    if (m_AutoWeatherChanges)
                    {
                        WU_Logger.Info("[AUTO WEATHER] Weather drifted, correcting to maintain preset");
                        WU_Logger.Debug("[DRIFT CORRECTION] Last correction was " + timeSinceLastCorrection.ToString() + "s ago");
                    }
                    else
                    {
                        WU_Logger.Info("[MANUAL WEATHER] Weather drifted from schedule, correcting to maintain preset");
                        WU_Logger.Debug("[DRIFT CORRECTION] Last correction was " + timeSinceLastCorrection.ToString() + "s ago");
                    }
                    ForceWeatherCorrection();
                    m_LastCorrectionTime = currentTime;
                }
                else
                {
                    WU_Logger.Info("[WEATHER STATUS] Drift detected but correction cooldown active");
                    WU_Logger.Debug("[DRIFT COOLDOWN] " + (m_DriftCorrectionCooldown - timeSinceLastCorrection).ToString() + "s remaining");
                }
            }
            else
            {
                WU_Logger.Debug("[WEATHER STATUS] All values within tolerance - no correction needed");
            }
        }
        else
        {
            WU_Logger.Info("[OVERCAST]     Current: " + currentOvercast.ToString());
            WU_Logger.Info("[FOG]          Current: " + currentFog.ToString());
            WU_Logger.Info("[RAIN]         Current: " + currentRain.ToString());
            WU_Logger.Info("[SNOWFALL]     Current: " + currentSnowfall.ToString());
            WU_Logger.Info("[WIND SPEED]   Current: " + windSpeed.ToString());
            WU_Logger.Info("[SNOWFLAKE SCALE]  Current: " + currentSnowflakeScale.ToString());
            WU_Logger.Info("========================================");
            WU_Logger.Warning("[WEATHER STATUS] No expected weather config loaded");
        }
    }
    
    void ForceWeatherCorrection()
    {
        if (!m_CurrentWeather)
        {
            WU_Logger.Error("[WEATHER] Cannot force correction - no current weather data");
            return;
        }
        
        Weather weather = GetGame().GetWeather();
        if (!weather)
        {
            WU_Logger.Error("[WEATHER] Failed to get weather system for correction");
            return;
        }
        
        weather.GetOvercast().Set(m_CurrentWeather.m_Overcast, m_DriftCorrectionTime, m_DriftCorrectionTime * 2.0);
        weather.GetFog().Set(m_CurrentWeather.m_Fog, m_DriftCorrectionTime, m_DriftCorrectionTime * 2.0);
        weather.GetRain().Set(m_CurrentWeather.m_Rain, m_DriftCorrectionTime, m_DriftCorrectionTime * 2.0);
        weather.GetSnowfall().Set(m_CurrentWeather.m_Snowfall, m_DriftCorrectionTime, m_DriftCorrectionTime * 2.0);
        
        float angleRad = Math.DEG2RAD * m_CurrentWeather.m_WindDirection;
        vector dir = Weather.AngleToWindDirection(angleRad);
        weather.SetWind(dir * m_CurrentWeather.m_WindSpeed);
        weather.SetWindMaximumSpeed(m_CurrentWeather.m_WindSpeed);
        weather.SetWindFunctionParams(m_CurrentWeather.m_WindFunc_Min, m_CurrentWeather.m_WindFunc_Max, m_CurrentWeather.m_WindFunc_Speed);
        
        if (weather.IsDynVolFogEnabled())
        {
            weather.SetDynVolFogHeightDensity(m_CurrentWeather.m_VolFogHeightDensity, m_DriftCorrectionTime);
            weather.SetDynVolFogDistanceDensity(m_CurrentWeather.m_VolFogDistanceDensity, m_DriftCorrectionTime);
            weather.SetDynVolFogHeightBias(m_CurrentWeather.m_VolFogHeightBias, m_DriftCorrectionTime);
        }
        
        weather.SetSnowflakeScale(m_CurrentWeather.m_SnowflakeScale);
        
        WU_Logger.Info("[WEATHER] Weather corrected over " + m_DriftCorrectionTime.ToString() + "s");
    }
    
    void CheckScheduledWeather()
    {
        if (!m_Started || m_AutoWeatherChanges)
            return;
        
        int currentYear, currentMonth, currentDay, currentHour, currentMinute;
        GetGame().GetWorld().GetDate(currentYear, currentMonth, currentDay, currentHour, currentMinute);
        int currentTimeMinutes = currentHour * 60 + currentMinute;
        
        string activePreset = GetActiveWeatherPreset(currentTimeMinutes);
        
        if (activePreset == "")
        {
            WU_Logger.Error("[WEATHER] No active weather preset found for current time");
            return;
        }
        
        if (m_LastAppliedPreset != activePreset)
        {
            if (m_WeatherPresets.Contains(activePreset))
            {
                ref WU_WeatherPreset preset = m_WeatherPresets.Get(activePreset);
                if (preset && preset.IsValid())
                {
                    if (m_LastAppliedPreset == "")
                    {
                        WU_Logger.Info("[MANUAL WEATHER] " + FormatGameTime(currentHour, currentMinute) + " - Initial schedule application: '" + activePreset + "'");
                    }
                    else
                    {
                        WU_Logger.Info("[MANUAL WEATHER] " + FormatGameTime(currentHour, currentMinute) + " - Schedule change: '" + m_LastAppliedPreset + "' -> '" + activePreset + "'");
                    }
                    
                    float durationUntilNext = CalculateDurationUntilNextSchedule(currentTimeMinutes);
                    WU_Logger.Info("[MANUAL WEATHER] Duration until next schedule: " + durationUntilNext.ToString() + "s");
                    
                    ref WU_WeatherConfig generatedConfig = preset.GenerateRandomValues();
                    
                    if (!generatedConfig)
                    {
                        WU_Logger.Error("[WEATHER] GenerateRandomValues returned null for preset: " + activePreset);
                        return;
                    }
                    
                    if (!generatedConfig.IsValid())
                    {
                        WU_Logger.Error("[WEATHER] Generated config failed validation for preset: " + activePreset);
                        WU_Logger.Debug("[WEATHER] O=" + generatedConfig.m_Overcast.ToString() + " F=" + generatedConfig.m_Fog.ToString() + " R=" + generatedConfig.m_Rain.ToString() + " S=" + generatedConfig.m_Snowfall.ToString());
                        WU_Logger.Debug("[WEATHER] W=" + generatedConfig.m_WindSpeed.ToString() + " Dir=" + generatedConfig.m_WindDirection.ToString() + " Trans=" + generatedConfig.m_TransitionTime.ToString() + " Dur=" + generatedConfig.m_MinDuration.ToString());
                        return;
                    }
                    
                    WU_Logger.Debug("[WEATHER] Generated values for '" + activePreset + "': O=" + generatedConfig.m_Overcast.ToString() + " F=" + generatedConfig.m_Fog.ToString() + " R=" + generatedConfig.m_Rain.ToString() + " S=" + generatedConfig.m_Snowfall.ToString() + " W=" + generatedConfig.m_WindSpeed.ToString());
                    
                    bool applyImmediate = false;
                    if (m_LastAppliedPreset == "")
                    {
                        applyImmediate = true;
                        WU_Logger.Debug("[WEATHER] First weather application - applying immediately");
                    }
                    else
                    {
                        WU_Logger.Debug("[WEATHER] Preset changed from '" + m_LastAppliedPreset + "' to '" + activePreset + "' - using transition");
                    }
                    
                    ApplyWeatherConfig(generatedConfig, applyImmediate);
                    m_LastAppliedPreset = activePreset;
                }
                else
                {
                    WU_Logger.Error("[WEATHER] Invalid preset: " + activePreset);
                }
            }
            else
            {
                WU_Logger.Error("[WEATHER] Preset not found: " + activePreset);
            }
        }
    }
    
    string GetActiveWeatherPreset(int currentTimeMinutes)
    {
        array<ref WU_ScheduleEntry> sortedSchedule = new array<ref WU_ScheduleEntry>();
        
        for (int i = 0; i < m_WeatherSchedule.Count(); i++)
        {
            ref WU_WeatherSchedule schedule = m_WeatherSchedule.Get(i);
            if (!schedule)
                continue;
            
            TStringArray timeParts = new TStringArray();
            schedule.Time.Split(":", timeParts);
            if (timeParts.Count() != 2)
                continue;
            
            int scheduledHour = timeParts[0].ToInt();
            int scheduledMinute = timeParts[1].ToInt();
            int scheduledTimeMinutes = scheduledHour * 60 + scheduledMinute;
            
            ref WU_ScheduleEntry entry = new WU_ScheduleEntry();
            entry.TimeMinutes = scheduledTimeMinutes;
            entry.Preset = schedule.Preset;
            entry.OriginalTime = schedule.Time;
            
            sortedSchedule.Insert(entry);
        }
        
        for (int sortI = 0; sortI < sortedSchedule.Count() - 1; sortI++)
        {
            for (int sortJ = 0; sortJ < sortedSchedule.Count() - 1 - sortI; sortJ++)
            {
                if (sortedSchedule[sortJ].TimeMinutes > sortedSchedule[sortJ + 1].TimeMinutes)
                {
                    ref WU_ScheduleEntry temp = sortedSchedule[sortJ];
                    sortedSchedule[sortJ] = sortedSchedule[sortJ + 1];
                    sortedSchedule[sortJ + 1] = temp;
                }
            }
        }
        
        string activePreset = "";
        
        for (int searchI = 0; searchI < sortedSchedule.Count(); searchI++)
        {
            ref WU_ScheduleEntry searchEntry = sortedSchedule[searchI];
            
            if (currentTimeMinutes >= searchEntry.TimeMinutes)
            {
                activePreset = searchEntry.Preset;
            }
            else
            {
                break;
            }
        }
        
        if (activePreset == "" && sortedSchedule.Count() > 0)
        {
            activePreset = sortedSchedule[sortedSchedule.Count() - 1].Preset;
        }
        
        return activePreset;
    }
    
    float CalculateDurationUntilNextSchedule(int currentTimeMinutes)
    {
        array<int> sortedTimes = new array<int>();
        
        for (int i = 0; i < m_WeatherSchedule.Count(); i++)
        {
            ref WU_WeatherSchedule schedule = m_WeatherSchedule.Get(i);
            if (!schedule) continue;
            
            TStringArray timeParts = new TStringArray();
            schedule.Time.Split(":", timeParts);
            if (timeParts.Count() != 2) continue;
            
            int scheduledHour = timeParts[0].ToInt();
            int scheduledMinute = timeParts[1].ToInt();
            int scheduledTimeMinutes = scheduledHour * 60 + scheduledMinute;
            
            sortedTimes.Insert(scheduledTimeMinutes);
        }
        
        for (int sortI = 0; sortI < sortedTimes.Count() - 1; sortI++)
        {
            for (int sortJ = 0; sortJ < sortedTimes.Count() - 1 - sortI; sortJ++)
            {
                if (sortedTimes[sortJ] > sortedTimes[sortJ + 1])
                {
                    int temp = sortedTimes[sortJ];
                    sortedTimes[sortJ] = sortedTimes[sortJ + 1];
                    sortedTimes[sortJ + 1] = temp;
                }
            }
        }
        
        int nextScheduleTime = -1;
        for (int searchI = 0; searchI < sortedTimes.Count(); searchI++)
        {
            if (sortedTimes[searchI] > currentTimeMinutes)
            {
                nextScheduleTime = sortedTimes[searchI];
                break;
            }
        }
        
        if (nextScheduleTime == -1 && m_WeatherSchedule.Count() > 0)
        {
            nextScheduleTime = sortedTimes[0] + (24 * 60);
        }
        
        if (nextScheduleTime == -1)
        {
            return 3600.0;
        }
        
        float durationMinutes = nextScheduleTime - currentTimeMinutes;
        float durationSeconds = durationMinutes * 60.0;
        
        return durationSeconds;
    }
    
    void ApplyRandomWeather()
    {
        if (!m_Started || !m_AutoWeatherChanges)
            return;
        
        int randomChance = Math.RandomInt(1, 100);
        WU_Logger.Debug("[AUTO WEATHER] Random change check - rolled: " + randomChance.ToString() + "% (threshold: " + m_RandomWeatherChance.ToString() + "%)");
        
        if (randomChance > m_RandomWeatherChance)
        {
            WU_Logger.Info("[AUTO WEATHER] Random roll: " + randomChance.ToString() + "% (threshold: " + m_RandomWeatherChance.ToString() + "%) - no change");
            return;
        }
        
        if (m_WeatherPresets.Count() > 0)
        {
            int randomIndex = Math.RandomInt(0, m_WeatherPresets.Count() - 1);
            string randomPresetName = m_WeatherPresets.GetKey(randomIndex);
            
            if (m_WeatherPresets.Contains(randomPresetName))
            {
                ref WU_WeatherPreset randomPreset = m_WeatherPresets.Get(randomPresetName);
                if (randomPreset && randomPreset.IsValid())
                {
                    WU_Logger.Info("[AUTO WEATHER] Random roll: " + randomChance.ToString() + "% - Applying '" + randomPresetName + "'");
                    
                    ref WU_WeatherConfig generatedConfig = randomPreset.GenerateRandomValues();
                    
                    if (!generatedConfig)
                    {
                        WU_Logger.Error("[WEATHER] GenerateRandomValues returned null for preset: " + randomPresetName);
                        return;
                    }
                    
                    if (!generatedConfig.IsValid())
                    {
                        WU_Logger.Error("[WEATHER] Generated config failed validation for preset: " + randomPresetName);
                        return;
                    }
                    
                    ApplyWeatherConfig(generatedConfig, false);
                    m_LastAppliedPreset = randomPresetName;
                }
            }
        }
    }
    
    void OnUpdate(float delta_time)
    {
        if (!m_Started)
            return;
        
        m_WeatherTime += delta_time;
        
        if (!m_AutoWeatherChanges)
        {
            static float lastScheduleCheck = 0.0;
            if (m_WeatherTime - lastScheduleCheck >= m_WeatherCheckInterval)
            {
                if (!m_InTransition)
                {
                    GetCurrentWeatherState();
                }
                
                CheckScheduledWeather();
                lastScheduleCheck = m_WeatherTime;
            }
        }
        else
        {
            static float lastAutoCheck = 0.0;
            float checkInterval = m_WeatherCheckInterval;
            
            if (lastAutoCheck == 0.0 && m_WeatherTime >= 10.0)
            {
                if (!m_InTransition)
                {
                    WU_Logger.Info("[AUTO WEATHER] First drift check (10s after startup)");
                    GetCurrentWeatherState();
                }
                lastAutoCheck = m_WeatherTime;
            }
            else if (m_WeatherTime - lastAutoCheck >= checkInterval)
            {
                if (!m_InTransition)
                {
                    GetCurrentWeatherState();
                }
                lastAutoCheck = m_WeatherTime;
            }
        }
        
        if (m_InTransition)
        {
            float currentTime = GetGame().GetTime();
            float elapsedTime = currentTime - m_TransitionStartTime;
            
            if (elapsedTime >= m_CurrentTransitionDuration)
            {
                m_InTransition = false;
                WU_Logger.Info("[WEATHER] Transition completed after " + elapsedTime.ToString() + "s");
            }
        }
        
        if (m_AutoWeatherChanges && m_WeatherTime >= m_NextChangeTime)
        {
            ApplyRandomWeather();
            m_WeatherTime = 0.0;
            m_NextChangeTime = m_WeatherChangeInterval;
        }
    }
    
    void OnDestroy()
    {
        Stop();
    }
};
