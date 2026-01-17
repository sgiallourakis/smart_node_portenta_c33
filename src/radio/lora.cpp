#include "lora.h"

void lora_init() {
    // init RadioLib / SX1262 here
}

bool lora_receive(uint8_t* buffer, size_t* len) {
    // non-blocking recieve
    // return true if packet available
    return false;
}