#pragma once

#include <stdint.h>
#include <stdbool.h>

uint32_t fetchCurIns();
void decodeAndExecuteIns(uint32_t encoding);
bool isInstruction16Bit(uint32_t encoding);