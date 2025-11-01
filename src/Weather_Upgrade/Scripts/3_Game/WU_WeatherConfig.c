class WU_WeatherPreset
{
    float m_TransitionTime_Min;
    float m_TransitionTime_Max;
    float m_MinDuration_Min;
    float m_MinDuration_Max;
    
    float m_Overcast_Min;
    float m_Overcast_Max;
    float m_Fog_Min;
    float m_Fog_Max;
    float m_Rain_Min;
    float m_Rain_Max;
    float m_Snowfall_Min;
    float m_Snowfall_Max;
    
    float m_WindSpeed_Min;
    float m_WindSpeed_Max;
    float m_WindDirection_Min;
    float m_WindDirection_Max;
    
    float m_WindFunc_Min;
    float m_WindFunc_Max;
    float m_WindFunc_Speed;
    
    int m_UseDynVolFog;
    float m_VolFogDistanceDensity_Min;
    float m_VolFogDistanceDensity_Max;
    float m_VolFogHeightDensity_Min;
    float m_VolFogHeightDensity_Max;
    float m_VolFogHeightBias;
    float m_FogTransitionTime;
    
    int m_UseSnowflakeScale;
    float m_SnowflakeScale_Min;
    float m_SnowflakeScale_Max;
    
    float m_SnowfallThreshold_Min;
    float m_SnowfallThreshold_Max;
    float m_SnowfallThreshold_Timeout;
    
    float m_RainThreshold_Min;
    float m_RainThreshold_Max;
    float m_RainThreshold_Timeout;
    
    float m_ThunderstormDensity;
    float m_ThunderstormThreshold;
    float m_LightningTimeout;
    
    int m_UseEnvironmentTemperature;
    float m_EnvironmentTemperature;
    
    void WU_WeatherPreset()
    {
        SetDefaultValues();
    }
    
    void SetDefaultValues()
    {
        m_TransitionTime_Min = 120.0;
        m_TransitionTime_Max = 240.0;
        m_MinDuration_Min = 600.0;
        m_MinDuration_Max = 1200.0;
        
        m_Overcast_Min = 0.1;
        m_Overcast_Max = 0.1;
        m_Fog_Min = 0.0;
        m_Fog_Max = 0.0;
        m_Rain_Min = 0.0;
        m_Rain_Max = 0.0;
        m_Snowfall_Min = 0.0;
        m_Snowfall_Max = 0.0;
        
        m_WindSpeed_Min = 1.0;
        m_WindSpeed_Max = 2.0;
        m_WindDirection_Min = 0.0;
        m_WindDirection_Max = 360.0;
        
        m_WindFunc_Min = 0.1;
        m_WindFunc_Max = 1.0;
        m_WindFunc_Speed = 20.0;
        
        m_UseDynVolFog = 0;
        m_VolFogDistanceDensity_Min = 0.0;
        m_VolFogDistanceDensity_Max = 0.0;
        m_VolFogHeightDensity_Min = 0.0;
        m_VolFogHeightDensity_Max = 0.0;
        m_VolFogHeightBias = 0.0;
        m_FogTransitionTime = 30.0;
        
        m_UseSnowflakeScale = 0;
        m_SnowflakeScale_Min = 1.0;
        m_SnowflakeScale_Max = 1.0;
        
        m_SnowfallThreshold_Min = 0.6;
        m_SnowfallThreshold_Max = 1.0;
        m_SnowfallThreshold_Timeout = 30.0;
        
        m_RainThreshold_Min = 0.3;
        m_RainThreshold_Max = 1.0;
        m_RainThreshold_Timeout = 30.0;
        
        m_ThunderstormDensity = 0.0;
        m_ThunderstormThreshold = 0.8;
        m_LightningTimeout = 45.0;
        
        m_UseEnvironmentTemperature = 0;
        m_EnvironmentTemperature = 15.0;
    }
    
    bool IsValid()
    {
        bool valid = true;
        
        if (m_Overcast_Min < 0.0 || m_Overcast_Max > 1.0 || m_Overcast_Min > m_Overcast_Max)
            valid = false;
        
        if (m_Fog_Min < 0.0 || m_Fog_Max > 1.0 || m_Fog_Min > m_Fog_Max)
            valid = false;
        
        if (m_Rain_Min < 0.0 || m_Rain_Max > 1.0 || m_Rain_Min > m_Rain_Max)
            valid = false;
        
        if (m_Snowfall_Min < 0.0 || m_Snowfall_Max > 1.0 || m_Snowfall_Min > m_Snowfall_Max)
            valid = false;
        
        if (m_WindSpeed_Min < 0.0 || m_WindSpeed_Max > 20.0 || m_WindSpeed_Min > m_WindSpeed_Max)
            valid = false;
        
        if (m_WindDirection_Min < 0.0 || m_WindDirection_Max > 360.0 || m_WindDirection_Min > m_WindDirection_Max)
            valid = false;
        
        if (m_TransitionTime_Min < 0.0 || m_TransitionTime_Max < m_TransitionTime_Min)
            valid = false;
        
        if (m_MinDuration_Min <= 0.0 || m_MinDuration_Max < m_MinDuration_Min)
            valid = false;
        
        return valid;
    }
    
    ref WU_WeatherConfig GenerateRandomValues()
    {
        ref WU_WeatherConfig config = new WU_WeatherConfig();
        
        config.m_TransitionTime = RandomFloatSafe(m_TransitionTime_Min, m_TransitionTime_Max);
        config.m_MinDuration = RandomFloatSafe(m_MinDuration_Min, m_MinDuration_Max);
        
        config.m_Overcast = RandomFloatSafe(m_Overcast_Min, m_Overcast_Max);
        config.m_Fog = RandomFloatSafe(m_Fog_Min, m_Fog_Max);
        config.m_Rain = RandomFloatSafe(m_Rain_Min, m_Rain_Max);
        config.m_Snowfall = RandomFloatSafe(m_Snowfall_Min, m_Snowfall_Max);
        
        config.m_WindSpeed = RandomFloatSafe(m_WindSpeed_Min, m_WindSpeed_Max);
        config.m_WindDirection = RandomFloatSafe(m_WindDirection_Min, m_WindDirection_Max);
        
        config.m_WindFunc_Min = m_WindFunc_Min;
        config.m_WindFunc_Max = m_WindFunc_Max;
        config.m_WindFunc_Speed = m_WindFunc_Speed;
        
        if (m_UseDynVolFog == 1)
        {
            config.m_VolFogDistanceDensity = RandomFloatSafe(m_VolFogDistanceDensity_Min, m_VolFogDistanceDensity_Max);
            config.m_VolFogHeightDensity = RandomFloatSafe(m_VolFogHeightDensity_Min, m_VolFogHeightDensity_Max);
            config.m_VolFogHeightBias = m_VolFogHeightBias;
        }
        else
        {
            config.m_VolFogDistanceDensity = 0.0;
            config.m_VolFogHeightDensity = 0.0;
            config.m_VolFogHeightBias = 0.0;
        }
        
        if (m_UseSnowflakeScale == 1)
        {
            config.m_SnowflakeScale = RandomFloatSafe(m_SnowflakeScale_Min, m_SnowflakeScale_Max);
        }
        else
        {
            config.m_SnowflakeScale = 1.0;
        }
        
        config.m_ThunderstormDensity = m_ThunderstormDensity;
        config.m_ThunderstormThreshold = m_ThunderstormThreshold;
        config.m_LightningTimeout = m_LightningTimeout;
        
        config.m_SnowfallThreshold_Min = m_SnowfallThreshold_Min;
        config.m_SnowfallThreshold_Max = m_SnowfallThreshold_Max;
        config.m_SnowfallThreshold_Timeout = m_SnowfallThreshold_Timeout;
        
        config.m_RainThreshold_Min = m_RainThreshold_Min;
        config.m_RainThreshold_Max = m_RainThreshold_Max;
        config.m_RainThreshold_Timeout = m_RainThreshold_Timeout;
        
        config.m_FogTransitionTime = m_FogTransitionTime;
        config.m_UseEnvironmentTemperature = m_UseEnvironmentTemperature;
        config.m_EnvironmentTemperature = m_EnvironmentTemperature;
        
        return config;
    }
    
    private float RandomFloatSafe(float min, float max)
    {
        if (min == max)
            return min;
        
        if (min > max)
            return min;
        
        float value = Math.RandomFloat(min, max);
        
        if (value < 0.0 && min >= 0.0)
            value = 0.0;
        
        return value;
    }
};

