#pragma once
#include "instruction.h"

typedef struct vm_registers {
  int zero; // x0
  int ra; // x1, return address
  int sp; // x2, stack pointer
  int gp; // x3, global pointer
  int tp; // x4, thread pointer
  int t0; // x5, temporary 0
  int t1; // x6, temporary 1
  int t2; // x7, temporary 2
  int s0_fp; // x8, saved register 0 / frame pointer
  int s1; // x9 saved register 1
  int a0; // x10, function argument 0 / return value 0
  int a1; // x11, function argument 1 / return value 1
  int a2; // x12, function argument 2
  int a3; // x13, function argument 3
  int a4; // x14, function argument 4
  int a5; // x15, function argument 5
  int a6; // x16, function argument 6
  int a7; // x17, function argument 7
 	int s2; // x18, saved register 2
 	int s3; // x19, saved register 3
 	int s4; // x20, saved register 4
 	int s5; // x21, saved register 5
 	int s6; // x22, saved register 6
 	int s7; // x23, saved register 7
 	int s8; // x24, saved register 8
 	int s9; // x25, saved register 9
 	int s10; // saved x26, register 10
 	int s11; // saved x27, register 11
 	int t3; // 	x28, temporary register 3 
 	int t4; // 	x29, temporary register 4 
 	int t5; // 	x30, temporary register 5 
 	int t6; // 	x31, temporary register 6 
} vm_registers_t;

typedef enum registers_idx_map {
  REG_ZERO = 0, 
  REG_RA = 1, 
  REG_SP = 2, 
  REG_GP = 3, 
  REG_TP = 4, 
  REG_T0 = 5, 
  REG_T1 = 6, 
  REG_T2 = 7, 
  REG_S0_FP = 8, 
  REG_S1 = 9, 
  REG_A0 = 10, 
  REG_A1 = 11, 
  REG_A2 = 12, 
  REG_A3 = 13, 
  REG_A4 = 14, 
  REG_A5 = 15, 
  REG_A6 = 16, 
  REG_A7 = 17, 
 	REG_S2 = 18, 
 	REG_S3 = 19, 
 	REG_S4 = 20, 
 	REG_S5 = 21, 
 	REG_S6 = 22, 
 	REG_S7 = 23, 
 	REG_S8 = 24, 
 	REG_S9 = 25, 
 	REG_S10 = 26, 
 	REG_S11 = 27, 
 	REG_T3 = 28, 
 	REG_T4 = 29, 
 	REG_T5 = 30, 
 	REG_T6 = 31, 
} vm_registers_idx_t;

typedef struct VM {
  instruction_t *ip;

  void *memory;
  unsigned long memory_size;
  unsigned long instructions_size;

  vm_registers_t registers;
} VM_t;

void init_vm(VM_t *vm, instruction_t *instructions,
             unsigned long instructions_size, unsigned long memory_size);
int step_vm(VM_t *vm);
void free_vm(VM_t *vm);
int write_register(VM_t *vm, int register_idx, int value);
int read_register(VM_t *vm, int register_idx);

typedef enum {
  VM_EXIT_END_OF_INSTRUCTIONS = 1,
  VM_EXIT_OUT_OF_BOUNDS_INSTRUCTION = 2,
  VM_EXIT_ILLEGAL_REGISTER_WRITE = 3,
} VM_EXIT_CODES;

const char* exit_codes[] = {
  "Ran successfully",
  "reached end of instructions",
  "instruction was out of bounds",
  "Illegal registry write"
};
