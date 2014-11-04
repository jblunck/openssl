#include "../crypto/modes/modes_lcl.h"
#include "../crypto/byte_order.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef union {
  u32 __u32;
  u8  __u8[4];
} u32_u8_t;

const static u8 input_u32[] = { 0x11, 0x22, 0x33, 0x44 };
const static u8 expect_u32[] = { 0x44, 0x33, 0x22, 0x11 };

static int test_BSWAP4()
{
  u32_u8_t x, res;
  memcpy(x.__u8, input_u32, sizeof(x.__u8));
  res.__u32 = BSWAP4(x.__u32);
  return memcmp(res.__u8, expect_u32, sizeof(res.__u8)) != 0;
}

typedef union {
  u64 __u64;
  u32 __u32[2];
  u8  __u8[8];
} u64_u8_t;

const static u8 input_u64[] =
  { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };
const static u8 expect_u64[] =
  { 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };

static int test_bswap8()
{
  u64_u8_t x, res;
  memcpy(x.__u8, input_u64, sizeof(x.__u8));
  res.__u64 = __bswap8(x.__u64);
  return memcmp(res.__u8, expect_u64, sizeof(res.__u8)) != 0;
}

static int test_BSWAP8()
{
  u64_u8_t x, res;
  memcpy(x.__u8, input_u64, sizeof(x.__u8));
  res.__u64 = BSWAP8(x.__u64);
  return memcmp(res.__u8, expect_u64, sizeof(res.__u8)) != 0;
}

int
main(int argc, char *argv[])
{
  int num_all = 0;
  int num_failed = 0;

  fprintf(stdout, "Testing byte swap operations...\n");

  num_failed += test_BSWAP4();
  num_all++;

  num_failed += test_bswap8();
  num_all++;
  num_failed += test_BSWAP8();
  num_all++;

  if (num_failed != 0) {
    fprintf(stdout, "%d tests failed!\n", num_failed);
    return EXIT_FAILURE;
  }

  fprintf(stdout, "ok (ran %d tests)\n", num_all);
  return EXIT_SUCCESS;
}
