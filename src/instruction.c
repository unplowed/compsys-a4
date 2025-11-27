#include "instruction.h"

int decode_i_immediate(instruction_t *instruction) {
  int v = *(int *)instruction;
  int imm31_11;
  int out = 0;
  if (v & 0b10000000000000000000000000000000) {
    out |= 0b11111111111111111111100000000000;
  }
  out = out + (v >> 20);
  return out;
};
int decode_s_immediate(instruction_t *instruction);
int decode_b_immediate(instruction_t *instruction);
int decode_u_immediate(instruction_t *instruction);
int decode_j_immediate(instruction_t *instruction);
