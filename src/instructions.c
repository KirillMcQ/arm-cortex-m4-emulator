#include "instructions.h"

uint32_t fetchCurIns(uint32_t r15)
{
  uint16_t firstHw = getHwFromFlash(r15);
  uint8_t highByte = (uint8_t)(firstHw >> 8);

  // 32 bit instruction
  if (highByte == 29 || highByte == 30 || highByte == 31)
  {
    return (getHwFromFlash(r15 + 2) << 16) | firstHw;
  }

  return (uint32_t)firstHw;
}