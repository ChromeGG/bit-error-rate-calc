#include <iostream>

void createLogger(std::string fileName) {
    logFile.open(fileName.c_str(), std::ios_base::app);
    if (!logFile.good() == true) {
        std::cout << "Cannot open log file "
                  << "log.txt" << std::endl;
    }
    persistLog("Log file opened");
}

void persistLog(std::string message) {
    time_t currentTime = time(NULL);
    std::string log;

    std::string timeString = (std::string)asctime(localtime(&currentTime));
    timeString = timeString.substr(0, timeString.length() - 1);
    log = (std::string) "[" + timeString + "] :" + message + "\n";
    logFile << log.c_str();
    logFile.flush();

    std::cout << log.c_str();
    std::cout.flush();
}

void closeLog(void) {
    persistLog("Log file closed");
    logFile.close();
}