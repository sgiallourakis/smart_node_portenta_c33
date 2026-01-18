#pragma once

// --- Timing ---
#define EXPECTED_PACKETS_PER_DAY 960
#define DAILY_ANALYSIS_HOUR       0     // midnight

// --- Battery Thresholds ---
#define LOW_BATTERY_MV      1000        // PLACEHOLDER
#define Z_SCORE_THRESHOLD   2.5f        // PLACEHOLDER

// --- Storage ---
#define MAX_NODES           10          // PLACEHOLDER

// --- Debug ---
#define DEBUG_SERIAL        1           // PLACEHOLDER

// --- WiFi broadcast window ---
#define WIFI_BROADCAST_HOUR    19    // 07:00PM
#define WIFI_WINDOW_MINUTES    10    // minutes of ON-TIME

// --- AP settings ---
#define WIFI_SSID        "CCI-SMART-NODE"
#define WIFI_PASSWORD    "3cHoDe#o1649342"