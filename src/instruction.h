typedef enum opcode {
  // Add upper immediate to PC
  OP_AUIPC = 0b0010111,
  // ADDI adds the sign-extended 12-bit immediate to register rs1
  OP_ADDI = 0b0010011,
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

typedef union __attribute__((packed)) instruction_args {
  struct __attribute__((packed)) r_type {
    opcode_t opcode : 7;
    unsigned int rd : 5;
    unsigned int funct3 : 3;
    unsigned int rs1 : 5;
    unsigned int rs2 : 5;
    unsigned int funct7 : 7;
  } R;
  struct __attribute__((packed)) i_type {
    opcode_t opcode : 7;
    unsigned int rd : 5;
    unsigned int funct3 : 3;
    unsigned int rs1 : 5;
    int imm12 : 12;
  } I;
  struct __attribute__((packed)) s_type {
    opcode_t opcode : 7;
    int imm11 : 5;
    unsigned int funct3 : 3;
    unsigned int rs1 : 5;
    unsigned int rs2 : 5;
    int imm31 : 7;
  } S;
  struct __attribute__((packed)) u_type {
    opcode_t opcode : 7;
    unsigned int rd : 5;
    int imm20 : 20;
  } U;
  struct __attribute__((packed)) j_type {
    opcode_t opcode : 7;
    unsigned int rd : 5;
    int imm11 : 9;
    int imm20 : 11;
  } J;
} instruction_args_t;

typedef union __attribute__((packed)) instruction {
  opcode_t opcode : 7;
  instruction_args_t args;
} instruction_t;
