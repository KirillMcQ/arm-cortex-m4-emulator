#pragma once

#include <stdio.h>
#include <stdint.h>
#include "cpu.h"

extern const uint32_t FLASH_START;
extern const uint32_t FLASH_END;
extern Registers regs;


uint8_t flash[1024];
uint8_t ram[2048];

void writeByteToMemory(uint8_t dataByte, uint32_t address);

uint8_t getByteFromMemory(uint32_t address);

int loadCodeToFlash();

uint32_t getWordFromFlash(uint32_t address);

uint16_t getHwFromFlash(uint32_t address);
