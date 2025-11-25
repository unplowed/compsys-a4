#include "simulate.h"
#include <assert.h>
#include <stdio.h>
#include "instruction.h"
#include "memory.h"
#include "registers.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

void unknown_instruction(instruction_t *op) {
  printf("Unknown instruction with opcode: %i, ", op->opcode);
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(op->opcode));
  printf("\ninstruction: 0x%08x\n", *(unsigned int*)op);
}

int write_register(int register_idx, int value) {
  assert(0);
}
int read_register(int register_idx, int *out) {
  assert(0);
}

struct Stat simulate(struct memory *mem, int start_addr, FILE *log_file, struct symbols* symbols) {
  int run = 1;
  registers_t registers;
  registers.pc = start_addr;

  while (run) {
    int m = memory_read_word(mem, registers.pc);
    instruction_t *op = (instruction_t*)&m;

    switch (op->opcode) {
      case OP_ADDI:
      break;
      case OP_AUIPC:
        // rd = pc + imm
      break;
      case OP_LUI:
        // rd = imm
      break;
      case OP_JAL:
        // rd = pc + 4
        // goto pc + (imm20 << 1)
      break;
      default:
        unknown_instruction(op);
        assert(0);
      break;
    }

    registers.pc += 4;
  }

  // TODO: fix
  struct Stat x = {0};
  return x;
}
