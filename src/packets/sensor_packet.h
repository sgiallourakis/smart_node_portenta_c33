#pragma once
#include <stdint.h>

struct SensorPacket {
    uint8_t node_id;
    uint32_t timestamp;     // seconds since epocj or boot
    uint16_t battery_mv;

    int16_t temp_c_x100;
    uint16_t humidity_x100;
    uint16_t air_quality;

    uint8_t flags;
};