class WU_WeatherConfig
{
    float m_TransitionTime;
    float m_MinDuration;
    
    float m_Overcast;
    float m_Fog;
    float m_Rain;
    float m_Snowfall;
    
    float m_WindSpeed;
    float m_WindDirection;
    
    float m_WindFunc_Min;
    float m_WindFunc_Max;
    float m_WindFunc_Speed;
    
    float m_VolFogDistanceDensity;
    float m_VolFogHeightDensity;
    float m_VolFogHeightBias;
    float m_FogTransitionTime;
    
    float m_SnowflakeScale;
    
    float m_SnowfallThreshold_Min;
    float m_SnowfallThreshold_Max;
    float m_SnowfallThreshold_Timeout;
    
    float m_RainThreshold_Min;
    float m_RainThreshold_Max;
    float m_RainThreshold_Timeout;
    
    float m_ThunderstormDensity;
    float m_ThunderstormThreshold;
    float m_LightningTimeout;
    
    int m_UseEnvironmentTemperature;
    float m_EnvironmentTemperature;
    
    void WU_WeatherConfig()
    {
        SetDefaultValues();
    }
    
    void SetDefaultValues()
    {
        m_TransitionTime = 180.0;
        m_MinDuration = 900.0;
        m_Overcast = 0.1;
        m_Fog = 0.0;
        m_Rain = 0.0;
        m_Snowfall = 0.0;
        m_WindSpeed = 1.0;
        m_WindDirection = 45.0;
        m_WindFunc_Min = 0.1;
        m_WindFunc_Max = 1.0;
        m_WindFunc_Speed = 20.0;
        m_VolFogDistanceDensity = 0.0;
        m_VolFogHeightDensity = 0.0;
        m_VolFogHeightBias = 0.0;
        m_FogTransitionTime = 30.0;
        m_SnowflakeScale = 1.0;
        m_SnowfallThreshold_Min = 0.6;
        m_SnowfallThreshold_Max = 1.0;
        m_SnowfallThreshold_Timeout = 30.0;
        m_RainThreshold_Min = 0.3;
        m_RainThreshold_Max = 1.0;
        m_RainThreshold_Timeout = 30.0;
        m_ThunderstormDensity = 0.0;
        m_ThunderstormThreshold = 0.8;
        m_LightningTimeout = 45.0;
        m_UseEnvironmentTemperature = 0;
        m_EnvironmentTemperature = 15.0;
    }
    
    bool IsValid()
    {
        bool valid = true;
        
        if (m_Overcast < 0.0 || m_Overcast > 1.0)
            valid = false;
        
        if (m_Fog < 0.0 || m_Fog > 1.0)
            valid = false;
        
        if (m_Rain < 0.0 || m_Rain > 1.0)
            valid = false;
        
        if (m_Snowfall < 0.0 || m_Snowfall > 1.0)
            valid = false;
        
        if (m_WindSpeed < 0.0 || m_WindSpeed > 20.0)
            valid = false;
        
        if (m_WindDirection < 0.0 || m_WindDirection > 360.0)
            valid = false;
        
        if (m_TransitionTime < 0.0)
            valid = false;
        
        if (m_MinDuration <= 0.0)
            valid = false;
        
        return valid;
    }
};

class WU_WeatherSchedule
{
    string Time;
    string Preset;
    int Chance;
    
    void WU_WeatherSchedule()
    {
        Time = "00:00";
        Preset = "clear";
        Chance = 100;
    }
    
    bool IsValid()
    {
        return Time != "" && Preset != "" && Chance >= 0 && Chance <= 100;
    }
};
