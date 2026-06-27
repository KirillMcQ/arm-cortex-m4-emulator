#pragma once

#include <stdint.h>
#include <stdbool.h>

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

void initRegisters();

void writeToRegister(uint32_t val, uint32_t reg);

uint32_t readRegister(uint32_t reg);

void incrementPC(int amt);

bool shouldExecuteInstruction();

void setITSTATE(uint8_t val);

void advanceITSTATE();

bool isInITBlock();

bool isLastInITBlock();

bool getDidCurInsUpdatePC();

void setDidCurInsUpdatePC(bool val);

void setConditionBitN(uint8_t val);

void setConditionBitZ(uint8_t val);

void setConditionBitC(uint8_t val);

void setConditionBitV(uint8_t val);

void setConditionBitQ(uint8_t val);