#include "battery_analysis.h"

#include "storage/storage.h"
#include "packets/sensor_packet.h"
#include "config/config.h"

#include <Arduino.h>
#include <math.h>

// ----------------------------------------------
// Internal aggregation state (static = no heap)
// ----------------------------------------------
struct BatteryAgg {
    uint32_t count;

    // For slope calculation
    uint32_t first_ts;
    uint32_t last_ts;

    uint16_t first_mv;
    uint16_t last_mv;

    uint32_t sum_mv;
};

static BatteryAgg node[MAX_NODES];

// ---------------------------------------------
// Helper: reset aggregation state
// ---------------------------------------------

static void reset_stage() {
    for (int i = 0; i < MAX_NODES; i++) {
        nodes[i] = {};
    }
}

// --------------------------------------------
// Callback for storage_iterate_today()
// --------------------------------------------

static void aggregate_packet(const SensorPacket& pkt) {
    if (pkt.node_id >= MAX_NDOES) return;

    BatteryAgg& a = nodes[pkt.node_id];

    if (a.count == 0) {
        // First packet seen for this node today
        a.first_ts = pkt.timestamp;
        a.first_mv = pkt.battery_mv;
    }

    // Always update last-seen values
    a.last_ts = pkt.timestamp;
    a.last_mv = pkt.battery_mv;

    a.sum_mv += pkt.battery_mv;
    a.count++;
}

// --------------------------------------------
// Core Analysis
// --------------------------------------------

void battery_analysis_run() {
    // 1. Aggregate packets per node
    // 2. Compute mean voltage + slope
    // 3. Fleet statistics
    // 4. Z-score anomalies
    reset_state();

    // Walk all packets for today
    storage_iterate_today(aggregate_packet);

    Serial.println(" === Daily Battery Report === ")

    for (int node = 0; node < MAX_NODES; node++) {
        BatteryAgg& a = nodes[node];

        if (a.count == 0) continue;

        float mean_mv = a.sum_mv / (float)a.count;

        float slope_mv_per_day = 0.0f;

        if (a.last_ts > a.first_ts) {
            float delta_mv = (float)a.last_mv - (float)a.first_mv;
            float delta_days = (a.last_ts - a.first_ts) / 86400.0f;

            if (delta_days > 0.0f) {
                slope_mv_per_day = delta_mv / delta_days;
            }
        }

        bool missing_packets  = a.count < EXPECTED_PACKETS_PER_DAY;

        bool low_battery = mean_mv < LOW_BATTERY_MV;

        // -----------------------------------------
        // Output
        // -----------------------------------------

        Serial.print("Node ");
        Serial.print(node);
        Serial.print(": packets=");
        Serial.print(a.count);

        Serial.print(" mean_mv");
        Serial.print(slope_mv_per_day, 1);

        if (missing_packets) {
            Serial.print(" [MISSING]");
        }

        if (low_battery) {
            Serial.print(" [LOW]")
        }

        Serial.println();
    }
}