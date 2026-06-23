#include "memory.h"

const uint32_t FLASH_START = 0x08000000;
const uint32_t FLASH_END = 0x080FFFFF;
Registers regs = {0};

void writeByteToMemory(uint8_t dataByte, uint32_t address)
{
  // Flash memory
  if ((address >= FLASH_START) && (address <= FLASH_END))
  {
    flash[(address - FLASH_START)] = dataByte;
  }
}

uint8_t getByteFromMemory(uint32_t address)
{
  // Flash memory
  if ((address >= FLASH_START) && (address <= FLASH_END))
  {
    return flash[(address - FLASH_START)];
  }
}

int loadCodeToFlash()
{
  FILE *pFile = fopen("./example/output.bin", "rb");

  if (pFile == NULL)
  {
    return 1;
  }

  char buf[1024];
  uint32_t curLoadAddr = FLASH_START;

  while (fgets(buf, 1024, pFile) != NULL)
  {
    // Don't include null terminator
    for (int i = 0; i < (sizeof(buf) / sizeof(buf[0])) - 1; i++)
    {
      writeByteToMemory((uint8_t)buf[i], curLoadAddr);
      curLoadAddr++;
    }
  }

  return 0;
}

uint32_t getWordFromFlash(uint32_t address)
{
  return (getByteFromMemory(address + 3) << 24) | (getByteFromMemory(address + 2) << 16) | (getByteFromMemory(address + 1) << 8) | (getByteFromMemory(address));
}

uint16_t getHwFromFlash(uint32_t address)
{
  return (getByteFromMemory(address + 1) << 8) | (getByteFromMemory(address));
}

void initRegisters()
{
  // Startup Registers
  regs.r13 = getWordFromFlash(FLASH_START);

  regs.r15 = getWordFromFlash(FLASH_START + 4);

  // T = 1
  regs.xPSR |= (1 << 24);
}

uint32_t readRegister(uint32_t reg)
{
  switch (reg)
  {
    case 0:
      return regs.r0;
    case 1:
      return regs.r1;
    case 2:
      return regs.r2;
    case 3:
      return regs.r3;
    case 4:
      return regs.r4;
    case 5:
      return regs.r5;
    case 6:
      return regs.r6;
    case 7:
      return regs.r7;
    case 8:
      return regs.r8;
    case 9:
      return regs.r9;
    case 10:
      return regs.r10;
    case 11:
      return regs.r11;
    case 12:
      return regs.r12;
    case 13:
      return regs.r13;
    case 14:
      return regs.r14;
    case 15:
      return regs.r15;
  }
}

void writeToRegister(uint32_t val, uint32_t reg)
{
  switch (reg)
  {
    case 0:
      regs.r0 = val;
      break;
    case 1:
      regs.r1 = val;
      break;
    case 2:
      regs.r2 = val;
      break;
    case 3:
      regs.r3 = val;
      break;
    case 4:
      regs.r4 = val;
      break;
    case 5:
      regs.r5 = val;
      break;
    case 6:
      regs.r6 = val;
      break;
    case 7:
      regs.r7 = val;
      break;
    case 8:
      regs.r8 = val;
      break;
    case 9:
      regs.r9 = val;
      break;
    case 10:
      regs.r10 = val;
      break;
    case 11:
      regs.r11 = val;
      break;
    case 12:
      regs.r12 = val;
      break;
    case 13:
      regs.r13 = val;
      break;
    case 14:
      regs.r14 = val;
      break;
    case 15:
      regs.r15 = val;
      break;
  }
}