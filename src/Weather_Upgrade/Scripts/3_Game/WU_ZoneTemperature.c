class WU_TemperatureZone
{
    string Name;
    ref array<float> Position;
    float Radius;
    float Height;
    float Temperature_Min;
    float Temperature_Max;
    
    void WU_TemperatureZone()
    {
        Name = "Zone";
        Position = new array<float>();
        Position.Insert(0.0);
        Position.Insert(0.0);
        Position.Insert(0.0);
        Radius = 500.0;
        Height = 100.0;
        Temperature_Min = 15.0;
        Temperature_Max = 20.0;
    }
    
    float GetRandomTemperature()
    {
        return Math.RandomFloat(Temperature_Min, Temperature_Max);
    }
};

class WU_ConfigZoneTemperature
{
    static const string CONFIG_FILE = "$profile:\\Weather_Upgrade\\WU_ZoneTemperatureControl.json";
    
    ref array<ref WU_TemperatureZone> Zones;
    
    void WU_ConfigZoneTemperature()
    {
        Zones = new array<ref WU_TemperatureZone>();
    }
    
    bool Load()
    {
        if (!FileExist(CONFIG_FILE))
        {
            WU_Logger.Warning("[ZONE TEMP] Configuration file not found: " + CONFIG_FILE);
            return false;
        }
        
        JsonFileLoader<WU_ConfigZoneTemperature>.JsonLoadFile(CONFIG_FILE, this);
        
        if (!Zones || Zones.Count() == 0)
        {
            WU_Logger.Warning("[ZONE TEMP] No temperature zones loaded");
            return true;
        }
        
        WU_Logger.Info("[ZONE TEMP] Zone temperature control loaded");
        WU_Logger.Info("[ZONE TEMP] - Zones Loaded: " + Zones.Count().ToString());
        
        for (int i = 0; i < Zones.Count(); i++)
        {
            ref WU_TemperatureZone zone = Zones.Get(i);
            if (zone && zone.Position && zone.Position.Count() >= 3)
            {
                string posStr = zone.Position.Get(0).ToString() + " " + zone.Position.Get(1).ToString() + " " + zone.Position.Get(2).ToString();
                string heightStr;
                if (zone.Height <= 0)
                {
                    heightStr = "disabled";
                }
                else
                {
                    heightStr = zone.Height.ToString() + "m";
                }
                WU_Logger.Info("[ZONE TEMP] Zone '" + zone.Name + "': Pos=[" + posStr + "] Radius=" + zone.Radius.ToString() + "m Height=" + heightStr + " Temp=" + zone.Temperature_Min.ToString() + "~" + zone.Temperature_Max.ToString() + "°C");
            }
        }
        
        return true;
    }
    
    void CreateDefault()
    {
        FileHandle file = OpenFile(CONFIG_FILE, FileMode.WRITE);
        if (file != 0)
        {
            FPrint(file, "{\n");
            FPrint(file, "    \"Zones\": [\n");
            FPrint(file, "        {\n");
            FPrint(file, "            \"Name\": \"Sky Bunker Alpha\",\n");
            FPrint(file, "            \"Position\": [\n");
            FPrint(file, "                5000,\n");
            FPrint(file, "                2000,\n");
            FPrint(file, "                7000\n");
            FPrint(file, "            ],\n");
            FPrint(file, "            \"Radius\": 1200,\n");
            FPrint(file, "            \"Height\": 250,\n");
            FPrint(file, "            \"Temperature_Min\": -20,\n");
            FPrint(file, "            \"Temperature_Max\": -15\n");
            FPrint(file, "        },\n");
            FPrint(file, "        {\n");
            FPrint(file, "            \"Name\": \"Underground Facility\",\n");
            FPrint(file, "            \"Position\": [\n");
            FPrint(file, "                3000,\n");
            FPrint(file, "                10,\n");
            FPrint(file, "                4000\n");
            FPrint(file, "            ],\n");
            FPrint(file, "            \"Radius\": 300,\n");
            FPrint(file, "            \"Height\": 50,\n");
            FPrint(file, "            \"Temperature_Min\": 5,\n");
            FPrint(file, "            \"Temperature_Max\": 8\n");
            FPrint(file, "        }\n");
            FPrint(file, "    ]\n");
            FPrint(file, "}\n");
            CloseFile(file);
            WU_Logger.Info("[ZONE TEMP] Default zone temperature config created");
        }
    }
};

ref WU_ConfigZoneTemperature g_WU_ZoneTemperatureConfig;

static WU_ConfigZoneTemperature GetWU_ZoneTemperatureConfig()
{
    if (!g_WU_ZoneTemperatureConfig)
    {
        g_WU_ZoneTemperatureConfig = new WU_ConfigZoneTemperature();
    }
    return g_WU_ZoneTemperatureConfig;
}

