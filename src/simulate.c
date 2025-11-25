#include "simulate.h"
#include <assert.h>
#include <stdio.h>
#include "instruction.h"
#include "memory.h"

void unknown_instruction(instruction_t *op) {
  printf("Unknown instruction with opcode: %04x", op->opcode);
  printf("instruction: %08x", *(unsigned int*)op);
}

struct Stat simulate(struct memory *mem, int start_addr, FILE *log_file, struct symbols* symbols) {
  int run = 1;
  int addr = start_addr;
  while (run) {
    int m = memory_read_word(mem, addr);
    instruction_t *op = (instruction_t*)&m;

    switch (op->opcode) {
      case OP_ADDI:
      break;
      default:
        unknown_instruction(op);
        assert(0);
      break;
    }
  }

  // TODO: fix
  struct Stat x = {0};
  return x;
}
