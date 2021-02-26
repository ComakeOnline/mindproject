/* SigmaStar trade secret */
/* Copyright (c) [2019~2020] SigmaStar Technology.
All rights reserved.

Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
SigmaStar and be kept in strict confidence
(SigmaStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of SigmaStar Confidential
Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
*/


#ifndef __CAM_OS_UTIL_H__
#define __CAM_OS_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define FORCE_INLINE __attribute__((always_inline)) inline

#ifndef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#ifdef size_t
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((int) &((TYPE *)0)->MEMBER)
#endif
#endif
#endif

#define CAM_OS_CONTAINER_OF(ptr, type, member) ({              \
        void *__mptr = (void *)(ptr);                   \
        ((type *)(__mptr - offsetof(type, member))); })

#ifndef likely
#define CAM_OS_LIKELY(x) __builtin_expect(!!(x), 1)
#else
#define CAM_OS_LIKELY(x) likely(x)
#endif

#ifndef unlikely
#define CAM_OS_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define CAM_OS_UNLIKELY(x) unlikely(x)
#endif

static FORCE_INLINE s32 CAM_OS_FLS(s32 x)
{
	int r = 32;

	if (!x)
		return 0;
	if (!(x & 0xffff0000u)) {
		x <<= 16;
		r -= 16;
	}
	if (!(x & 0xff000000u)) {
		x <<= 8;
		r -= 8;
	}
	if (!(x & 0xf0000000u)) {
		x <<= 4;
		r -= 4;
	}
	if (!(x & 0xc0000000u)) {
		x <<= 2;
		r -= 2;
	}
	if (!(x & 0x80000000u)) {
		x <<= 1;
		r -= 1;
	}
	return r;
}

#if CAM_OS_BITS_PER_LONG == 32
static FORCE_INLINE s32 CAM_OS_FLS64(u64 x)
{
	u32 h = x >> 32;
	if (h)
		return CAM_OS_FLS(h) + 32;
	return CAM_OS_FLS(x);
}
#elif CAM_OS_BITS_PER_LONG == 64
static FORCE_INLINE s32 _CAM_OS_FLS(u64 word)
{
	s32 num = CAM_OS_BITS_PER_LONG - 1;

//#if CAM_OS_BITS_PER_LONG == 64
	if (!(word & (~0ul << 32))) {
		num -= 32;
		word <<= 32;
	}
//#endif
	if (!(word & (~0ul << (CAM_OS_BITS_PER_LONG-16)))) {
		num -= 16;
		word <<= 16;
	}
	if (!(word & (~0ul << (CAM_OS_BITS_PER_LONG-8)))) {
		num -= 8;
		word <<= 8;
	}
	if (!(word & (~0ul << (CAM_OS_BITS_PER_LONG-4)))) {
		num -= 4;
		word <<= 4;
	}
	if (!(word & (~0ul << (CAM_OS_BITS_PER_LONG-2)))) {
		num -= 2;
		word <<= 2;
	}
	if (!(word & (~0ul << (CAM_OS_BITS_PER_LONG-1))))
		num -= 1;
	return num;
}

static FORCE_INLINE s32 CAM_OS_FLS64(u64 x)
{
	if (x == 0)
		return 0;
	return _CAM_OS_FLS(x) + 1;
}
#else
#error CAM_OS_BITS_PER_LONG not 32 or 64
#endif

#define CAM_OS_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#ifdef __cplusplus
}
#endif /* __cplusplus */

#define CAM_OS_MIN(a,b) (((a)<(b))?(a):(b))
#define CAM_OS_MAX(a,b) (((a)>(b))?(a):(b))

#endif //__CAM_OS_UTIL_H__
