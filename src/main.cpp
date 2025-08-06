// NOTE : This requires some configuration on the RN4871
// Use command : NA,06,55E405D2AF9FA98FE54A7DFE43535349 to set the
// advertisement UUID packet so it can be identified.

#include "bluetooth_logger/bluetooth_logger.h"

int main(void) {
    log_enabled(TRUE);
    log_set_level(LOG_INFO);

    BluetoothLogger::init();

    return 0;
}
