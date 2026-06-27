#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "memory.h"
#include "instructions.h"
#include "cpu.h"

int main()
{
  int loadStatus = loadCodeToFlash();

  if (loadStatus == 1)
  {
    printf("Unable to open output.bin\n");
    return 1;
  }

  initRegisters();

  printf("Initial stack pointer: %u\n", readRegister(13));
  printf("Initial program counter: %u\n", readRegister(15));

  while (1)
  {
    // Test instruction, cur is LSR
    writeToRegister(5, 0);
    writeToRegister(10, 1);
    writeByteToMemory(1, readRegister(15));
    writeByteToMemory(8, readRegister(15) + 1);

    uint32_t curIns = fetchCurIns();
    printf("Cur instruction: %u\n", curIns);


    if (shouldExecuteInstruction())
    {
      decodeAndExecuteIns(curIns);
    }
    else
    {
      printf("Treating instruction like nop due to IT condition\n");
    }

    if (!getDidCurInsUpdatePC())
    {
      if (isInstruction16Bit(curIns))
      {
        incrementPC(2);
      }
      else
      {
        incrementPC(4);
      }
    }
    else
    {
      setDidCurInsUpdatePC(0);
    }

    printf("r0: %u, r1: %u, pc: %u\n", readRegister(0), readRegister(1), readRegister(15));
    printf("xPSR: %u\n", regs.xPSR);

    break;
  }

  return 0;
}