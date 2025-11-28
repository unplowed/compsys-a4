#include "instruction.h"
#include <assert.h>
#include <stdio.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)                                                   \
  ((byte) & 0x80 ? '1' : '0'), ((byte) & 0x40 ? '1' : '0'),                    \
      ((byte) & 0x20 ? '1' : '0'), ((byte) & 0x10 ? '1' : '0'),                \
      ((byte) & 0x08 ? '1' : '0'), ((byte) & 0x04 ? '1' : '0'),                \
      ((byte) & 0x02 ? '1' : '0'), ((byte) & 0x01 ? '1' : '0')

int decode_i_immediate(instruction_t *instruction) {
  int *intstruction = (int *)instruction;
  return (int)(*intstruction & 0x00000FFF) >> 20;
}

int decode_s_immediate(instruction_t *instruction) {
  int *intstruction = (int *)instruction;
  return ((int)(*intstruction & 0xFE000000) >> 20) |
         ((*intstruction & 0x00000F80) >> 7);
}

void printBinary(int n) {
  for (int i = sizeof(n) * 8 - 1; i >= 0; i--) {
    int bit = (n >> i) & 1;
    printf("%d", bit);
  }
}

int decode_b_immediate(instruction_t *instruction) {
  struct b_type l = instruction->args.B;

  printBinary(*(int *)instruction);
  printf("\n");

  int immediate = (l.imm31 << 12) | (l.imm12 << 5) | (l.imm11 << 1) | (l.imm7<<2);

  // Sign extend if the immediate's 12th bit is set
  if (immediate & (1 << 12)) {
    immediate |= 0xFFFFFF00; // Fill upper bits with 1s for negative numbers
  }

  printBinary(immediate);
  printf(": %i\n", immediate);

  return immediate;
}

int decode_u_immediate(instruction_t *instruction) {
  int *intstruction = (int *)instruction;
  return (int)(*intstruction & 0xFFFFF000);
}

int decode_j_immediate(instruction_t *instruction) {
  int *intstruction = (int *)instruction;
  return ((int)(*intstruction & 0x80000000) >> 11) |
         ((int)(*intstruction & 0x7FE00000) >> 9) |
         ((int)(*intstruction & 0x00100000) >> 20);
}
