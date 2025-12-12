#include "branch_prediction.h"
#include "disassemble.h"
#include "instruction.h"
#include "memory.h"
#include "simulate.h"
#include "test.h"
#include <stdlib.h>
#include <time.h>

/* --------- IMMEDIATES TESTS -------- */
// Runs at the start of every test
void *immediate_init() { return 0; };
// Cleans up after every test
void immediate_cleanup(void *data) {};

int i_immediate(void *_) {
#define TEST_SIZE 5
  int data[TEST_SIZE][2] = {
      {0x7FF00013, 2047},
      {0x80000013, -2048},
      {0xFFF00013, -1},
      {0x00000013, 0},
  };

  for (int i = 0; i < TEST_SIZE; i++) {
    int imm = decode_i_immediate((instruction_t *)&data[i][0]);
    ASSERT_EQ(imm, data[i][1]);
  }

  for (int i = 0; i < 2 << 11; i++) {
    int instruction = (i << 20) + 0x13;
    int imm = decode_i_immediate((instruction_t *)&instruction);
    int cmp = i;
    if (cmp >= 2048) {
      cmp -= 4096;
    }
    ASSERT_EQ(imm, cmp);
  }

  return 0;
}

int i_immediate_sign_extended(void *_) {
  int data[TEST_SIZE][2] = {
      {0x7FF00013, 2047},
      {0x80000013, -2048},
      {0xFFF00013, -1},
      {0x00000013, 0},
  };

  for (int i = 0; i < TEST_SIZE; i++) {
    int imm = decode_i_immediate_sign_extended((instruction_t *)&data[i][0]);
    ASSERT_EQ(imm, data[i][1]);
  }

  for (int i = 0; i < 2 << 11; i++) {
    int instruction = (i << 20) + 0x13;
    int imm = decode_i_immediate((instruction_t *)&instruction);
    int cmp = i;
    if (cmp >= 2048) {
      cmp -= 4096;
    }
    ASSERT_EQ(imm, cmp);
  }

  return 0;
}

// unsure if non-sign_extended is ever used
int s_immediate(void *_) { return 0; }
int s_immediate_sign_extended(void *_) {
  int data[4][2] = {{0x00002023, 0},
                    {0x7e002fa3, 2047},
                    {0xfe002fa3, -1},
                    {0x80002023, -2048}};

  for (int i = 0; i < 4; i++) {
    int imm = decode_s_immediate_sign_extended((instruction_t *)&data[i][0]);
    ASSERT_EQ(imm, data[i][1]);
  }

  return 0;
}

int b_immediate(void *_) { return -1; }
int b_immediate_sign_extended(void *_) { return -1; }
int u_immediate(void *_) { return -1; }
int u_immediate_sign_extended(void *_) { return -1; }
int j_immediate(void *_) { return -1; }
int j_immediate_sign_extended(void *_) { return -1; }

test_t immediate_tests[] = {
    TEST(i_immediate), TEST(i_immediate_sign_extended),
    TEST(s_immediate), TEST(s_immediate_sign_extended),
    TEST(b_immediate), TEST(b_immediate_sign_extended),
    TEST(u_immediate), TEST(u_immediate_sign_extended),
    TEST(j_immediate), TEST(j_immediate_sign_extended),
};
/* ------- IMMEDIATES TESTS END ------ */

/* -------- DISASSEMBLER TESTS ------- */
// Runs at the start of every test
void *disassemble_init() { return 0; };
// Cleans up after every test
void disassemble_cleanup(void *data) {};

// Tests get data from init passed as argument
int disassemble_addi(void *_) {
  char out[32];
  disassemble(0, 0x05010113, out, 32, NULL);
  ASSERT_STR_EQ("addi sp, sp, 80", out);

  disassemble(0, 0x03010413, out, 32, NULL);
  ASSERT_STR_EQ("addi s0, sp, 48", out);

  disassemble(0, 0x01C78793, out, 32, NULL);
  ASSERT_STR_EQ("addi a5, a5, 28", out);

  disassemble(0, 0x0FF7F713, out, 32, NULL);
  ASSERT_STR_EQ("andi a4, a5, 255", out);

  disassemble(0, 0x00078713, out, 32, NULL);
  ASSERT_STR_EQ("addi a4, a5, 0", out);

  return 0;
}

test_t disassemble_tests[] = {
    TEST(disassemble_addi),
};
/* ------ DISASSEMBLER TESTS END ----- */

