#include "storage.h"

void storage_init() {
    // mount QSPI
}

void storage_append(const SensorPacket& pkt) {
    // append binary packet
}

void storage_iterate_today(void (*cb)(const SensorPacket&)) {
    // read today's file sequentially
}