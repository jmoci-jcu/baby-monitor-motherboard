// NOTE : This requires some configuration on the RN4871
// Use command : NA,06,55E405D2AF9FA98FE54A7DFE43535349 to set the
// advertisement UUID packet so it can be identified.


#include "bluetooth_logger.h"
#include <iostream>
#include <sstream>
#include <vector>
#include "log_hook/log_hook.h"

Adapter *BluetoothLogger::default_adapter = NULL;

void BluetoothLogger::on_connection_state_changed(Device *device, ConnectionState state, const GError *error) {
    if (error != NULL) {
        log_info(TAG, "(dis)connect failed (error %d: %s)", error->code, error->message);
        return;
    }

    log_info(TAG, "'%s' (%s) state: %s (%d)", binc_device_get_name(device), binc_device_get_address(device),
              binc_device_get_connection_state_name(device), state);

    if (state == BINC_DISCONNECTED) {
        // Remove devices immediately of they are not bonded
        if (binc_device_get_bonding_state(device) != BINC_BONDED) {
            binc_adapter_remove_device(default_adapter, device);
        }
    }
}


// Notification callback: prints incoming data
void BluetoothLogger::on_notify(Device *device, Characteristic *ch, const GByteArray *value) {
    //read through string and separate by \n
    //std::vector<std::string> lines;
    std::string logs = (char*)value->data;
    std::istringstream stream(logs);
    std::string line;

    while(std::getline(stream,line)){
        processLog(line); //this isn't great but nice and simple
    }
}

// Called when services are resolved: subscribe to TX notifications
void BluetoothLogger::on_services_resolved(Device *dev) {
    log_info(TAG, "Services resolved -> start notify");
    binc_device_start_notify(dev, UART_SERVICE_UUID, UART_TX_CHAR_UUID);
}



void BluetoothLogger::on_bonding_state_changed(Device *device, BondingState new_state, BondingState old_state, const GError *error) {
    log_info(TAG, "bonding state changed from %d to %d", old_state, new_state);
}

void BluetoothLogger::on_notification_state_changed(Device *device, Characteristic *characteristic, const GError *error) {
    const char *uuid = binc_characteristic_get_uuid(characteristic);

    if (error != NULL) {
        log_info(TAG, "notifying <%s> failed (error %d: %s)", uuid, error->code, error->message);
        return;
    }

    log_info(TAG, "<%s> notifying %s", uuid, binc_characteristic_is_notifying(characteristic) ? "true" : "false");
}

void BluetoothLogger::on_desc_read(Device *device, Descriptor *descriptor, const GByteArray *byteArray, const GError *error) {
    log_info(TAG, "on descriptor read");
    Parser *parser = parser_create(byteArray, LITTLE_ENDIAN);
    GString *parsed_string = parser_get_string(parser);
    log_info(TAG, "CUD %s", parsed_string->str);
    parser_free(parser);
}

// Scan callback: connect immediately
void BluetoothLogger::on_scan_result(Adapter *ad, Device *dev) {
    char *deviceToString = binc_device_to_string(dev);
    log_info(TAG, deviceToString);
    g_free(deviceToString);

    binc_device_set_connection_state_change_cb(dev, &on_connection_state_changed);
    binc_device_set_services_resolved_cb(dev, &on_services_resolved);
    binc_device_set_bonding_state_changed_cb(dev, &on_bonding_state_changed);
    // binc_device_set_read_char_cb(dev, &on_read);
    // binc_device_set_write_char_cb(dev, &on_write);
    binc_device_set_notify_char_cb(dev, &on_notify);
    binc_device_set_notify_state_cb(dev, &on_notification_state_changed);
    binc_device_set_read_desc_cb(dev, &on_desc_read);
    binc_device_connect(dev);
}


//testing
void BluetoothLogger::on_discovery_state_changed(Adapter *adapter,
        DiscoveryState state,
        const GError *error)
    {
    if (error) {
        log_error(TAG, "discovery error %s", error->message);
    } else {
        log_info(TAG, "discovery %s", binc_adapter_get_discovery_state_name(adapter));
    }
}   

int BluetoothLogger::init() {
    // set up DBus
    GMainLoop *loop = NULL;
    GDBusConnection *conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);
    loop = g_main_loop_new(NULL, FALSE);

    // get default adapter
    default_adapter = binc_adapter_get_default(conn);
    if (!default_adapter) {
        fprintf(stderr, "No Bluetooth adapter found\n");
        return 1;
    }

    //testing
    // at startup, right after you get default_adapter:
    binc_adapter_set_discovery_state_cb(default_adapter, &on_discovery_state_changed);

    // filter scan to only our UART service
    GPtrArray *uuids = g_ptr_array_new();
    g_ptr_array_add(uuids, (gpointer)UART_SERVICE_UUID);
    binc_adapter_set_discovery_filter(default_adapter, -100, uuids, NULL);
    g_ptr_array_free(uuids, TRUE);

    // set scan callback
    binc_adapter_set_discovery_cb(default_adapter, &on_scan_result);
    binc_adapter_start_discovery(default_adapter);

    // run
    g_main_loop_run(loop);

    return 0;
}
