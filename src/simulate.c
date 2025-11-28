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

#define DEBUG(...)                                                             \
  if (debug)                                                                   \
  printf(__VA_ARGS__)

void unknown_instruction(instruction_t *op) {
  fprintf(stderr, "Unknown instruction with opcode: %i, ", op->opcode);
  fprintf(stderr, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(op->opcode));
  fprintf(stderr, "\ninstruction: 0x%08x\n", *(unsigned int *)op);
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

  DEBUG("%i\n", *out);

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

    DEBUG("pc: %8x\n", registers.named.pc);
    DEBUG("op: %08x\n", *(unsigned int *)op);

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
      int j_imm = op->args.J.imm20 << 1;
      registers.named.pc += j_imm;
      DEBUG("OP_JAL:\tjumping to %8x (%i)\n", registers.named.pc + 4, j_imm);
      continue;
      break;

    case OP_JALR: {
      int addr_start;
      read_register(&registers, op->args.I.rs1, &addr_start);
      int j_imm = decode_j_immediate(op);
      int addr = (addr_start + j_imm);
      write_register(&registers, op->args.I.rd, registers.named.pc + 4);
      registers.named.pc = addr;
      DEBUG("OP_JALR:\tjumping to %8x\n", addr);
      continue;
      break;
    }

    case OP_BEQ: {
      int rs1;
      read_register(&registers, op->args.S.rs1, &rs1);
      int rs2;
      read_register(&registers, op->args.S.rs2, &rs2);
      int mux = op->args.S.funct3;
      DEBUG("funct3: %i\n", mux);

      int b_immediate = decode_b_immediate(op);
      DEBUG("imm12: %i\n", b_immediate);

      int condition = 0;

      switch (op->args.S.funct3) {
      case 0: // BEQ
        condition = rs1 == rs2;
        break;
      case 1: // BNE
        condition = rs1 != rs2;
        break;
      default:
        fprintf(stderr, "Unknown branch funct3: %i\n", op->args.S.funct3);
        assert(0);
      }

      if (condition) {
        registers.named.pc += b_immediate;
      }
      break;
    }

    case OP_LW: {
      int addr_start;
      read_register(&registers, op->args.I.rs1, &addr_start);
      DEBUG("OP_LW: imm12: %i\n", op->args.I.imm12);
      int memory_addr = addr_start + op->args.I.imm12;

      int word;
      switch (op->args.I.funct3) {
      case 2: {
        word = memory_read_word(mem, memory_addr);
        break;
      }
      case 4: {
        word = memory_read_byte(mem, memory_addr);
        break;
      }
      default:
        fprintf(stderr, "LW Unknown funct3: %i\n", op->args.I.funct3);
        assert(0);
        break;
      }

      DEBUG("OP_LW:\tLoading %i from %8x\n", word, memory_addr);

      write_register(&registers, op->args.I.rd, word);
      break;
    }

    case OP_SW: {
      int addr_start;
      read_register(&registers, op->args.S.rs1, &addr_start);
      int imm12 = decode_s_immediate(op);
      int memory_addr = addr_start + imm12;
      int value;
      read_register(&registers, op->args.S.rs2, &value);
      DEBUG("OP_SW:\tStoring %i in %8x\n", value, memory_addr);
      memory_write_word(mem, memory_addr, value);
      break;
    }

    case OP_ECALL: {
      // Register A7 contains the type of syscall
      DEBUG("Executing ecall: %i\n", registers.named.a7);
      switch (registers.named.a7) {
      case 1:
        registers.named.a0 = getchar();
        break;
      case 2:
        DEBUG("value of A0: %i\n", registers.named.a0);
        putchar(registers.named.a0);
        break;
      case 3:
      case 93:
        run = 0;
        break;
      default:
        fprintf(stderr, "Unknown syscall: %i\n", registers.named.a7);
        assert(0);
        break;
      }
      break;
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
