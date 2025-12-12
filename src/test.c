#include "test.h"
#include <stdio.h>

int run_test(const char *tests_name, void *(*init_tests)(),
             void (*cleanup_tests)(void *), test_t tests[], int tests_length) {

  int failed[tests_length] = {};
  int failed_len = 0;

  printf("Running test suite \x1b[0;33m%s\x1b[0m\n", tests_name);

  for (int i = 0; i < tests_length; i++) {
    test_t test = tests[i];
    fflush(stdout);

    // Initialize data
    void *data = init_tests();

    // Run test
    int ret = test.test(data);

    if (ret < 0) {
      printf("\x1b[0;31m\x1b[1mx\x1b[1m %s\x1b[0m\n", test.name);
      failed[failed_len++] = i;
    } else {
      printf("\x1b[0;32m✓\x1b[1;90m %s\x1b[0m\n", test.name);
    }

    // Cleanup data
    if (data != NULL && data != 0)
      cleanup_tests(data);
  }

  if (failed_len) {
    printf("\n\x1b[0;31m\x1b[1m--- Tests failed! ---\x1b[0m\n");
    for (int i = 0; i < failed_len; i++) {
      printf("\x1b[0;31mx \x1b[0;33m%s:\x1b[0m %s\n", tests_name,
             tests[failed[i]].name);
    }
    return -1;
  }

  return 0;
}
