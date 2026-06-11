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

char flash[1024];
char ram[2048];

int main()
{
  FILE *pFile = fopen("output.bin", "r");

  if (pFile == NULL)
  {
    printf("Unable to open output.bin");
    return 1;
  }

  char buf[1024];
  while (fgets(buf, 1024, pFile) != NULL)
  {
    // Don't include null terminator
    for (int i = 0; i < (sizeof(buf) / sizeof(buf[0])) - 1; i++)
    {
      printf("%u", buf[i]);
    }
  }

  return 0;
}