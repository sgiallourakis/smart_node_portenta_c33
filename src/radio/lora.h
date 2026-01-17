#pragma once
#include <stddef.h>
#include <stdint.h>

void lora_init();
bool lora_receive(uint8_t* buffer, size_t* len);