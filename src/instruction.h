typedef union instruction_args {
  struct arg_addi {
    unsigned int rd : 5;
    int funct3 : 3;
    unsigned int rs1 : 5;
    int imm : 11;
  } addi;
  struct arg_rd_imm20 {
    unsigned int rd : 5;
    int imm20 : 20
  } rd_imm20;
} instruction_args_t;

typedef enum opcode { 
  OP_ADDI = 0b0010011,  // Add immediate
  OP_AUIPC = 0b0010111, // Add upper immediate to PC
  OP_LUI = 0b0110111,   // load upper immediate
  OP_JAL = 0b1101111,   // jump and link
} opcode_t;

typedef struct instruction {
  opcode_t opcode : 7;
  instruction_args_t args;
} instruction_t;
