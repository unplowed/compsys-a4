#ifndef __MEMORY_H__
#define __MEMORY_H__

struct memory;

// opret/nedlæg lager
struct memory *memory_create();
void memory_delete(struct memory *);

// skriv word/halfword/byte til lager
void memory_write_word(struct memory *mem, int addr, int data);
void memory_write_halfword(struct memory *mem, int addr, int data);
void memory_write_byte(struct memory *mem, int addr, int data);

// læs word/halfword/byte fra lager - data er nul-forlænget
int memory_read_word(struct memory *mem, int addr);
int memory_read_halfword(struct memory *mem, int addr);
int memory_read_byte(struct memory *mem, int addr);
#endif
