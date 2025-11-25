typedef union __attribute__((packed)) instruction_args {
  struct __attribute__((packed)) r_type {
    unsigned int rd : 5;
    unsigned int funct3 : 3;
    unsigned int rs1 : 5;
    unsigned int rs2 : 5;
    unsigned int funct7 : 7;
  } R;
  struct __attribute__((packed)) i_type {
    unsigned int rd : 5;
    unsigned int funct3 : 3;
    unsigned int rs1 : 5;
    int imm12 : 12;
  } I;
  struct __attribute__((packed)) s_type {
    int imm11 : 5;
    unsigned int funct3 : 3;
    unsigned int rs1 : 5;
    unsigned int rs2 : 5;
    int imm31 : 7;
  } S;
  struct __attribute__((packed)) u_type {
    unsigned int rd : 5;
    int imm20 : 20;
  } U;
  struct __attribute__((packed)) j_type {
    unsigned int rd : 5;
    unsigned int imm19 : 8;
    unsigned int imm20 : 1;
    unsigned int imm30 : 10;
    unsigned int imm31 : 1;
  } J;
} instruction_args_t;

typedef enum opcode {
  OP_AUIPC = 0b0010111, // Add upper immediate to PC
  OP_ADDI =  0b0010011, // ADDI adds the sign-extended 12-bit immediate to register rs1
} opcode_t;

typedef struct __attribute__((packed)) instruction {
  opcode_t opcode : 7 __attribute__((packed));
  instruction_args_t args __attribute__((packed));
} instruction_t;
