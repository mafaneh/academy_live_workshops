#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(ble_central);

bool adv_data_parser(struct bt_data *data, void *user_data)
{
    bt_addr_le_t *addr = (bt_addr_le_t *)user_data;

    // Parse for device name
    // if (data->type == BT_DATA_NAME_COMPLETE)
    // {
        // LOG_INF("Found device with addr %02x:%02x:%02x:%02x:%02x:%02x, name %s",
        //         addr->a.val[0], addr->a.val[1], addr->a.val[2],
        //         addr->a.val[3], addr->a.val[4], addr->a.val[5], data->data);
        // return false;
    // }

    // Parser for service data
    if (data->type == BT_DATA_SVC_DATA16)
    {
        LOG_INF("Found device with addr %02x:%02x:%02x:%02x:%02x:%02x, UUID16 %02x%02x with service data (length=%d)",
                addr->a.val[0], addr->a.val[1], addr->a.val[2],
                addr->a.val[3], addr->a.val[4], addr->a.val[5], data->data[1], data->data[0], data->data_len);
        LOG_HEXDUMP_INF(data->data+2, data->data_len-2, "Service Data");

        // Continue parsing
        return false;
    }

    return true;
}

void scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type, struct net_buf_simple *buf)
{
    bt_data_parse(buf, adv_data_parser, (void *)addr);
}

int main(void)
{
    int err;

    struct bt_le_scan_param scan_param = {
        .type       = BT_LE_SCAN_TYPE_ACTIVE,
        .options    = BT_LE_SCAN_OPT_FILTER_DUPLICATE,
        .interval   = 160,
        .window     = 16,
    };

    LOG_INF("Starting Bluetooth Central example");

    err = bt_enable(NULL);
    if (err) {
        LOG_INF("Bluetooth init failed (err %d)\n", err);
        return 1;
    }

    err = bt_le_scan_start(&scan_param, &scan_cb);
    if (err) {
        LOG_INF("Bluetooth start scan failed (err %d)\n", err);
        return 1;
    }

    return 0;
}
