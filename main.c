#include <stdio.h>
#include <stdint.h>
#include <math.h>

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

Registers regs = {0};

const uint32_t FLASH_START = 0x08000000;
const uint32_t FLASH_END = 0x080FFFFF;

uint8_t flash[1024];
uint8_t ram[2048];

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
  FILE *pFile = fopen("output.bin", "rb");

  if (pFile == NULL)
  {
    printf("Unable to open output.bin\n");
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

void setStartupRegisters()
{
  regs.r13 = getWordFromFlash(FLASH_START);

  regs.r15 = getWordFromFlash(FLASH_START + 4);
}

uint32_t fetchCurIns()
{
  uint16_t firstHw = getHwFromFlash(regs.r15);
  uint8_t highByte = (uint8_t)(firstHw >> 8);

  // 32 bit instruction
  if (highByte == 29 || highByte == 30 || highByte == 31)
  {
    return (getHwFromFlash(regs.r15 + 2) << 16) | firstHw;
  }

  return (uint32_t)firstHw;
}

int main()
{
  int loadStatus = loadCodeToFlash();

  if (loadStatus == 1)
  {
    return 1;
  }

  setStartupRegisters();

  printf("Initial stack pointer: %u\n", regs.r13);
  printf("Initial program counter: %u\n", regs.r15);

  while (1)
  {
    uint32_t curIns = fetchCurIns();
    printf("Cur instruction: %u\n", curIns);
    break;
  }

  return 0;
}