#ifndef __SIMULATE_H__
#define __SIMULATE_H__

#include "instruction.h"
#include "memory.h"
#include "read_elf.h"
#include "registers.h"
#include <stdio.h>

// Simuler RISC-V program i givet lager og fra given start adresse
struct Stat {
  long int instructions;
};

// NOTE: Use of symbols provide for nicer disassembly, but is not required for
// A4. Feel free to remove this parameter or pass in a NULL pointer and ignore
// it.
struct Stat simulate(struct memory *mem, int start_addr, FILE *log_file,
                     struct symbols *symbols);

int simulate_single(struct memory *mem, registers_t *registers, FILE *log_file,
                    instruction_t *op);

int write_register(registers_t *registers, int register_idx, int value);
int read_register(const registers_t *registers, int register_idx, int *out);

#endif
