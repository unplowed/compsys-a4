#pragma once
typedef enum opcode {
  // Add upper immediate to PC
  OP_AUIPC = 23,
  // ADDI adds the sign-extended 12-bit immediate to register rs1
  OP_ADDI = 19,
  // ADD performs the addition of rs1 and rs2
  OP_ADD = 51,
  // Load Upper Immediate
  OP_LUI = 55,
  // jump and link
  OP_JAL = 111,
  // loads a 32-bit value from memory into rd
  OP_LW = 3,
  // stores a 32-bit value from the register rs2 to memory.
  OP_SW = 35,
  // indirect jump instruction JALR (jump and link register)
  OP_JALR = 103,
  // jumps if rs1 == rs2, otherwise goes to next instruction
  OP_BEQ = 99,
  // service call to execution environment
  OP_ECALL = 115,
} opcode_t;

typedef union instruction {
  opcode_t opcode : 7;
  int args;
  unsigned int hex;
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
