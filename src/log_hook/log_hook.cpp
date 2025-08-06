#include "log_hook.h"
#include "logger.h"
#include "camera/camera.h"

void processLog(const std::string logMessage) {

    int dashPos = logMessage.find('-');
    if (dashPos == std::string::npos) {
        return;
    }
    std::string suffix = logMessage.substr(dashPos + 1);

    if (suffix == "Sound Threshold") {
        //cam::triggerMotionDetection();
    } else if (suffix == "Motion Threshold") {
        cam::triggerMotionDetection();
    } else if (suffix == "Vibration Threshold") {
        //cam::triggerMotionDetection();
    } else {
        log_info("log", "Ignoring corrupted log");
    }

}



