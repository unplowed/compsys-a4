#pragma once
#include <stdio.h>
#include <string.h>

typedef struct test {
  char *name;
  int (*test)(void *);
} test_t;

int run_test(const char *tests_name, void *(*init_tests)(),
             void (*cleanup_tests)(void *), test_t[], int tests_length);

inline void print_bytes(void *data, size_t size) {
  for (int i = 0; i < size; i++) {
    if (i != 0 && i % 4 == 0)
      printf("\n");
    printf("%02X ", ((char *)data)[i]);
  }
}

#define TEST(function) {#function, function}

#define ASSERT(cond, msg)                                                      \
  if (!(cond)) {                                                               \
    printf("Assert failed! condition '%s': %s\n", #cond, msg);                 \
    return -1;                                                                 \
  }

#define ASSERT_STR_EQ(str1, str2)                                              \
  if (strcmp(str1, str2)) {                                                    \
    printf("Assert failed! '%s' != '%s'\n", str1, str2);                       \
    return -1;                                                                 \
  }

#define ASSERT_MEM_EQ(addr1, addr2, size)                                      \
  if (memcmp(addr1, addr2, size) != 0) {                                       \
    printf("Assert failed! Memory at locations %p, %p was different!\n",       \
           addr1, addr2);                                                      \
    printf("%p: \n", addr1);                                                   \
    print_bytes(addr1, size);                                                  \
    printf("\n\n%p: \n", addr2);                                               \
    print_bytes(addr2, size);                                                  \
    printf("\n\n");                                                            \
    return -1;                                                                 \
  }

#define ASSERT_EQ(a, b)                                                        \
  if (a != b) {                                                                \
    printf("Assert failed! %s == %s, %i == %i\n", #a, #b, a, b);               \
    return -1;                                                                 \
  }

#define ASSERT_NEQ(a, b)                                                       \
  if (a == b) {                                                                \
    printf("Assert failed! %s != %s, %i != %i\n", #a, #b, a, b);               \
    return -1;                                                                 \
  }

#define ASSERT_LT(a, b, msg)                                                   \
  if (!(a < b)) {                                                              \
    printf("Assert failed! %s < %s, %i < %i\n", #a, #b, a, b);                 \
    return -1;                                                                 \
  }

#define ASSERT_GR(a, b, msg)                                                   \
  if (!(a > b)) {                                                              \
    printf("Assert failed! %s < %s, %i < %i\n", #a, #b, a, b);                 \
    return -1;                                                                 \
  }

#define ASSERT_LTE(a, b, msg)                                                  \
  if (!(a <= b)) {                                                             \
    printf("Assert failed! %s < %s, %i < %i\n", #a, #b, a, b);                 \
    return -1;                                                                 \
  }

#define ASSERT_GRE(a, b, msg)                                                  \
  if (!(a >= b)) {                                                             \
    printf("Assert failed! %s < %s, %i < %i\n", #a, #b, a, b);                 \
    return -1;                                                                 \
  }
