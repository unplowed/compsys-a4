#include "instruction.h"
#include <assert.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)                                                   \
  ((byte) & 0x80 ? '1' : '0'), ((byte) & 0x40 ? '1' : '0'),                    \
      ((byte) & 0x20 ? '1' : '0'), ((byte) & 0x10 ? '1' : '0'),                \
      ((byte) & 0x08 ? '1' : '0'), ((byte) & 0x04 ? '1' : '0'),                \
      ((byte) & 0x02 ? '1' : '0'), ((byte) & 0x01 ? '1' : '0')

// Extracts [`end`:`start`] bits from `number`
int extract_bits(int number, int end, int start) {
  // Extracts bits using (int32 >> i) & ((1 << n) - 1)
  // where i is the lowest bit in the sequence to be extracted and
  // n is the length of the sequence.

  int length = (end - start) + 1; // +1 since the most significant bit is inclusive
  int mask = (1 << length) - 1;   // Mask contains `length` 1's starting from the least-significant-bit.
                                  // This effectively clears any bits above the desired value

  return (number >> start) & mask;
}

int extract_bits_instruction(instruction_t *instruction, int end, int start) {
  return extract_bits(*(int *)instruction, end, start);
}

// Sign extends a number of a given length.
int sign_extend(unsigned int number, int length) {
  int ret = number;

  if (number & ((1 << length) - (1 << (length - 1)))) {
    ret |= 0b1111111111111111111111111111111 << length;
  }

  return ret;
}

int decode_i_immediate(instruction_t *instruction) {
  int *intstruction = (int *)instruction;
  int ret = (int)(*intstruction >> 20);
  return ret;
}

int decode_i_immediate_sign_extended(instruction_t *instruction) {
  int intstruction = *(int *)instruction;

  // Extracting bits using (int32 >> i) & ((1 << n) - 1)
  // where i is the lowest bit in the sequence to be extracted and
  // n is the length of the sequence.

  int imm1 = extract_bits(intstruction, 20, 20);
  int imm4 = extract_bits(intstruction, 24, 21);
  // extract 1 more bit for sign bit
  int imm6 = extract_bits(intstruction, 31, 25);

  int imm11 = imm1 | imm4 << 1 | imm6 << 5;

  return sign_extend(imm11, 12);
}

int decode_s_immediate_sign_extended(instruction_t *instruction) {
  int intstruction = *(int *)instruction;
  unsigned int imm7 = (intstruction >> 25) & 0b1111111;
  unsigned int imm5 = (intstruction >> 7) & 0b11111;
  unsigned int imm12 = ((imm7 << 5) | (imm5));
  // check sign bit
  if (imm12 & 0b100000000000) {
    imm12 |= 0b11111111111111111111100000000000;
  }
  return imm12;
}

int decode_b_immediate_sign_extended(instruction_t *instruction) {

  int intstruction = *(int *)instruction;

  // bits [11:8]
  unsigned int imm4 = (intstruction >> 8) & ((1 << 4) - 1);
  // bits [30:25]
  unsigned int imm6 = (intstruction >> 25) & ((1 << 6) - 1);
  // bit [7]
  unsigned int imm1 = (intstruction >> 7) & 0x1;
  unsigned int imm12 = (imm1 << 11) | (imm6 << 5) | imm4 << 1;

  if (intstruction >> 31) { // 31st bit set
    // sign extend 20 bits
    imm12 |= 0b11111111111111111111000000000000;
  }

  return imm12;
}

int decode_u_immediate(instruction_t *instruction) {
  int *intstruction = (int *)instruction;
  int ret = (int)(*intstruction >> 12);
  return ret;
}

int decode_j_immediate_sign_extended(instruction_t *instruction) {
  int intstruction = *(int *)instruction;

  // Extracting bits using (int32 >> i) & ((1 << n) - 1)
  // where i is the lowest bit in the sequence to be extracted and
  // n is the length of the sequence.

  // bits [24:21[
  unsigned int imm4 = (intstruction >> 21) & ((1 << 4) - 1);
  // bits [30:25[
  unsigned int imm6 = (intstruction >> 25) & ((1 << 6) - 1);
  // bit [20]
  unsigned int imm1 = (intstruction >> 20) & 0x1;
  // bits [19:12[
  unsigned int imm8 = (intstruction >> 12) & ((1 << 8) - 1);
  unsigned int imm20 = (imm8 << 12) | (imm1 << 11) | (imm6 << 5) | imm4 << 1;

  if (intstruction >> 31) { // 31st bit set
    // sign extend top 12 bits
    imm20 |= 0b11111111111100000000000000000000;
  }

  return imm20;
}
