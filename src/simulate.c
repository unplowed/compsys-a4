#include "simulate.h"
#include "instruction.h"
#include "memory.h"
#include "registers.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)                                                   \
  ((byte) & 0x80 ? '1' : '0'), ((byte) & 0x40 ? '1' : '0'),                    \
      ((byte) & 0x20 ? '1' : '0'), ((byte) & 0x10 ? '1' : '0'),                \
      ((byte) & 0x08 ? '1' : '0'), ((byte) & 0x04 ? '1' : '0'),                \
      ((byte) & 0x02 ? '1' : '0'), ((byte) & 0x01 ? '1' : '0')

#define debug 1

void unknown_instruction(instruction_t *op) {
  printf("Unknown instruction with opcode: %i, ", op->opcode);
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(op->opcode));
  printf("\ninstruction: 0x%08x\n", *(unsigned int *)op);
}

int write_register(registers_t *registers, int register_idx, int value) {
  if (debug) {
    printf("writing %i to %i (%s)\n", value, register_idx,
           register_names[register_idx]);

    if (register_idx < 0 || register_idx > 31) {
      printf("Out of bounds write!\n");
      assert(0);
    }
  }

  // ignore writes to zero
  switch (register_idx) {
  case REG_ZERO:
    return 0;
  }

  registers->unnamed[register_idx] = value;
  return 0;
}

int read_register(const registers_t *registers, int register_idx, int *out) {
  if (debug) {
    printf("reading register %i (%s): ", register_idx,
           register_names[register_idx]);

    if (register_idx < 0 || register_idx > 31) {
      printf("Out of bounds read!\n");
      assert(0);
    }
  }

  *out = registers->unnamed[register_idx];

  if (debug) {
    printf("%i\n", *out);
  }

  return 0;
}

struct Stat simulate(struct memory *mem, int start_addr, FILE *log_file,
                     struct symbols *symbols) {
  int run = 1;
  registers_t registers;
  memset(&registers, 0, sizeof(registers_t));
  registers.named.pc = start_addr;

  while (run) {
    int m = memory_read_word(mem, registers.named.pc);
    instruction_t *op = (instruction_t *)&m;

    if (debug)
      printf("pc: %8x\n", registers.named.pc);

    switch (op->opcode) {

    case OP_AUIPC:
      write_register(&registers, op->args.U.rd,
                     registers.named.pc + (op->args.U.imm20 << 12));
      break;
    case OP_ADDI: {
      int rs1;
      read_register(&registers, op->args.I.rs1, &rs1);
      write_register(&registers, op->args.I.rd, rs1 + op->args.I.imm12);
      break;
    }
    case OP_LUI:
      write_register(&registers, op->args.U.rd, op->args.U.imm20 << 12);
      break;

    case OP_JAL:
      write_register(&registers, op->args.J.rd, registers.named.pc + 4);
      registers.named.pc += (op->args.J.imm20 << 1);
      printf("jumping to %8x (%i)\n", registers.named.pc + 4,
             (op->args.J.imm20 << 1));
      break;

    case OP_JALR: {
      int addr_start;
      read_register(&registers, op->args.I.rs1, &addr_start);
      int addr =
          (addr_start + op->args.I.imm12) & 0b11111111111111111111111111111110;
      write_register(&registers, op->args.J.rd, registers.named.pc + 4);
      registers.named.pc = addr;
      printf("jumping to %8x\n", addr);
      break;
    }

    case OP_BEQ: {
      int rs1;
      read_register(&registers, op->args.S.rs1, &rs1);
      int rs2;
      read_register(&registers, op->args.S.rs2, &rs2);
      if (rs1 == rs2) {
        registers.named.pc += (op->args.S.imm31 << 1);
        printf("branching to %8x (%i)\n", registers.named.pc + 4,
               (op->args.S.imm31 << 1));
      }
      break;
    }

    case OP_LW: {
      int addr_start;
      read_register(&registers, op->args.I.rs1, &addr_start);
      int memory_addr = addr_start + op->args.I.imm12;
      int word = memory_read_word(mem, memory_addr);
      write_register(&registers, op->args.I.rd, word);
      break;
    }

    case OP_SW: {
      int addr_start;
      read_register(&registers, op->args.S.rs1, &addr_start);
      int memory_addr = addr_start + op->args.S.imm11;
      int value;
      read_register(&registers, op->args.S.rs2, &value);
      memory_write_word(mem, memory_addr, value);
      break;
    }

    case OP_ECALL: {
      // yah mate idfk
      // TODO: Implement syscalls
    }

    default:
      unknown_instruction(op);
      assert(0);
      break;
    }

    registers.named.pc += 4;
  }

  // TODO: fix
  struct Stat x = {0};
  return x;
}
