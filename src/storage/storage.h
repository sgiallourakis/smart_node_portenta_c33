#pragma once

// Core data structure for all sensor packets
// This MUST stay in sync with the sender (WisBlock nodes)
#include "packets/sensor_packet.h"

/**
 * Initialize the filesystem and prepare storage directories.
 * Called once during setup().
 */
void storage_init();

/**
 * Append a single sensor packet to today's binary log file.
 * This is an append-only operation for safety and flash longevity.
 */
void storage_append(const SensorPacket& pkt);

/**
 * Iterate over all packets recorded today.
 *
 * For each packet found, the callback function `cb` is invoked.
 * This allows analysis code to process packets without loading
 * everything into RAM.
 *
 * @param cb A function pointer called once per SensorPacket
 */
void storage_iterate_today(void (*cb)(const SensorPacket&));
