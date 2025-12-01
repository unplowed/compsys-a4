#include "simulate.h"
#include "disassemble.h"
#include "instruction.h"
#include "memory.h"
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
  fprintf(stderr, __VA_ARGS__)

void unknown_instruction(instruction_t *op) {
  fprintf(stderr, "\x1b[0;33m   ^^^^^   ^^^^^^^^\x1b[0m\n");
  fprintf(stderr,
          "\x1b[0;31mError:\x1b[0m Unknown instruction with opcode %i\n\n",
          op->opcode);
}

int write_register(registers_t *registers, int register_idx, int value) {
  if (debug) {
    DEBUG("writing %i to %i (%s)\n", value, register_idx,
          register_names[register_idx]);

    if (register_idx < 0 || register_idx > 31) {
      DEBUG("Out of bounds write!\n");
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
    DEBUG("reading register %i (%s): ", register_idx,
          register_names[register_idx]);

    if (register_idx < 0 || register_idx > 31) {
      DEBUG("Out of bounds read!\n");
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

  struct Stat stats = {0};

  while (run) {
    unsigned int m = memory_read_word(mem, registers.named.pc);
    instruction_t *op = (instruction_t *)&m;

    if (debug) {
      char disassembly[100];
      disassemble(registers.named.pc, m, disassembly, 100, NULL);
      DEBUG("%8x : %08X       %s\n", registers.named.pc, m, disassembly);
      fflush(stderr);
    }

    switch (simulate_single(mem, &registers, log_file, op)) {
    case 1: // end simulation
      run = 0;
      break;
    case -1: // Unknown instruction
      unknown_instruction(op);
      assert(0);
      break;
    }

    stats.instructions++;

    // switch (op->opcode) {
    //
    // case OP_AUIPC:
    //   write_register(&registers, op->args.U.rd,
    //                  registers.named.pc + (op->args.U.imm20 << 12));
    //   break;
    // case OP_ADDI: {
    //   int rs1;
    //   read_register(&registers, op->args.I.rs1, &rs1);
    //   write_register(&registers, op->args.I.rd, rs1 + op->args.I.imm12);
    //   break;
    // }
    // case OP_LUI:
    //   write_register(&registers, op->args.U.rd, op->args.U.imm20 << 12);
    //   break;
    //
    // case OP_JAL:
    //   write_register(&registers, op->args.J.rd, registers.named.pc + 4);
    //   int j_imm = op->args.J.imm20 << 1;
    //   registers.named.pc += j_imm;
    //   DEBUG("OP_JAL:\tjumping to %8x (%i)\n", registers.named.pc + 4, j_imm);
    //   continue;
    //   break;
    //
    // case OP_JALR: {
    //   int addr_start;
    //   read_register(&registers, op->args.I.rs1, &addr_start);
    //   int j_imm = decode_j_immediate(op);
    //   int addr = (addr_start + j_imm);
    //   write_register(&registers, op->args.I.rd, registers.named.pc + 4);
    //   registers.named.pc = addr;
    //   DEBUG("OP_JALR:\tjumping to %8x\n", addr);
    //   continue;
    //   break;
    // }
    //
    // case OP_BEQ: {
    //   int rs1;
    //   read_register(&registers, op->args.S.rs1, &rs1);
    //   int rs2;
    //   read_register(&registers, op->args.S.rs2, &rs2);
    //   int mux = op->args.S.funct3;
    //   DEBUG("funct3: %i\n", mux);
    //
    //   int b_immediate = decode_b_immediate(op);
    //   DEBUG("imm12: %i\n", b_immediate);
    //
    //   int condition = 0;
    //
    //   switch (op->args.S.funct3) {
    //   case 0: // BEQ
    //     condition = rs1 == rs2;
    //     break;
    //   case 1: // BNE
    //     condition = rs1 != rs2;
    //     break;
    //   default:
    //     fprintf(stderr, "Unknown branch funct3: %i\n", op->args.S.funct3);
    //     assert(0);
    //   }
    //
    //   if (condition) {
    //     registers.named.pc += b_immediate;
    //   }
    //   break;
    // }
    //
    // case OP_LW: {
    //   int addr_start;
    //   read_register(&registers, op->args.I.rs1, &addr_start);
    //   DEBUG("OP_LW: imm12: %i\n", op->args.I.imm12);
    //   int memory_addr = addr_start + op->args.I.imm12;
    //
    //   int word;
    //   switch (op->args.I.funct3) {
    //   case 2: {
    //     word = memory_read_word(mem, memory_addr);
    //     break;
    //   }
    //   case 4: {
    //     word = memory_read_byte(mem, memory_addr);
    //     break;
    //   }
    //   default:
    //     fprintf(stderr, "LW Unknown funct3: %i\n", op->args.I.funct3);
    //     assert(0);
    //     break;
    //   }
    //
    //   DEBUG("OP_LW:\tLoading %i from %8x\n", word, memory_addr);
    //
    //   write_register(&registers, op->args.I.rd, word);
    //   break;
    // }
    //
    // case OP_SW: {
    //   int addr_start;
    //   read_register(&registers, op->args.S.rs1, &addr_start);
    //   int imm12 = decode_s_immediate_sign_extended(op);
    //   int memory_addr = addr_start + imm12;
    //   int value;
    //   read_register(&registers, op->args.S.rs2, &value);
    //   DEBUG("OP_SW:\tStoring %i in %8x\n", value, memory_addr);
    //   memory_write_word(mem, memory_addr, value);
    //   break;
    // }
    //
    // case OP_ECALL: {
    //   // Register A7 contains the type of syscall
    //   DEBUG("Executing ecall: %i\n", registers.named.a7);
    //   switch (registers.named.a7) {
    //   case 1:
    //     registers.named.a0 = getchar();
    //     break;
    //   case 2:
    //     DEBUG("value of A0: %i\n", registers.named.a0);
    //     putchar(registers.named.a0);
    //     break;
    //   case 3:
    //   case 93:
    //     run = 0;
    //     break;
    //   default:
    //     fprintf(stderr, "Unknown syscall: %i\n", registers.named.a7);
    //     assert(0);
    //     break;
    //   }
    //   break;
    // }
    //
    // default:
    //   unknown_instruction(op);
    //   assert(0);
    //   break;
    // }

    registers.named.pc += 4;
  }

  return stats;
}

int simulate_single(struct memory *mem, registers_t *registers, FILE *log_file,
                    instruction_t *op) {
  switch (op->opcode) {

  case OP_AUIPC: {
    int imm20 = decode_u_immediate(op);
    int rd = extract_bits_instruction(op, 11, 7);
    write_register(registers, rd, imm20 << 12);
    break;
  }

  case OP_ADDI: {
    int funct3 = extract_bits_instruction(op, 14, 12);
    switch (funct3) {
    // addi
    case 0: {
      int imm12 = decode_i_immediate(op);
      int imm = sign_extend(imm12, 12);
      int rd = extract_bits_instruction(op, 11, 7);
      int rs1 = extract_bits_instruction(op, 19, 15);
      int rs1_value;
      read_register(registers, rs1, &rs1_value);
      write_register(registers, rd, rs1_value + imm);
      break;
    }
    default:
      DEBUG("Unknown funct3 %i\n", funct3);
      return -1;
    };
    break;
  }

  case OP_LUI: {
    int rd = extract_bits_instruction(op, 11, 7);
    int imm12 = extract_bits_instruction(op, 31, 12);
    int imm = imm12 << 12;
    write_register(registers, rd, imm);
    break;
  }

  case OP_JAL: {
    int rd = extract_bits_instruction(op, 11, 7);
    int imm20 = decode_j_immediate_sign_extended(op);
    int imm = imm20 << 1;
    write_register(registers, rd, registers->named.pc + 4);
    DEBUG("Jumping by %i\n", imm);
    registers->named.pc += imm;
    break;
  }

  case OP_SW: {
    int rs1 = extract_bits_instruction(op, 19, 15);
    int rs2 = extract_bits_instruction(op, 24, 20);
    int imm = decode_s_immediate_sign_extended(op);
    int addr_start;
    read_register(registers, rs1, &addr_start);
    int rs2_value;
    read_register(registers, rs2, &rs2_value);

    DEBUG("imm: %i\n", imm);
    DEBUG("Writing %8x to %00008x\n", rs2_value, addr_start + imm);
    memory_write_word(mem, addr_start + imm, rs2_value);
    break;
  }

  case OP_LW: {
    int funct3 = extract_bits_instruction(op, 14, 12);
    int rd = extract_bits_instruction(op, 11, 7);
    int rs1 = extract_bits_instruction(op, 19, 15);
    int imm = decode_i_immediate(op);
    int addr_start;
    read_register(registers, rs1, &addr_start);
    DEBUG("imm: %i\n", imm);
    switch (funct3) {
    // lw
    case 2: {
      int word = memory_read_word(mem, addr_start + imm);
      DEBUG("Read %8x from %00008x\n", word, addr_start + imm);
      write_register(registers, rd, word);
      break;
    }
    // lbu
    case 4: {
      int word = memory_read_byte(mem, addr_start + imm);
      DEBUG("Read %2x from %00008x\n", word, addr_start + imm);
      write_register(registers, rd, word);
      break;
    }
    default:
      DEBUG("Unknown funct3 %i\n", funct3);
      return -1;
    };
    break;
  }

  case OP_JALR: {
    int rd = extract_bits_instruction(op, 11, 7);
    int rs1 = extract_bits_instruction(op, 19, 15);
    int imm12 = decode_i_immediate(op);
    int imm = sign_extend(imm12, 12);
    read_register(registers, rs1, &rs1);

    // set LSB to 0
    int target_addr = ((rs1 + imm) >> 1) << 1;

    // write current pc to rd
    write_register(registers, rd, registers->named.pc + 4);

    DEBUG("Jumping to %i\n", target_addr);
    registers->named.pc = target_addr;
    break;
  }

  case OP_BEQ: {
    int rs1 = extract_bits_instruction(op, 19, 15);
    int rs2 = extract_bits_instruction(op, 24, 20);
    int imm12 = decode_b_immediate_sign_extended(op);
    read_register(registers, rs1, &rs1);
    read_register(registers, rs2, &rs2);

    DEBUG("imm: %i\n", imm12);

    int funct3 = extract_bits_instruction(op, 14, 12);
    switch (funct3) {

    // beq
    case 0: {
      if (rs1 == rs2) {
        registers->named.pc += imm12;
        DEBUG("branching to %i\n", registers->named.pc);
      }
      break;
    }

    // bne
    case 1: {
      if (rs1 != rs2) {
        registers->named.pc += imm12;
        DEBUG("branching to %i\n", registers->named.pc);
      }
      break;
    }

    default:
      DEBUG("Unknown funct3 %i\n", funct3);
      return -1;
    };
    break;
  }

  case OP_ECALL: {
    switch (registers->named.a7) {
    case 1:
      registers->named.a0 = getchar();
      break;

    case 2:
      putchar(registers->named.a0);
      break;

    case 3:
    case 93:
      return 1;
      break;

    default:
      fprintf(stderr, "Unknown ecall %i\n", registers->named.a7);
      return -1;
      break;
    }
    break;
  }

  default: // Unknown or unimplemented instruction
    return -1;
    break;
  }

  return 0;
}
