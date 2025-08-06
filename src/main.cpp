// READ README.md FOR IMPORTANT INSTRUCTIONS ELSE THE CODE WILL NOT WORK

#include "bluetooth_logger/bluetooth_logger.h"

int main(void) {
    log_enabled(TRUE);
    log_set_level(LOG_INFO);

    BluetoothLogger::init();

    return 0;
}
