#include <glib.h>
#include <stdio.h>
#include <signal.h>
#include "adapter.h"
#include "device.h"
#include "logger.h"
#include "parser.h"


#define TAG "SimpleCentral"
// Transparent UART Service and characteristics (RN4870/90 defaults)
#define UART_SERVICE_UUID   "49535343-fe7d-4ae5-8fa9-9fafd205e455"
#define UART_TX_CHAR_UUID   "49535343-1e4d-4bd9-ba61-23c647249616"  // Write to RX
#define UART_RX_CHAR_UUID   "49535343-8841-43f4-a8d4-ecbe34729bb3"  // Notify from TX

class BluetoothLogger{
    public:
    static int init();
    private:
    static Adapter *default_adapter;
    static void on_connection_state_changed(Device *device, ConnectionState state, const GError *error);
    static void on_notify(Device *device, Characteristic *ch, const GByteArray *value);
    static void on_services_resolved(Device *dev);
    static void on_bonding_state_changed(Device *device, BondingState new_state, BondingState old_state, const GError *error);
    static void on_notification_state_changed(Device *device, Characteristic *characteristic, const GError *error);
    static void on_desc_read(Device *device, Descriptor *descriptor, const GByteArray *byteArray, const GError *error);
    static void on_scan_result(Adapter *ad, Device *dev);
    static void on_discovery_state_changed(Adapter *adapter,DiscoveryState state,const GError *error);
};