#pragma once
typedef enum opcode {
  // Add upper immediate to PC
  OP_AUIPC = 0b0010111,
  // ADDI adds the sign-extended 12-bit immediate to register rs1
  OP_ADDI = 0b0010011,
  // ADD performs the addition of rs1 and rs2
  OP_ADD = 0b0110011,
  // Load Upper Immediate
  OP_LUI = 0b0110111,
  // jump and link
  OP_JAL = 0b1101111,
  // loads a 32-bit value from memory into rd
  OP_LW = 0b0000011,
  // stores a 32-bit value from the register rs2 to memory.
  OP_SW = 0b0100011,
  // indirect jump instruction JALR (jump and link register)
  OP_JALR = 0b1100111,
  // jumps if rs1 == rs2, otherwise goes to next instruction
  OP_BEQ = 0b1100011,
  // service call to execution environment
  OP_ECALL = 0b1110011,
} opcode_t;

typedef union __attribute__((packed)) instruction {
  opcode_t opcode : 7;
  int args;
} instruction_t;

// Extracts the bits [end:start]
int extract_bits(int number, int end, int start);
int extract_bits_instruction(instruction_t *instruction, int end, int start);

// Sign extends a number of a given length.
int sign_extend(unsigned int number, int length);

int decode_i_immediate(instruction_t *instruction);
int decode_i_immediate_sign_extended(instruction_t *instruction);
int decode_s_immediate(instruction_t *instruction);
int decode_s_immediate_sign_extended(instruction_t *instruction);
int decode_b_immediate(instruction_t *instruction);
int decode_b_immediate_sign_extended(instruction_t *instruction);
int decode_u_immediate(instruction_t *instruction);
int decode_u_immediate_sign_extended(instruction_t *instruction);
int decode_j_immediate(instruction_t *instruction);
int decode_j_immediate_sign_extended(instruction_t *instruction);
