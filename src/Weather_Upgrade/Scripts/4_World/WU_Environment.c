modded class Environment
{
    // Track zone entry/exit for richer debug logs
    private bool m_WU_InZone;
    private string m_WU_LastZoneName;
    private float m_WU_LastZoneTemp;
    private int m_WU_NextTempLogTick;
    private static bool m_WU_DiagnosticLogged;

    private void WU_LogPlayerTempPeriodic(string playerName, vector playerPos, float appliedTemp, float baseTemp, string zoneName)
    {
        int now = GetGame().GetTime();
        if (now >= m_WU_NextTempLogTick)
        {
            string posStr = playerPos[0].ToString() + " " + playerPos[1].ToString() + " " + playerPos[2].ToString();
            string zoneStr = zoneName;
            if (zoneStr == "")
                zoneStr = "None";
            WU_Logger.Debug("[PLAYER TEMP] '" + playerName + "' Pos=[" + posStr + "] Zone='" + zoneStr + "' BaseTemp=" + baseTemp.ToString() + "°C AppliedTemp=" + appliedTemp.ToString() + "°C");
            m_WU_NextTempLogTick = now + 5000; // 5 seconds
        }
    }

    override float GetEnvironmentTemperature()
    {
        if (!m_WU_DiagnosticLogged)
        {
            WU_Logger.Info("[DIAGNOSTIC] Weather_Upgrade Environment.GetEnvironmentTemperature() override is ACTIVE");
            m_WU_DiagnosticLogged = true;
        }
        
        float baseTemp = super.GetEnvironmentTemperature();
        
        if (WU_ENABLE_ZONE_TEMPERATURE_CONTROL && m_Player)
        {
            WU_ConfigZoneTemperature zoneConfig = GetWU_ZoneTemperatureConfig();
            if (zoneConfig && zoneConfig.Zones && zoneConfig.Zones.Count() > 0)
            {
                PlayerBase player = PlayerBase.Cast(m_Player);
                if (player)
                {
                    vector playerPos = player.GetPosition();
                    
                    for (int i = 0; i < zoneConfig.Zones.Count(); i++)
                    {
                        ref WU_TemperatureZone zone = zoneConfig.Zones.Get(i);
                        if (zone && zone.Position && zone.Position.Count() >= 3)
                        {
                            float zoneX = zone.Position.Get(0);
                            float zoneY = zone.Position.Get(1);
                            float zoneZ = zone.Position.Get(2);
                            
                            float deltaX = playerPos[0] - zoneX;
                            float deltaZ = playerPos[2] - zoneZ;
                            float horizontalDist = Math.Sqrt(deltaX * deltaX + deltaZ * deltaZ);
                            
                            if (horizontalDist <= zone.Radius)
                            {
                                float playerAltitude = playerPos[1];
                                float zoneFloor = zoneY;
                                float zoneCeiling = zoneY + zone.Height;
                                bool ignoreHeight = false;
                                
                                if (zone.Height <= 0)
                                {
                                    ignoreHeight = true;
                                }
                                
                                if (ignoreHeight)
                                {
                                    float zoneTempNoHeight = zone.GetRandomTemperature();
                                    string pname = "Survivor";
                                    if (player.GetIdentity())
                                    {
                                        pname = player.GetIdentity().GetName();
                                    }
                                    if (!m_WU_InZone || m_WU_LastZoneName != zone.Name)
                                    {
                                        WU_Logger.Debug("[ZONE TEMP] Player '" + pname + "' entered zone '" + zone.Name + "' (height disabled) | BaseTemp=" + baseTemp.ToString() + "°C -> ZoneTemp=" + zoneTempNoHeight.ToString() + "°C");
                                        m_WU_InZone = true;
                                        m_WU_LastZoneName = zone.Name;
                                    }
                                    m_WU_LastZoneTemp = zoneTempNoHeight;
                                    WU_Logger.Debug("[ZONE TEMP] Player in zone '" + zone.Name + "' (height disabled) - Temperature: " + zoneTempNoHeight.ToString() + "°C (Range: " + zone.Temperature_Min.ToString() + "~" + zone.Temperature_Max.ToString() + ")");
                                    WU_LogPlayerTempPeriodic(pname, playerPos, zoneTempNoHeight, baseTemp, zone.Name);
                                    return zoneTempNoHeight;
                                }
                                else
                                {
                                    if (playerAltitude >= zoneFloor && playerAltitude <= zoneCeiling)
                                    {
                                        float zoneTemp = zone.GetRandomTemperature();
                                        string pname2 = "Survivor";
                                        if (player.GetIdentity())
                                        {
                                            pname2 = player.GetIdentity().GetName();
                                        }
                                        if (!m_WU_InZone || m_WU_LastZoneName != zone.Name)
                                        {
                                            WU_Logger.Debug("[ZONE TEMP] Player '" + pname2 + "' entered zone '" + zone.Name + "' | BaseTemp=" + baseTemp.ToString() + "°C -> ZoneTemp=" + zoneTemp.ToString() + "°C");
                                            m_WU_InZone = true;
                                            m_WU_LastZoneName = zone.Name;
                                        }
                                        m_WU_LastZoneTemp = zoneTemp;
                                        WU_Logger.Debug("[ZONE TEMP] Player in zone '" + zone.Name + "' - Temperature: " + zoneTemp.ToString() + "°C (Range: " + zone.Temperature_Min.ToString() + "~" + zone.Temperature_Max.ToString() + ")");
                                        WU_LogPlayerTempPeriodic(pname2, playerPos, zoneTemp, baseTemp, zone.Name);
                                        return zoneTemp;
                                    }
                                }
                            }
                        }
                    }
                    // If we reach here, player is not in any zone; if previously in one, log exit
                    if (m_WU_InZone)
                    {
                        string pname3 = "Survivor";
                        if (player.GetIdentity())
                        {
                            pname3 = player.GetIdentity().GetName();
                        }
                        WU_Logger.Debug("[ZONE TEMP] Player '" + pname3 + "' left zone '" + m_WU_LastZoneName + "' | BaseTemp=" + baseTemp.ToString() + "°C now applied");
                        m_WU_InZone = false;
                        m_WU_LastZoneName = "";
                        m_WU_LastZoneTemp = 0;
                    }
                    // periodic log outside zones
                    string pname4 = "Survivor";
                    if (player.GetIdentity())
                    {
                        pname4 = player.GetIdentity().GetName();
                    }
                    WU_LogPlayerTempPeriodic(pname4, playerPos, baseTemp, baseTemp, "");
                }
            }
        }
        
        if (WU_USE_TEMPERATURE_OVERRIDE)
        {
            WU_Logger.Debug("[WEATHER TEMP] Weather temperature override: " + WU_GLOBAL_TEMPERATURE_OVERRIDE.ToString() + "°C");
            return WU_GLOBAL_TEMPERATURE_OVERRIDE;
        }
        
        return baseTemp;
    }
};

