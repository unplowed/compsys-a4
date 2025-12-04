#pragma once
#include <string.h>
#include <stdio.h>

typedef struct test {
  char *name;
  int (*test)(void *);
} test_t;

int run_test(const char *tests_name, void *(*init_tests)(),
             void (*cleanup_tests)(void *), test_t[], int tests_length);

#define TEST(function) {#function, function}

#define ASSERT(cond, msg)                                                      \
  if (!cond) {                                                                 \
    printf("Assert failed! %s wasnt 1: %s", #cond, msg);                       \
    return -1;                                                                 \
  }

#define ASSERT_STR_EQ(str1, str2)                                              \
  if (strcmp(str1, str2)) {                                                   \
    printf("Assert failed! '%s' != '%s'", str1, str2);                             \
    return -1;                                                                 \
  }

#define ASSERT_EQ(a, b)                                                        \
  if (a != b) {                                                                \
    printf("Assert failed! %s == %s", #a, #b);                                 \
    return -1;                                                                 \
  }

#define ASSERT_NEQ(a, b)                                                       \
  if (a == b) {                                                                \
    printf("Assert failed! %s != %s", #a, #b);                                 \
    return -1;                                                                 \
  }

#define ASSERT_LT(a, b, msg)                                                   \
  if (!(a < b)) {                                                              \
    printf("Assert failed! %s < %s", #a, #b);                                  \
    return -1;                                                                 \
  }

#define ASSERT_GR(a, b, msg)                                                   \
  if (!(a > b)) {                                                              \
    printf("Assert failed! %s < %s", #a, #b);                                  \
    return -1;                                                                 \
  }

#define ASSERT_LTE(a, b, msg)                                                  \
  if (!(a <= b)) {                                                             \
    printf("Assert failed! %s < %s", #a, #b);                                  \
    return -1;                                                                 \
  }

#define ASSERT_GRE(a, b, msg)                                                  \
  if (!(a >= b)) {                                                             \
    printf("Assert failed! %s < %s", #a, #b);                                  \
    return -1;                                                                 \
  }
