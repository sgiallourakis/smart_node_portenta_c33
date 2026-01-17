#pragma once
#include "packets/sensor_packet.h"

void storage_init();
void storage_append(const SensorPacket& pkt);
void storage_iterative_today(void (*cb)(const SensorPacker&));