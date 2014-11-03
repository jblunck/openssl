/*
 * Copyright (c) 2014 The OpenSSL Project.  All rights reserved.
 */

#ifndef HEADER_BYTE_ORDER_H
#define HEADER_BYTE_ORDER_H

#ifdef __cplusplus
extern "C" {
#endif

inline u64 __bswap8(u64 in)
{
  union {
    u64 __u64;
    u32 __u32[2];
  } x;
  u32 tmp;

  x.__u64 = in;
  tmp = BSWAP4(x.__u32[0]);
  x.__u32[0] = BSWAP4(x.__u32[1]);
  x.__u32[1] = tmp;
  return x.__u64;
}
#if !defined(BSWAP8)
#define BSWAP8(x) __bswap8((x))
#endif /* !defined(BSWAP8) */

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define HTOBE32(x) BSWAP4((x))
#define BE32TOH(x) BSWAP4((x))
#define HTOBE64(x) BSWAP8((x))
#define BE64TOH(x) BSWAP8((x))
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define HTOBE32(x) (x)
#define BE32TOH(x) (x)
#define HTOBE64(x) (x)
#define BE64TOH(x) (x)
#else
#error "Could not determine byte order!"
#endif

#ifdef __cplusplus
}
#endif

#endif /* HEADER_BYTE_ORDER_H */
