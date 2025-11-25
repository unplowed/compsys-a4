typedef union instruction_args {
  struct arg_addi {
    unsigned int rd : 5;
    int funct3 : 3;
    unsigned int rs1 : 5;
    int imm : 11;
  } addi;
} instruction_args_t;

typedef enum opcode { 
  OP_ADDI = 0b0010011
} opcode_t;

typedef struct instruction {
  opcode_t opcode : 7;
  instruction_args_t args;
} instruction_t;
