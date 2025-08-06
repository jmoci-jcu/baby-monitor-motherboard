// READ README.md FOR IMPORTANT INSTRUCTIONS ELSE THE CODE WILL NOT WORK

#include "bluetooth_logger/bluetooth_logger.h"
#include "log_hook/log_hook.h"
#include "logger.h"

int main(void) {
    log_enabled(TRUE);
    log_set_level(LOG_INFO);
    processLog("99441-Sound Threshold");
    BluetoothLogger::init();
}