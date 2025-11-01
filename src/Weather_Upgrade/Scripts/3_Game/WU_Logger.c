class WU_Logger
{
    private static string m_LogFileName = "Weather_Upgrade.log";
    private static string m_DebugLogFileName = "Weather_Upgrade_Debug.log";
    private static string m_LogPath = "$profile:\\Weather_Upgrade\\Logs\\";
    private static bool m_Initialized = false;
    private static bool m_EnableLogging = true;
    private static bool m_EnableDebugLogging = false;
    
    static void Initialize()
    {
        if (m_Initialized)
            return;
        
        if (!FileExist("$profile:\\Weather_Upgrade\\"))
        {
            MakeDirectory("$profile:\\Weather_Upgrade\\");
        }
        
        if (!FileExist(m_LogPath))
        {
            MakeDirectory(m_LogPath);
        }
        
        m_Initialized = true;
    }
    
    static void SetLogFileName(string fileName)
    {
        m_LogFileName = fileName;
        m_DebugLogFileName = "Debug_" + fileName;
    }
    
    static void SetEnableLogging(bool enable)
    {
        m_EnableLogging = enable;
    }
    
    static void SetEnableDebugLogging(bool enable)
    {
        m_EnableDebugLogging = enable;
    }
    
    static void Info(string message)
    {
        if (!m_EnableLogging)
            return;
        
        WriteToFile(m_LogFileName, "[INFO]", message);
        
        if (m_EnableDebugLogging)
        {
            WriteToFile(m_DebugLogFileName, "[INFO]", message);
        }
    }
    
    static void Warning(string message)
    {
        if (!m_EnableLogging)
            return;
        
        WriteToFile(m_LogFileName, "[WARNING]", message);
        
        if (m_EnableDebugLogging)
        {
            WriteToFile(m_DebugLogFileName, "[WARNING]", message);
        }
    }
    
    static void Error(string message)
    {
        if (!m_EnableLogging)
            return;
        
        WriteToFile(m_LogFileName, "[ERROR]", message);
        
        if (m_EnableDebugLogging)
        {
            WriteToFile(m_DebugLogFileName, "[ERROR]", message);
        }
    }
    
    static void Debug(string message)
    {
        if (!m_EnableDebugLogging)
            return;
        
        WriteToFile(m_DebugLogFileName, "[DEBUG]", message);
    }
    
    private static void WriteToFile(string fileName, string level, string message)
    {
        if (!m_Initialized)
            return;
        
        if (fileName == m_DebugLogFileName && !m_EnableDebugLogging)
            return;
        
        string fullPath = m_LogPath + fileName;
        FileHandle file = OpenFile(fullPath, FileMode.APPEND);
        
        if (file != 0)
        {
            int year, month, day, hour, minute, second;
            GetYearMonthDay(year, month, day);
            GetHourMinuteSecond(hour, minute, second);
            
            string timestamp = string.Format("%1-%2-%3 %4:%5:%6", year, month, day, hour, minute, second);
            string logEntry = timestamp + " " + level + " " + message + "\n";
            
            FPrint(file, logEntry);
            CloseFile(file);
        }
    }
};
