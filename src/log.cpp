#include "log.h"


void LogToFile(const std::string& message) {
    std::string logFilePath = "TFS.log";
    std::ofstream logFile;


    logFile.open(logFilePath, std::ios_base::app);
    if (logFile.is_open()) {
        logFile << message << std::endl;
        logFile.close();
    } else {
        std::cerr << "Failed to open log file: " << logFilePath << std::endl;
    }
}
