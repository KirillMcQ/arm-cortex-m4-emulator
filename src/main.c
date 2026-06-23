#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "memory.h"
#include "instructions.h"

extern Registers regs;

int main()
{
  int loadStatus = loadCodeToFlash();

  if (loadStatus == 1)
  {
    printf("Unable to open output.bin\n");
    return 1;
  }

  initRegisters();

  printf("Initial stack pointer: %u\n", regs.r13);
  printf("Initial program counter: %u\n", regs.r15);

  while (1)
  {
    /*
     * Put in a fake LSL instruction to test
     * Example: LSL r1, r0, 3
     * r0: 8
     * Expected r1: 32
     */
    writeToRegister(0, 4);
    writeByteToMemory(193, 0x800019D);
    writeByteToMemory(0, 0x800019E);
    uint32_t curIns = fetchCurIns();
    printf("Cur instruction: %u\n", curIns);
    decodeAndExecuteIns(curIns);
    printf("r1: %u\n", regs.r1);
    break;
  }

  return 0;
}