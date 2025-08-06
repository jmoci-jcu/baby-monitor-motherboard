// READ README.md FOR IMPORTANT INSTRUCTIONS ELSE THE CODE WILL NOT WORK

#include "bluetooth_logger/bluetooth_logger.h"
#include "log_hook/log_hook.h"
#include "logger.h"
#include "camera/camera.h"

int main(void) {
    log_enabled(TRUE);
    log_set_level(LOG_INFO);
    cam::init();
    GMainContext *gContext = BluetoothLogger::init();
    while(true){
        cam::cameraLoop();
        g_main_context_iteration(gContext,false);
    }
    return 0;
}
