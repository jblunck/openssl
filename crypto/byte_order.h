/*
 * Copyright (c) 2014 The OpenSSL Project.  All rights reserved.
 */

#ifndef HEADER_BYTE_ORDER_H
#define HEADER_BYTE_ORDER_H

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__MINGW32__)
typedef __int64 i64;
typedef unsigned __int64 u64;
#define U64(C) C##UI64
#elif defined(__arch64__)
typedef long i64;
typedef unsigned long u64;
#define U64(C) C##UL
#else
typedef long long i64;
typedef unsigned long long u64;
#define U64(C) C##ULL
#endif

typedef unsigned int u32;
typedef unsigned char u8;

/*
 * TODO: Platform specific configuration should eventually go somewhere else.
 */
#define STRICT_ALIGNMENT 1
#ifndef PEDANTIC
#if defined(__i386)	|| defined(__i386__)	|| \
    defined(__x86_64)	|| defined(__x86_64__)	|| \
    defined(_M_IX86)	|| defined(_M_AMD64)	|| defined(_M_X64) || \
    defined(__aarch64__)			|| \
    defined(__s390__)	|| defined(__s390x__)
# undef STRICT_ALIGNMENT
#endif
#endif

#if !defined(PEDANTIC) && !defined(OPENSSL_NO_ASM) && !defined(OPENSSL_NO_INLINE_ASM)
#if defined(__GNUC__) && __GNUC__>=2
# if defined(__x86_64) || defined(__x86_64__)
#  define BSWAP8(x) ({	u64 ret=(x);			\
			asm ("bswapq %0"		\
			: "+r"(ret));	ret;		})
#  define BSWAP4(x) ({	u32 ret=(x);			\
			asm ("bswapl %0"		\
			: "+r"(ret));	ret;		})
# elif (defined(__i386) || defined(__i386__)) && !defined(I386_ONLY)
#  define BSWAP8(x) ({	u32 lo=(u64)(x)>>32,hi=(x);	\
			asm ("bswapl %0; bswapl %1"	\
			: "+r"(hi),"+r"(lo));		\
			(u64)hi<<32|lo;			})
#  define BSWAP4(x) ({	u32 ret=(x);			\
			asm ("bswapl %0"		\
			: "+r"(ret));	ret;		})
# elif defined(__aarch64__)
#  define BSWAP8(x) ({	u64 ret;			\
			asm ("rev %0,%1"		\
			: "=r"(ret) : "r"(x)); ret;	})
#  define BSWAP4(x) ({	u32 ret;			\
			asm ("rev %w0,%w1"		\
			: "=r"(ret) : "r"(x)); ret;	})
# elif (defined(__arm__) || defined(__arm)) && !defined(STRICT_ALIGNMENT)
#  define BSWAP8(x) ({	u32 lo=(u64)(x)>>32,hi=(x);	\
			asm ("rev %0,%0; rev %1,%1"	\
			: "+r"(hi),"+r"(lo));		\
			(u64)hi<<32|lo;			})
#  define BSWAP4(x) ({	u32 ret;			\
			asm ("rev %0,%1"		\
			: "=r"(ret) : "r"((u32)(x)));	\
			ret;				})
# endif
#elif defined(_MSC_VER)
# if _MSC_VER>=1300
#  pragma intrinsic(_byteswap_uint64,_byteswap_ulong)
#  define BSWAP8(x)	_byteswap_uint64((u64)(x))
#  define BSWAP4(x)	_byteswap_ulong((u32)(x))
# elif defined(_M_IX86)
   __inline u32 _bswap4(u32 val) {
	_asm mov eax,val
	_asm bswap eax
   }
#  define BSWAP4(x)	_bswap4(x)
# endif
#endif
#endif

static inline u64 __bswap8(u64 in)
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


/*
 * Legacy macros GETU32() and PUTU32().
 */
#if defined(BSWAP4) && !defined(STRICT_ALIGNMENT)
#define GETU32(p)	BSWAP4(*(const u32 *)(p))
#define PUTU32(p,v)	*(u32 *)(p) = BSWAP4(v)
#else
#define GETU32(p)	((u32)(p)[0]<<24|(u32)(p)[1]<<16|(u32)(p)[2]<<8|(u32)(p)[3])
#define PUTU32(p,v)	((p)[0]=(u8)((v)>>24),(p)[1]=(u8)((v)>>16),(p)[2]=(u8)((v)>>8),(p)[3]=(u8)(v))
#endif


#ifdef __cplusplus
}
#endif

#endif /* HEADER_BYTE_ORDER_H */
