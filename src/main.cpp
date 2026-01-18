#include <Arduino.h>

#include "radio/lora.h"
#include "storage/storage.h"
#include "analysis/daily_runner.h"
#include "time/rtc.h"
#include "packets/sensor_packet.h"
#include "wifi/wifi_manager.h"


void setup() {
    Serial.begin(115200)

    lora_init();
    storage_init();
    wifi_init();
}

void loop() {
    uint8_t buf[32];
    size_t len = sizeof(buf);

    if (lora_receive(buf, &len)) {
        if (len == sizeof(SensorPacket)) {
            SensorPacket pkt;
            memcpy(&pkt, buf, sizeof(pkt));
            storage_append(pkt);
        }
    }

    if (is_new_day()) {
        run_daily_analysis();
    }
    
    // WiFi schedule check
    uint32_t now = rtc_now();
    uint8_t hour = (now / 3600) % 24;

    if (hour == WIFI_BROADCAST_HOUR && !wifi_active()) {
        wifi_start_ap();
    }

    wifi_loop();
}