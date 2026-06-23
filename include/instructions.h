#pragma once

#include <stdint.h>
#include "memory.h"

extern Registers regs;

uint32_t fetchCurIns();
void decodeAndExecuteIns(uint32_t encoding);