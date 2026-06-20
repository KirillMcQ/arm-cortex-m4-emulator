#include "memory.h"

const uint32_t FLASH_START = 0x08000000;
const uint32_t FLASH_END = 0x080FFFFF;

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

void setStartupRegisters(uint32_t *r13, uint32_t *r15)
{
  *r13 = getWordFromFlash(FLASH_START);

  *r15 = getWordFromFlash(FLASH_START + 4);
}