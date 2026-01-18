#include "wifi_manager.h"
#include "time/rtc.h"
#include "config/config.h"

#include <Arduino.h>
#include <WiFi.h>

static bool ap_active = false;
static uint32_t wifi_start_time = 0;

void wifi_init() {
    WiFi.mode(WIFI_OFF);
}

void wifi_start_app() {
    if (ap_active) return;

    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);

    wifi_start_time = rtc_now();
    ap_active = true;

    Serial.println("WiFi AP started");
}

void wifi_stop() {
    if(!ap_active) return;

    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_OFF);

    ap_active = false;
    Serial.println("WiFi AP stopped");
}

bool wifi_active() {
    return ap_active
}

void wifi_loop() {
    if (!ap_active) return;

    // Shut down after window expires
    if ((rtc_now() - wifi_start_time) >
        (WIFI_WINDOW_MINUTES * 60)) {
        wifi_stop();
    }
}





