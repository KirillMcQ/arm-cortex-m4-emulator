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
    flash[(address - FLASH_START) / 4] = dataByte;
  }
}

uint8_t readByteFromMemory(uint32_t address)
{
  // Flash memory
  if ((address >= FLASH_START) && (address <= FLASH_END))
  {
    return flash[(address - FLASH_START) / 4];
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
      curLoadAddr += 4;
    }
  }

  return 0;
}

void setStartupRegisters()
{
  regs.r13 = (readByteFromMemory(FLASH_START + 12) << 24) | (readByteFromMemory(FLASH_START + 8) << 16) | (readByteFromMemory(FLASH_START + 4) << 8) | (readByteFromMemory(FLASH_START));

  printf("%d\n", (readByteFromMemory(FLASH_START + 16)));
  printf("%d\n", (readByteFromMemory(FLASH_START + 20)));
  printf("%d\n", (readByteFromMemory(FLASH_START + 24)));
  printf("%d\n", (readByteFromMemory(FLASH_START + 28)));
  regs.r15 = (readByteFromMemory(FLASH_START + 28) << 24) | (readByteFromMemory(FLASH_START + 24) << 16) | (readByteFromMemory(FLASH_START + 20) << 8) | (readByteFromMemory(FLASH_START + 16));
}

int main()
{
  int loadStatus = loadCodeToFlash();

  if (loadStatus == 1)
  {
    return 1;
  }

  setStartupRegisters();

  printf("Initial stack pointer: %d\n", regs.r13);
  printf("Initial program counter: %d\n", regs.r15);

  return 0;
}