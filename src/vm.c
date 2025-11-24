#include "vm.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG(...)                                                             \
  if (debug)                                                                   \
  printf(__VA_ARGS__)

int debug = 1;

void init_vm(VM_t *vm, instruction_t *instructions,
             unsigned long instructions_size, unsigned long memory_size) {

  DEBUG("Initializing VM with %lu bytes of memory and %lu instructions\n\n",
        memory_size, instructions_size / sizeof(instruction_t));

  vm->memory = malloc(memory_size);
  vm->memory_size = memory_size;
  vm->ip = vm->memory;
  vm->instructions_size = instructions_size;
  memset(&vm->registers, 0, sizeof(vm_registers_t));
  memcpy(vm->memory, instructions, memory_size);
}

int step_vm(VM_t *vm) {
  instruction_t *op = vm->ip;

  DEBUG("instruction pointer:  %p\n", op);
  DEBUG("instructions boundry: %p-%p\n", vm->memory,
        vm->memory + vm->instructions_size);

  // Bounds check
  if (((unsigned long)op >
       (unsigned long)(vm->memory + vm->instructions_size)) ||
      ((unsigned long)op < (unsigned long)(vm->memory))) {
    return VM_EXIT_OUT_OF_BOUNDS_INSTRUCTION;
  }

  DEBUG("instruction type: %i\n", op->opcode);
  switch (op->opcode) {
  case OP_ADDI:
    if (op->args.addi.rd == REG_ZERO) // nop
      break;

    int b = read_register(vm, op->args.addi.rs1);
    int imm = op->args.addi.imm;
    int ret = write_register(vm, op->args.addi.rd, b + imm);
    if (ret != 0)
      return ret;

    break;
  }

  vm->ip += sizeof(instruction_t);

  if ((unsigned long)vm->ip ==
      (unsigned long)(vm->memory + vm->instructions_size)) {
    return VM_EXIT_END_OF_INSTRUCTIONS;
  }
  if ((unsigned long)vm->ip >
      (unsigned long)(vm->memory + vm->instructions_size)) {
    return VM_EXIT_OUT_OF_BOUNDS_INSTRUCTION;
  }

  return -1;
}

int write_register(VM_t *vm, int register_idx, int value) {
  if (debug)
    assert(register_idx < 32 && register_idx > 0);

  // ignore writes to zero and gp
  switch (register_idx) {
  case REG_ZERO:
  case REG_GP:
    return 0;
  }

  memcpy((int *)&vm->registers + register_idx, &value, sizeof(int));
  return 0;
}

int read_register(VM_t *vm, int register_idx) {
  if (debug)
    assert(register_idx < 32 && register_idx > 0);

  return *((int *)&vm->registers + register_idx);
}

void free_vm(VM_t *vm) {
  free(vm->memory);
  free(vm);
}

int main(int argv, char *argc[]) {
  VM_t *vm = malloc(sizeof(VM_t));

  instruction_t instructions[] = {
      {OP_ADDI, {.addi = {0}}}, // nop

      {OP_ADDI, {.addi = {.rd = REG_T0, .funct3 = 0, .rs1 = REG_T0, .imm = 1}}},
      {OP_ADDI, {.addi = {.rd = REG_T0, .funct3 = 0, .rs1 = REG_T0, .imm = 1}}},
      {OP_ADDI, {.addi = {.rd = REG_T0, .funct3 = 0, .rs1 = REG_T0, .imm = 1}}},
      {OP_ADDI, {.addi = {.rd = REG_T0, .funct3 = 0, .rs1 = REG_T0, .imm = 1}}},
      {OP_ADDI, {.addi = {.rd = REG_T0, .funct3 = 0, .rs1 = REG_T0, .imm = 1}}},
      {OP_ADDI, {.addi = {.rd = REG_T0, .funct3 = 0, .rs1 = REG_T0, .imm = 1}}},

  };

  init_vm(vm, instructions, sizeof(instructions), 4096);

  int ret = -1;
  do {
    ret = step_vm(vm);
  } while (ret == -1);

  fprintf(stderr, "VM Finished execution\n");

  free_vm(vm);
  fprintf(stderr, "VM Exitted with code %i:\n\t%s\n", ret, exit_codes[ret]);
  return ret;
}
