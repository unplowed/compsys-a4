#include "disassemble.h"
#include "read_elf.h"
#include "registers.h"
#include <assert.h>
#include <stdio.h>

// NOTE: Use of symbols provide for nicer disassembly, but is not required for
// A4. Feel free to remove this parameter or pass in a NULL pointer and ignore
// it.

void disassemble(uint32_t addr, uint32_t instruction, char *result,
                 size_t buf_size, struct symbols *symbols) {
  instruction_t op = *(instruction_t *)&instruction;
  switch (op.opcode) {
  case OP_ADDI: {
    int funct3 = extract_bits_instruction(&op, 14, 12);
    switch (funct3) {
    // addi
    case 0: {
      int imm12 = decode_i_immediate(&op);
      int rd = extract_bits_instruction(&op, 11, 7);
      int rs1 = extract_bits_instruction(&op, 19, 15);
      snprintf(result, buf_size, "addi %s, %s, %i", register_names[rd],
               register_names[rs1], imm12);
      break;
    }
    // slli
    case 1: {
      int rd = extract_bits_instruction(&op, 11, 7);
      snprintf(result, buf_size, "slli %s, %u", register_names[rd],
               decode_i_immediate(&op));
      break;
    }
    // xori
    case 4: {
      int rd = extract_bits_instruction(&op, 11, 7);
      int rs1 = extract_bits_instruction(&op, 19, 15);
      int imm12 = decode_i_immediate(&op);
      int imm = sign_extend(imm12, 12);
      snprintf(result, buf_size, "xori %s, %s, %i", register_names[rd],
               register_names[rs1], imm);
      break;
    }
    // srai
    case 5: {
      int rd = extract_bits_instruction(&op, 11, 7);
      snprintf(result, buf_size, "srai %s, %u", register_names[rd],
               decode_i_immediate(&op));
      break;
    }
    // andi
    case 7: {
      int rd = extract_bits_instruction(&op, 11, 7);
      int rs1 = extract_bits_instruction(&op, 19, 15);
      snprintf(result, buf_size, "andi %s, %s, %i", register_names[rd],
               register_names[rs1], decode_i_immediate(&op));
      break;
    }
    default:
      snprintf(result, buf_size,
               "unknown instruction type. addi with funct3 of %i", funct3);
      break;
    }
    break;
  }

  case OP_ADD: {
    int rd = extract_bits_instruction(&op, 11, 7);
    int rs1 = extract_bits_instruction(&op, 19, 15);
    int rs2 = extract_bits_instruction(&op, 24, 20);
    snprintf(result, buf_size, "add %s, %s, %s", register_names[rd],
             register_names[rs1], register_names[rs2]);
    break;
  }

  case OP_SW: {
    int rs1 = extract_bits_instruction(&op, 19, 15);
    int rs2 = extract_bits_instruction(&op, 24, 20);
    int funct3 = extract_bits_instruction(&op, 14, 12);
    switch (funct3) {
    // sb
    case 0: {
      snprintf(result, buf_size, "sb %s, %i(%s)", register_names[rs2],
               decode_s_immediate_sign_extended(&op), register_names[rs1]);
      break;
    }
    // sw
    case 2: {
      snprintf(result, buf_size, "sw %s, %i(%s)", register_names[rs2],
               decode_s_immediate_sign_extended(&op), register_names[rs1]);
      break;
    }
    }
    break;
  }

  case OP_LW: {
    int funct3 = extract_bits_instruction(&op, 14, 12);
    switch (funct3) {
    // lw
    case 2: {
      int rd = extract_bits_instruction(&op, 11, 7);
      int rs1 = extract_bits_instruction(&op, 19, 15);
      snprintf(result, buf_size, "lw %s, %s, %i", register_names[rd],
               register_names[rs1], decode_i_immediate(&op));
      break;
    }
    // lbu
    case 4: {
      int rd = extract_bits_instruction(&op, 11, 7);
      int rs1 = extract_bits_instruction(&op, 19, 15);
      snprintf(result, buf_size, "lbu %s, %s, %i", register_names[rd],
               register_names[rs1], decode_i_immediate(&op));
      break;
    }
    default:
      snprintf(result, buf_size,
               "unknown instruction type. lw with funct3 of %i", funct3);
      break;
    }
    break;
  }

  case OP_LUI: {
    int rd = extract_bits_instruction(&op, 11, 7);
    snprintf(result, buf_size, "lui %s, %i", register_names[rd],
             decode_u_immediate(&op) << 12);
    break;
  }

  case OP_JAL: {
    int rd = extract_bits_instruction(&op, 11, 7);
    snprintf(result, buf_size, "jal %s, %i", register_names[rd],
             decode_j_immediate_sign_extended(&op));
    break;
  }

  case OP_JALR: {
    int rd = extract_bits_instruction(&op, 11, 7);
    int rs1 = extract_bits_instruction(&op, 19, 15);
    int imm12 = decode_i_immediate(&op);
    int imm = sign_extend(imm12, 12);
    snprintf(result, buf_size, "jalr %s, %s, %i", register_names[rd],
             register_names[rs1], imm);
    break;
  }

  case OP_BEQ: {
    int rs1 = extract_bits_instruction(&op, 19, 15);
    int rs2 = extract_bits_instruction(&op, 24, 20);
    int funct3 = extract_bits_instruction(&op, 14, 12);
    switch (funct3) {
    // BEQ
    case 0: {
      snprintf(result, buf_size, "beq %s, %s, %i", register_names[rs1],
               register_names[rs2], decode_b_immediate_sign_extended(&op));
      break;
    }
    // BNE
    case 1: {
      snprintf(result, buf_size, "bne %s, %s, %i", register_names[rs1],
               register_names[rs2], decode_b_immediate_sign_extended(&op));
      break;
    }
    // BLT
    case 4: {
      snprintf(result, buf_size, "blt %s, %s, %i", register_names[rs1],
               register_names[rs2], decode_b_immediate_sign_extended(&op));
      break;
    }
    // BGE
    case 5: {
      snprintf(result, buf_size, "bge %s, %s, %i", register_names[rs1],
               register_names[rs2], decode_b_immediate_sign_extended(&op));
      break;
    }
    // BLTU
    case 6: {
      snprintf(result, buf_size, "bltu %s, %s, %i", register_names[rs1],
               register_names[rs2], decode_b_immediate_sign_extended(&op));
      break;
    }

    // BGEU
    case 7: {
      snprintf(result, buf_size, "bgeu %s, %s, %i", register_names[rs1],
               register_names[rs2], decode_b_immediate_sign_extended(&op));
      break;
    }

    default: {
      snprintf(result, buf_size, "beq: unknown funct3");
    }
    }
    break;
  }

  case OP_AUIPC: {
    int rd = extract_bits_instruction(&op, 11, 7);
    snprintf(result, buf_size, "auipc %s, %i", register_names[rd],
             decode_u_immediate(&op) << 12);
    break;
  }

  case OP_ECALL:
    snprintf(result, buf_size, "ecall");
    break;

  default:
    snprintf(result, buf_size, "unknown instruction type");
  }
}
