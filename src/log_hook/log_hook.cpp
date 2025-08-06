#include "log_hook.h"
#include "logger.h"


void processLog(const std::string logMessage) {

    int dashPos = logMessage.find('-');
    if (dashPos == std::string::npos) {
        return;
    }
    std::string suffix = logMessage.substr(dashPos + 1);

    if (suffix == "Sound Threshold") {
        log_info("log", "Sound");
    } else if (suffix == "Motion Threshold") {
        log_info("log", "Motion");
    } else if (suffix == "Vibration Threshold") {
        log_info("log", "Vibration");
    } else {
        log_info("log", "Ignore corrupted log");
    }

}



