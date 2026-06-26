#include "instructions.h"
#include "memory.h"
#include "cpu.h"

extern Registers regs;

static void execute16BitIns(uint16_t encoding);
static void execute32BitIns(uint32_t encoding);
static void executeShiftMathMoveCompare(uint16_t encoding);
static void executeMiscellaneous16BitInstructions(uint16_t encoding);
static void handleLSLImmediate(uint16_t encoding);
static void handleIT(uint16_t encoding);

uint32_t fetchCurIns()
{
  uint16_t firstHw = getHwFromFlash(regs.r15);
  uint8_t high5Bits = (uint8_t)(firstHw >> 11);

  // 32 bit instruction
  if (high5Bits == 29 || high5Bits == 30 || high5Bits == 31)
  {
    return (getHwFromFlash(regs.r15 + 2) << 16) | firstHw;
  }

  return (uint32_t)firstHw;
}

void decodeAndExecuteIns(uint32_t encoding)
{
  if ((encoding >> 16) == 0)
  {
    execute16BitIns((uint16_t)encoding);
    return;
  }

  execute32BitIns(encoding);
}

static void execute16BitIns(uint16_t encoding)
{
  // uint8_t closest to 6 bits, opcode is 6 bits
  uint8_t opcode = encoding >> 10;

  if ((opcode >> 4) == 0)
  {
    // A5.2.1
    executeShiftMathMoveCompare(encoding);
  }
  else if ((opcode >> 2) == 11)
  {
    // TODO: Out of order, but this implementation is foundational for other instructions' implementation
    executeMiscellaneous16BitInstructions(encoding);
  }
}

static void execute32BitIns(uint32_t encoding)
{
}

/*
 * These functions handle: Shift (immediate), add, subtract, move, and compare
 * Section in Armv7-M Reference Manual: A5.2.1
 */

static void executeShiftMathMoveCompare(uint16_t encoding)
{
  // uint8_t closest to 5 bits, opcode is 5 bits
  uint8_t opcode = (opcode << 2) >> 11;
  
  switch (opcode >> 2)
  {
    // LSL
    case 0:
      handleLSLImmediate(encoding);
      break;
  }
}

static void handleLSLImmediate(uint16_t encoding)
{
  printf("encoding: %u\n", encoding);
  // uint8_t closest to 5 bits, imm5 is 5 bits
  uint8_t imm5 = (encoding >> 6);

  // Same for rm and rd, but rm and rd are each 3 bits
  uint8_t rm = (uint16_t)(encoding << 10) >> 13;
  uint8_t rd = (uint16_t)(encoding << 13) >> 13;

  uint32_t rm_val = readRegister(rm);
  uint32_t result = (rm_val << imm5);

  printf("imm5: %u, rm: %u, rd: %u, result: %u\n", imm5, rm, rd, result);

  writeToRegister(result, rd);
}

/*
 * The functions handle: Miscellaneous 16-bit instructions
 * Section in Armv7-M Reference Manual: A5.2.5
*/
static void executeMiscellaneous16BitInstructions(uint16_t encoding)
{
  uint8_t opcode = (encoding >> 5) & 127;

  // TODO: Out of place, but implementing IT first is foundational to the emulator. Will be fixed.
  if ((opcode >> 3) == 15)
  {
    handleIT(encoding);
  }
}

static void handleIT(uint16_t encoding)
{
  uint8_t firstCond = (encoding >> 4) & 15;
  uint8_t mask = encoding & 15;

  setITSTATE((firstCond << 4) | mask);
}

bool isInstruction16Bit(uint32_t encoding)
{
  return (encoding >> 16) == 0;
}