#include "storage.h"

// Provides rtc_now(), used to determine the current day
#include "time/rtc.h"

// Core Arduino includes
#include <Arduino.h>

// Generic filesystem interface used by LittleFS, SD, etc.
#include <FS.h>

// LittleFS is the filesystem we are using on QSPI flash
#include <LittleFS.h>

// ------------------------------------------------------
// Storage backend abstraction
// ------------------------------------------------------
// By aliasing LittleFS as STORAGE_FS, we can later swap
// to SD or another filesystem by changing ONE line.
#define STORAGE_FS LittleFS

// Holds the filename for the current day's data file
static char current_filename[20];

/**
 * Build the filename for today's data log.
 *
 * For now this uses "days since boot" as a placeholder.
 * Once RTC is fully wired, this should be replaced with
 * YYYYMMDD-based naming.
 */
static void build_today_filename() {
    // Current time in seconds (or uptime)
    uint32_t now = rtc_now();

    // Convert seconds into whole days
    uint32_t day = now / 86400;

    // Example filename: /data/day_3.bin
    snprintf(
        current_filename,
        sizeof(current_filename),
        "/data/day_%lu.bin",
        day
    );
}

/**
 * Initialize the filesystem and ensure the data directory exists.
 */
void storage_init() {
    // Mount the filesystem
    if (!STORAGE_FS.begin()) {
        Serial.println("ERROR: Failed to mount filesystem");
        return;
    }

    // Create /data directory if it does not exist
    STORAGE_FS.mkdir("/data");
}

/**
 * Append a single SensorPacket to today's log file.
 *
 * The packet is written in raw binary form.
 * No buffering or transformation is performed.
 */
void storage_append(const SensorPacket& pkt) {
    // Ensure filename is correct for today
    build_today_filename();

    // Open the file in append mode
    File f = STORAGE_FS.open(current_filename, FILE_APPEND);
    if (!f) {
        Serial.println("ERROR: Failed to open data file for append");
        return;
    }

    // Write the packet bytes directly to flash
    f.write(
        reinterpret_cast<const uint8_t*>(&pkt),
        sizeof(SensorPacket)
    );

    // Close immediately to minimize corruption risk
    f.close();
}

/**
 * Iterate over all packets stored today.
 *
 * Reads the file sequentially and invokes the callback
 * once per complete SensorPacket record.
 */
void storage_iterate_today(void (*cb)(const SensorPacket&)) {
    // Ensure filename is correct for today
    build_today_filename();

    // Open today's file for reading
    File f = STORAGE_FS.open(current_filename, FILE_READ);
    if (!f) {
        // No file means no data yet today
        return;
    }

    SensorPacket pkt;

    // Read packets sequentially until EOF
    while (f.available() >= sizeof(SensorPacket)) {
        // Attempt to read exactly one packet
        size_t read = f.read(
            reinterpret_cast<uint8_t*>(&pkt),
            sizeof(SensorPacket)
        );

        // Stop if we fail to read a full record
        if (read != sizeof(SensorPacket)) {
            break;
        }

        // Pass packet to caller-provided callback
        cb(pkt);
    }

    // Close file when done
    f.close();
}
