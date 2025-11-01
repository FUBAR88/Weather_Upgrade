class WU_PlayerZonePlugin
{
    private static ref WU_PlayerZonePlugin m_Instance;
    private ref map<string, bool> m_PlayerInZone;
    private ref map<string, string> m_PlayerCurrentZone;
    private float m_CheckInterval = 5.0;
    private float m_TimeSinceLastCheck = 0.0;
    
    void WU_PlayerZonePlugin()
    {
        m_PlayerInZone = new map<string, bool>();
        m_PlayerCurrentZone = new map<string, string>();
    }
    
    static WU_PlayerZonePlugin GetInstance()
    {
        if (!m_Instance)
        {
            m_Instance = new WU_PlayerZonePlugin();
        }
        return m_Instance;
    }
    
    void Update(float deltaTime)
    {
        m_TimeSinceLastCheck += deltaTime;
        
        if (m_TimeSinceLastCheck < m_CheckInterval)
            return;
        
        m_TimeSinceLastCheck = 0.0;
        
        if (!WU_ENABLE_ZONE_TEMPERATURE_CONTROL)
            return;
        
        WU_ConfigZoneTemperature zoneConfig = GetWU_ZoneTemperatureConfig();
        if (!zoneConfig || !zoneConfig.Zones || zoneConfig.Zones.Count() == 0)
            return;
        
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        
        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.IsAlive())
                continue;
            
            PlayerIdentity identity = player.GetIdentity();
            if (!identity)
                continue;
            
            string playerID = identity.GetId();
            string playerName = identity.GetName();
            vector playerPos = player.GetPosition();
            
            bool foundZone = false;
            string currentZoneName = "";
            ref WU_TemperatureZone currentZone;
            
            for (int i = 0; i < zoneConfig.Zones.Count(); i++)
            {
                ref WU_TemperatureZone zone = zoneConfig.Zones.Get(i);
                if (!zone || !zone.Position || zone.Position.Count() < 3)
                    continue;
                
                float zoneX = zone.Position.Get(0);
                float zoneY = zone.Position.Get(1);
                float zoneZ = zone.Position.Get(2);
                
                float deltaX = playerPos[0] - zoneX;
                float deltaZ = playerPos[2] - zoneZ;
                float horizontalDist = Math.Sqrt(deltaX * deltaX + deltaZ * deltaZ);
                
                if (horizontalDist <= zone.Radius)
                {
                    bool insideVertically = true;
                    
                    if (zone.Height > 0)
                    {
                        float playerAltitude = playerPos[1];
                        float zoneFloor = zoneY;
                        float zoneCeiling = zoneY + zone.Height;
                        insideVertically = (playerAltitude >= zoneFloor && playerAltitude <= zoneCeiling);
                    }
                    
                    if (insideVertically)
                    {
                        foundZone = true;
                        currentZoneName = zone.Name;
                        currentZone = zone;
                        break;
                    }
                }
            }
            
            bool wasInZone = m_PlayerInZone.Contains(playerID) && m_PlayerInZone.Get(playerID);
            string previousZone = "";
            if (m_PlayerCurrentZone.Contains(playerID))
                previousZone = m_PlayerCurrentZone.Get(playerID);
            
            float baseTemp;
            float zoneTemp;
            string posStr;
            
            if (foundZone && !wasInZone)
            {
                m_PlayerInZone.Set(playerID, true);
                m_PlayerCurrentZone.Set(playerID, currentZoneName);
                
                baseTemp = GetGame().GetMission().GetWorldData().GetBaseEnvTemperatureAtObject(player);
                zoneTemp = currentZone.GetRandomTemperature();
                posStr = playerPos[0].ToString() + " " + playerPos[1].ToString() + " " + playerPos[2].ToString();
                
                string heightStr = "disabled";
                if (currentZone.Height > 0)
                    heightStr = currentZone.Height.ToString() + "m";
                
                WU_Logger.Info("[ZONE ENTRY] Player '" + playerName + "' entered zone '" + currentZoneName + "' | Pos=[" + posStr + "] Height=" + heightStr);
                WU_Logger.Info("[ZONE ENTRY] BaseTemp=" + baseTemp.ToString() + "°C -> TargetZoneTemp=" + zoneTemp.ToString() + "°C (Range: " + currentZone.Temperature_Min.ToString() + "~" + currentZone.Temperature_Max.ToString() + ")");
            }
            else if (foundZone && wasInZone && currentZoneName == previousZone)
            {
                baseTemp = GetGame().GetMission().GetWorldData().GetBaseEnvTemperatureAtObject(player);
                zoneTemp = currentZone.GetRandomTemperature();
                posStr = playerPos[0].ToString() + " " + playerPos[1].ToString() + " " + playerPos[2].ToString();
                
                WU_Logger.Debug("[ZONE STATUS] Player '" + playerName + "' in zone '" + currentZoneName + "' | Pos=[" + posStr + "] BaseTemp=" + baseTemp.ToString() + "°C ZoneTemp=" + zoneTemp.ToString() + "°C");
            }
            else if (!foundZone && wasInZone)
            {
                m_PlayerInZone.Set(playerID, false);
                m_PlayerCurrentZone.Remove(playerID);
                
                baseTemp = GetGame().GetMission().GetWorldData().GetBaseEnvTemperatureAtObject(player);
                posStr = playerPos[0].ToString() + " " + playerPos[1].ToString() + " " + playerPos[2].ToString();
                
                WU_Logger.Info("[ZONE EXIT] Player '" + playerName + "' left zone '" + previousZone + "' | Pos=[" + posStr + "] BaseTemp=" + baseTemp.ToString() + "°C now applied");
            }
        }
    }
};

