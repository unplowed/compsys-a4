#include "test.h"
#include <stdio.h>

int run_test(const char* tests_name, void *(*init_tests)(), void (*cleanup_tests)(void *),
             test_t tests[], int tests_length) {

  int failed[tests_length] = {};
  int failed_len = 0;

  printf("Running test suite \x1b[0;33m%s\x1b[0m\n", tests_name);

  for (int i = 0; i < tests_length; i++) {
    test_t test = tests[i];
    printf("Running test\t\x1b[0;33m%s\x1b[0m: ", test.name);
    fflush(stdout);

    // Initialize data
    void *data = init_tests();

    // Run test
    int ret = test.test(data);

    if (ret < 0) {
      printf("\t\x1b[0;31mFailed!\x1b[0m\n");
      failed[failed_len++] = i;
    } else {
      printf("\t\x1b[0;32mSuccess!\x1b[0m\n");
    }

    // Cleanup data
    if (data != NULL && data != 0)
      cleanup_tests(data);
  }

  if (failed_len) {
    printf("\x1b[0;31mTests failed!\x1b[0m\nTests ");
    for (int i = 0; i < failed_len; i++) {
      printf("%i, ", failed[i]);
    }
    printf("failed!\n");
    return -1;
  }

  return 0;
}
