/* simple_ble_central.c
 * Minimal BlueZ Inc demo: connect to RN4870/90 via Transparent UART Service
 */

#include <glib.h>
#include <stdio.h>
#include <signal.h>
#include "adapter.h"
#include "device.h"
#include "logger.h"
#include "parser.h"

#define TAG             "SimpleCentral"
// Transparent UART Service and characteristics (RN4870/90 defaults)
#define UART_SERVICE_UUID   "49535343-fe7d-4ae5-8fa9-9fafd205e455"
#define UART_TX_CHAR_UUID   "49535343-1e4d-4bd9-ba61-23c647249616"  // Write to RX
#define UART_RX_CHAR_UUID   "49535343-8841-43f4-a8d4-ecbe34729bb3"  // Notify from TX

GMainLoop *loop = NULL;
Adapter    *default_adapter = NULL;

void on_connection_state_changed(Device *device, ConnectionState state, const GError *error) {
    log_info(TAG,"test");
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
void on_notify(Device *device, Characteristic *ch, const GByteArray *value) {
    printf("[RX] %.*s\n", value->len, value->data);
}

// Called when services are resolved: subscribe to TX notifications
void on_services_resolved(Device *dev) {
    log_info(TAG, "Services resolved -> start notify");
    binc_device_start_notify(dev, UART_SERVICE_UUID, UART_TX_CHAR_UUID);
}



void on_bonding_state_changed(Device *device, BondingState new_state, BondingState old_state, const GError *error) {
    log_info(TAG, "bonding state changed from %d to %d", old_state, new_state);
}

void on_notification_state_changed(Device *device, Characteristic *characteristic, const GError *error) {
    const char *uuid = binc_characteristic_get_uuid(characteristic);

    if (error != NULL) {
        log_info(TAG, "notifying <%s> failed (error %d: %s)", uuid, error->code, error->message);
        return;
    }

    log_info(TAG, "<%s> notifying %s", uuid, binc_characteristic_is_notifying(characteristic) ? "true" : "false");
}

void on_desc_read(Device *device, Descriptor *descriptor, const GByteArray *byteArray, const GError *error) {
    log_info(TAG, "on descriptor read");
    Parser *parser = parser_create(byteArray, LITTLE_ENDIAN);
    GString *parsed_string = parser_get_string(parser);
    log_info(TAG, "CUD %s", parsed_string->str);
    parser_free(parser);
}

// Scan callback: connect immediately
void on_scan_result(Adapter *ad, Device *dev) {
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
void on_discovery_state_changed(Adapter *adapter,
        DiscoveryState state,
        const GError *error)
    {
    if (error) {
        log_error(TAG, "discovery error %s", error->message);
    } else {
        log_info(TAG, "discovery %s", binc_adapter_get_discovery_state_name(adapter));
    }
}   

int main(void) {
    log_enabled(TRUE);
    log_set_level(LOG_INFO);

    // set up DBus
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
