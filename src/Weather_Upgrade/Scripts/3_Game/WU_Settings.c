class WU_Settings
{
    static const string CONFIG_FILE = "$profile:\\Weather_Upgrade\\WU_Settings.json";
    
    int EnableLogging;
    int EnableDebugLogging;
    int KeepLogsDays;
    int AutoWeatherChanges;
    int EnableTemperatureControl;
    int EnableZoneTemperatureControl;
    
    float DriftCorrectionTime;
    float DriftCorrectionCooldown;
    float DriftTolerance;
    
    void WU_Settings()
    {
        EnableLogging = 1;
        EnableDebugLogging = 0;
        KeepLogsDays = 30;
        AutoWeatherChanges = 0;
        EnableTemperatureControl = 1;
        EnableZoneTemperatureControl = 0;
        
        DriftCorrectionTime = 60.0;
        DriftCorrectionCooldown = 30.0;
        DriftTolerance = 0.1;
    }
    
    bool Load()
    {
        if (!FileExist(CONFIG_FILE))
        {
            WU_Logger.Warning("[SETTINGS] Configuration file not found: " + CONFIG_FILE);
            return false;
        }
        
        JsonFileLoader<WU_Settings>.JsonLoadFile(CONFIG_FILE, this);
        
        WU_Logger.Info("[SETTINGS] Master settings loaded");
        WU_Logger.Info("[SETTINGS] - EnableLogging: " + EnableLogging.ToString());
        WU_Logger.Info("[SETTINGS] - EnableDebugLogging: " + EnableDebugLogging.ToString());
        WU_Logger.Info("[SETTINGS] - KeepLogsDays: " + KeepLogsDays.ToString());
        WU_Logger.Info("[SETTINGS] - AutoWeatherChanges: " + AutoWeatherChanges.ToString());
        WU_Logger.Info("[SETTINGS] - EnableTemperatureControl: " + EnableTemperatureControl.ToString());
        WU_Logger.Info("[SETTINGS] - EnableZoneTemperatureControl: " + EnableZoneTemperatureControl.ToString());
        WU_Logger.Info("[SETTINGS] - DriftCorrectionTime: " + DriftCorrectionTime.ToString() + "s");
        WU_Logger.Info("[SETTINGS] - DriftCorrectionCooldown: " + DriftCorrectionCooldown.ToString() + "s");
        WU_Logger.Info("[SETTINGS] - DriftTolerance: " + DriftTolerance.ToString());
        
        return true;
    }
    
    void CreateDefault()
    {
        if (!FileExist("$profile:\\Weather_Upgrade\\"))
        {
            MakeDirectory("$profile:\\Weather_Upgrade\\");
        }
        
        FileHandle file = OpenFile(CONFIG_FILE, FileMode.WRITE);
        if (file != 0)
        {
            FPrint(file, "{\n");
            FPrint(file, "    \"EnableLogging\": 1,\n");
            FPrint(file, "    \"EnableDebugLogging\": 0,\n");
            FPrint(file, "    \"KeepLogsDays\": 30,\n");
            FPrint(file, "    \"AutoWeatherChanges\": 0,\n");
            FPrint(file, "    \"EnableTemperatureControl\": 1,\n");
            FPrint(file, "    \"EnableZoneTemperatureControl\": 0,\n");
            FPrint(file, "    \"DriftCorrectionTime\": 60,\n");
            FPrint(file, "    \"DriftCorrectionCooldown\": 30,\n");
            FPrint(file, "    \"DriftTolerance\": 0.1\n");
            FPrint(file, "}\n");
            CloseFile(file);
            WU_Logger.Info("[SETTINGS] Default master settings created");
        }
    }
};

