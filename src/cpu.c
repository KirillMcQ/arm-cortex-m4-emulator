#include "cpu.h"
#include "memory.h"

Registers regs = {0};
extern const uint32_t FLASH_START;
extern const uint32_t FLASH_END;
static bool didCurInsUpdatePC;

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
    default:
      return 0;
      break;
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
      didCurInsUpdatePC = true;
      regs.r15 = val;
      break;
  }
}

void incrementPC(int amt)
{
  regs.r15 += amt;
}

static uint8_t getITSTATE()
{
  uint8_t lowBits = (regs.xPSR >> 25) & 3;
  uint8_t highBits = (regs.xPSR >> 10) & 63;

  return (highBits << 2) | lowBits;
}

// Returns a 4 bit condition specifier 
static uint8_t getCurrentCond()
{
  uint8_t itstate = getITSTATE();

  if ((itstate & 15) != 0)
  {
    return (itstate >> 4) & 7;
  }
  else if (itstate == 0)
  {
    return 14;
  }

  // Execution of instruction is undefined if code gets here
  return 0;
}

bool shouldExecuteInstruction()
{
  uint8_t cond = getCurrentCond();
  bool result = 0;

  switch ((cond >> 1) & 7)
  {
    case 0:
      result = (regs.xPSR >> 30) & 1;
      break;
    case 1:
      result = (regs.xPSR >> 29) & 1;
      break;
    case 2:
      result = (regs.xPSR >> 31) & 1;
      break;
    case 3:
      result = (regs.xPSR >> 28) & 1;
      break;
    case 4:
      result = ((regs.xPSR >> 29) & 1) && (((regs.xPSR >> 30) & 1) == 0);
      break;
    case 5:
      result = ((regs.xPSR >> 31) & 1) == ((regs.xPSR >> 28) & 1);
      break;
    case 6:
      result = (((regs.xPSR >> 31) & 1) == ((regs.xPSR >> 28) & 1)) && (((regs.xPSR >> 30) & 1) == 0);
      break;
    case 7:
      result = true;
      break;
  }

  if ((cond & 1) && (cond != 15))
  {
    result = ! result;
  }

  return result;
}

void setITSTATE(uint8_t val)
{
  // 2 least significant bits
  uint8_t lsb = val & 3;
  
  // 6 most significant bits
  uint8_t msb = (val >> 2);

  // Clear ITSTATE first
  regs.xPSR &= ~(63 << 10);
  regs.xPSR &= ~(3 << 25);

  regs.xPSR |= (lsb << 25) | (msb << 10);
}

void advanceITSTATE()
{
  uint8_t ITSTATE = getITSTATE();

  if ((ITSTATE & 7) == 0)
  {
    setITSTATE(0);
    return;
  }

  uint8_t first5Bits = ITSTATE & 31;

  setITSTATE((((ITSTATE >> 5) & 7) << 5) | ((first5Bits << 1) & 31));
}

bool isInITBlock()
{
  uint8_t ITSTATE = getITSTATE();
  return (ITSTATE & 15) != 0;
}

bool isLastInITBlock()
{
  uint8_t ITSTATE = getITSTATE();
  return (ITSTATE & 15) == 8;
}

bool getDidCurInsUpdatePC()
{
  return didCurInsUpdatePC;
}

void setDidCurInsUpdatePC(bool val)
{
  didCurInsUpdatePC = val;
}

void setConditionBitN(uint8_t val)
{
  regs.xPSR &= ~(1 << 31);
  regs.xPSR |= (val << 31);
}

void setConditionBitZ(uint8_t val)
{
  regs.xPSR &= ~(1 << 30);
  regs.xPSR |= (val << 30);
}

void setConditionBitC(uint8_t val)
{
  regs.xPSR &= ~(1 << 29);
  regs.xPSR |= (val << 29);
}

void setConditionBitV(uint8_t val)
{
  regs.xPSR &= ~(1 << 28);
  regs.xPSR |= (val << 28);
}

void setConditionBitQ(uint8_t val)
{
  regs.xPSR &= ~(1 << 27);
  regs.xPSR |= (val << 27);
}