/* --------- REGISTERS TESTS --------- */
void *registers_init() {
  srand(time(NULL));
  return calloc(1, sizeof(registers_t));
};
void registers_cleanup(void *data) { free(data); };

int registers_read(void *raw_data) {
  registers_t *registers = raw_data;

  // by default all reads should return 0;
  for (int i = 0; i < 31; i++) {
    int out;
    int ret = read_register(registers, i, &out);
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(out, 0)
  }

  int random = rand();
  registers->named.ra = random;
  int out;
  int ret = read_register(registers, 1, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.sp = random;
  ret = read_register(registers, 2, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.gp = random;
  ret = read_register(registers, 3, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.tp = random;
  ret = read_register(registers, 4, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.t0 = random;
  ret = read_register(registers, 5, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.t1 = random;
  ret = read_register(registers, 6, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.t2 = random;
  ret = read_register(registers, 7, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.s0_fp = random;
  ret = read_register(registers, 8, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.s1 = random;
  ret = read_register(registers, 9, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.a0 = random;
  ret = read_register(registers, 10, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.a1 = random;
  ret = read_register(registers, 11, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.a2 = random;
  ret = read_register(registers, 12, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.a3 = random;
  ret = read_register(registers, 13, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.a4 = random;
  ret = read_register(registers, 14, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.a5 = random;
  ret = read_register(registers, 15, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.a6 = random;
  ret = read_register(registers, 16, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.a7 = random;
  ret = read_register(registers, 17, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.s2 = random;
  ret = read_register(registers, 18, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.s3 = random;
  ret = read_register(registers, 19, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.s4 = random;
  ret = read_register(registers, 20, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.s5 = random;
  ret = read_register(registers, 21, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.s6 = random;
  ret = read_register(registers, 22, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.s7 = random;
  ret = read_register(registers, 23, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.s8 = random;
  ret = read_register(registers, 24, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.s9 = random;
  ret = read_register(registers, 25, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.s10 = random;
  ret = read_register(registers, 26, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.s11 = random;
  ret = read_register(registers, 27, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.t3 = random;
  ret = read_register(registers, 28, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.t4 = random;
  ret = read_register(registers, 29, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.t5 = random;
  ret = read_register(registers, 30, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.t6 = random;
  ret = read_register(registers, 31, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(out, random)

  random = rand();
  registers->named.pc = random;
  ret = read_register(registers, 32, &out);
  // reading from pc should return -1
  ASSERT_EQ(ret, -1);

  ret = read_register(registers, -1, &out);
  // reading from register <0 should return -1
  ASSERT_EQ(ret, -1);

  return 0;
}

int registers_write(void *raw_data) {
  registers_t *registers = raw_data;

  for (int i = 0; i < 31; i++) {
    int ret = write_register(registers, i, rand());
    ASSERT_EQ(ret, 0);
  }
  int random;

  // ZERO register should ignore writes
  int ret = write_register(registers, 0, rand());
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.zero, 0);

  random = rand();
  registers->named.ra = random;
  ret = write_register(registers, 1, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.ra, random)

  random = rand();
  registers->named.sp = random;
  ret = write_register(registers, 2, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.sp, random)

  random = rand();
  registers->named.gp = random;
  ret = write_register(registers, 3, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.gp, random)

  random = rand();
  registers->named.tp = random;
  ret = write_register(registers, 4, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.tp, random)

  random = rand();
  registers->named.t0 = random;
  ret = write_register(registers, 5, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.t0, random)

  random = rand();
  registers->named.t1 = random;
  ret = write_register(registers, 6, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.t1, random)

  random = rand();
  registers->named.t2 = random;
  ret = write_register(registers, 7, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.t2, random)

  random = rand();
  registers->named.s0_fp = random;
  ret = write_register(registers, 8, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.s0_fp, random)

  random = rand();
  registers->named.s1 = random;
  ret = write_register(registers, 9, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.s1, random)

  random = rand();
  registers->named.a0 = random;
  ret = write_register(registers, 10, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.a0, random)

  random = rand();
  registers->named.a1 = random;
  ret = write_register(registers, 11, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.a1, random)

  random = rand();
  registers->named.a2 = random;
  ret = write_register(registers, 12, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.a2, random)

  random = rand();
  registers->named.a3 = random;
  ret = write_register(registers, 13, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.a3, random)

  random = rand();
  registers->named.a4 = random;
  ret = write_register(registers, 14, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.a4, random)

  random = rand();
  registers->named.a5 = random;
  ret = write_register(registers, 15, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.a5, random)

  random = rand();
  registers->named.a6 = random;
  ret = write_register(registers, 16, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.a6, random)

  random = rand();
  registers->named.a7 = random;
  ret = write_register(registers, 17, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.a7, random)

  random = rand();
  registers->named.s2 = random;
  ret = write_register(registers, 18, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.s2, random)

  random = rand();
  registers->named.s3 = random;
  ret = write_register(registers, 19, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.s3, random)

  random = rand();
  registers->named.s4 = random;
  ret = write_register(registers, 20, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.s4, random)

  random = rand();
  registers->named.s5 = random;
  ret = write_register(registers, 21, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.s5, random)

  random = rand();
  registers->named.s6 = random;
  ret = write_register(registers, 22, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.s6, random)

  random = rand();
  registers->named.s7 = random;
  ret = write_register(registers, 23, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.s7, random)

  random = rand();
  registers->named.s8 = random;
  ret = write_register(registers, 24, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.s8, random)

  random = rand();
  registers->named.s9 = random;
  ret = write_register(registers, 25, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.s9, random)

  random = rand();
  registers->named.s10 = random;
  ret = write_register(registers, 26, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.s10, random)

  random = rand();
  registers->named.s11 = random;
  ret = write_register(registers, 27, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.s11, random)

  random = rand();
  registers->named.t3 = random;
  ret = write_register(registers, 28, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.t3, random)

  random = rand();
  registers->named.t4 = random;
  ret = write_register(registers, 29, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.t4, random)

  random = rand();
  registers->named.t5 = random;
  ret = write_register(registers, 30, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.t5, random)

  random = rand();
  registers->named.t6 = random;
  ret = write_register(registers, 31, random);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(registers->named.t6, random)

  registers->named.pc = random;
  ret = write_register(registers, 32, rand());
  // writing to pc with unnamed registers should fail
  ASSERT_EQ(ret, -1);

  ret = write_register(registers, -1, rand());
  // writing to register <0 should return -1
  ASSERT_EQ(ret, -1);

  return 0;
}

int registers_read_unsigned(void *raw_data) {
  registers_t *registers = raw_data;

  unsigned int value = 3495345;
  registers->named.a0 = value;
  int out;
  int ret = read_register(registers, 10, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(value, out);

  value = 0;
  registers->named.a1 = value;
  ret = read_register(registers, 11, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(value, out);

  // Max value of unsigned integer
  value = 1 << 31;
  registers->named.a1 = value;
  ret = read_register(registers, 11, &out);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(value, out);

  return 0;
}

test_t registers_tests[] = {
    TEST(registers_read),
    TEST(registers_write),
    TEST(registers_read_unsigned),
};
/* ------- REGISTERS TESTS END ------- */

/* ---------- SIMULATE TESTS --------- */
typedef struct simulate_test_data {
  registers_t registers;
  struct memory *memory;
} simulate_test_data_t;

void *simulate_init() {
  simulate_test_data_t *data = calloc(1, sizeof(simulate_test_data_t));

  data->memory = memory_create();

  return data;
};
void simulate_cleanup(void *data) {
  memory_delete(((simulate_test_data_t *)data)->memory);
  free(data);
};

int simulate_addi(void *raw_data) {
  simulate_test_data_t *data = raw_data;
  registers_t *registers = &data->registers;

  // addi a5, a5, -4
  instruction_t op = (instruction_t)0xFFC78793;

  // Simulate instruction
  int ret = simulate_single(data->memory, registers, NULL, op);
  ASSERT_EQ(ret, 0);

  ASSERT_EQ(registers->named.a5, -4);

  // Check that no other registers where affected
  registers_t *cmp_registers = calloc(1, sizeof(registers_t));
  cmp_registers->named.a5 = -4;
  ASSERT_MEM_EQ(&data->registers, cmp_registers, sizeof(registers_t));

  // addi a4,a5,0
  // mv a4,a5
  instruction_t mv = (instruction_t)0x00078713;

  ret = simulate_single(data->memory, registers, NULL, mv);
  ASSERT_EQ(ret, 0);

  ASSERT_EQ(registers->named.a5, -4);
  ASSERT_EQ(registers->named.a4, -4);

  // Check that no other registers where affected
  cmp_registers->named.a4 = -4;
  ASSERT_MEM_EQ(&data->registers, cmp_registers, sizeof(registers_t));

  free(cmp_registers);

  return 0;
}

test_t simulate_tests[] = {
    TEST(simulate_addi),
};
/* -------- SIMULATE TESTS END ------- */

/* ---------- BRANCH PREDICTION TESTS --------- */

void *branch_prediction_init() {
  branch_prediction_data_t *data = calloc(1, sizeof(branch_prediction_data_t));

  init_BHT(data, 16);

  return data;
}

void branch_prediction_cleanup(void *raw_data) {
  branch_prediction_data_t *data = (branch_prediction_data_t *)raw_data;

  if (data->BranchHistoryTable)
    free(data->BranchHistoryTable);

  free(data);
};

int BHT_test(void *raw_data, int size) {
  branch_prediction_data_t *data = (branch_prediction_data_t *)raw_data;
  init_BHT(data, size);

  for (int i = 0; i < size; i++) {
    ASSERT_EQ(data->BranchHistoryTable[i], 1)
  }

  int offset = 4;

  return 0;
}

int BHT_test_256(void *data) { return BHT_test(data, 256); }

int BHT_test_1k(void *data) { // 1k
  return BHT_test(data, 1024);
}

int BHT_test_4k(void *data) { // 4k
  return BHT_test(data, 4096);
}

int BHT_test_16k(void *data) { // 16k
  return BHT_test(data, 16384);
}

// what does NT mean? longer descriptive names please
int branch_prediction_test_NT(void *data) {
  int pc = 0;
  int offset = 8;
  predictor_mode predictor = NT;

  int prediction = branch_prediction(data, pc, offset, predictor);

  ASSERT_NEQ(predictor, 0);

  ASSERT_EQ(prediction, pc + 4);

  return 0;
}

// same issue here
int branch_prediction_test_BTFNT(void *data) {
  int pc = 0;
  int offset = 8;
  predictor_mode predictor = BTFNT;

  int prediction = branch_prediction(data, pc, offset, predictor);

  ASSERT_NEQ(predictor, 0);

  ASSERT_EQ(prediction, pc + 4);

  offset = -1;

  ASSERT_EQ(prediction, pc + offset);

  return 0;
}

int branch_prediction_test_Bimodal(void *raw_data) {
  int pc = 0;
  int offset = 8;
  int bimodal_index;
  predictor_mode predictor = Bimodal;
  branch_prediction_data_t *data = (branch_prediction_data_t *)raw_data;

  init_BHT(data, 8);

  data->BranchHistoryTable[bimodal_index] = 1;

  int prediction = branch_prediction(data, pc, offset, predictor);

  ASSERT_NEQ(predictor, 0);

  ASSERT_EQ(data->BranchHistoryTable[bimodal_index], 2);

  return 0;
}

int branch_prediction_test_gShare(void *raw_data) {
  int pc = 0;
  int offset = 8;
  int gShare_index;
  branch_prediction_data_t *data = (branch_prediction_data_t *)raw_data;
  init_BHT(data, 8);
  data->GlobalHistoryRegister =
      ((data->GlobalHistoryRegister << 1) | (offset < 0)) & GHR_MASK;

  predictor_mode predictor = gShare;

  int prediction = branch_prediction(data, pc, offset, predictor);

  ASSERT_NEQ(predictor, 0);

  ASSERT_EQ(prediction, pc + 4);

  ASSERT_EQ(data->BranchHistoryTable[gShare_index], 2);

  prediction = branch_prediction(data, pc, offset, predictor);

  ASSERT_EQ(prediction, pc + offset);

  return 0;
}

test_t branch_prediction_tests[] = {
    TEST(BHT_test_256),
    TEST(BHT_test_1k),
    TEST(BHT_test_4k),
    TEST(BHT_test_16k),
    TEST(branch_prediction_test_NT),
    TEST(branch_prediction_test_BTFNT),
    TEST(branch_prediction_test_Bimodal),
    TEST(branch_prediction_test_gShare),
};
/* -------- BRANCH PREDICTION TESTS END ------- */

int main() {
  run_test("Immediates", immediate_init, immediate_cleanup, immediate_tests,
           sizeof(immediate_tests) / sizeof(test_t));

  run_test("Disassemble", disassemble_init, disassemble_cleanup,
           disassemble_tests, sizeof(disassemble_tests) / sizeof(test_t));

  run_test("Registers", registers_init, registers_cleanup, registers_tests,
           sizeof(registers_tests) / sizeof(test_t));

  run_test("Simulate", simulate_init, simulate_cleanup, simulate_tests,
           sizeof(simulate_tests) / sizeof(test_t));

  run_test("Branch Prediction", branch_prediction_init,
           branch_prediction_cleanup, branch_prediction_tests,
           sizeof(branch_prediction_tests) / sizeof(test_t));
}
