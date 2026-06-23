#pragma once

#include <stdio.h>
#include <stdint.h>

typedef struct
{
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r4;
  uint32_t r5;
  uint32_t r6;
  uint32_t r7;
  uint32_t r8;
  uint32_t r9;
  uint32_t r10;
  uint32_t r11;
  uint32_t r12;
  uint32_t r13; // SP, SP_PROCESS, SP_MAIN
  uint32_t r14; // LR
  uint32_t r15; // PC
  uint32_t xPSR;
} Registers;

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

void initRegisters();

void writeToRegister(uint32_t val, uint32_t reg);

uint32_t readRegister(uint32_t reg);