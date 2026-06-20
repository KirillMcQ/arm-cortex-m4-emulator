#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "memory.h"
#include "instructions.h"

Registers regs = {0};

int main()
{
  int loadStatus = loadCodeToFlash();

  if (loadStatus == 1)
  {
    printf("Unable to open output.bin\n");
    return 1;
  }

  setStartupRegisters(&regs.r13, &regs.r15);

  printf("Initial stack pointer: %u\n", regs.r13);
  printf("Initial program counter: %u\n", regs.r15);

  while (1)
  {
    uint32_t curIns = fetchCurIns(regs.r15);
    printf("Cur instruction: %u\n", curIns);
    break;
  }

  return 0;
}