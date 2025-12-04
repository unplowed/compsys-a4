#include "disassemble.h"
#include "test.h"

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

  return 0;
}

test_t disassemble_tests[] = {
  TEST(disassemble_addi),
};

/* ------ DISASSEMBLER TESTS END ----- */

int main() {
  run_test("Disassemble", disassemble_init, disassemble_cleanup,
           disassemble_tests, sizeof(disassemble_tests) / sizeof(test_t));
}
