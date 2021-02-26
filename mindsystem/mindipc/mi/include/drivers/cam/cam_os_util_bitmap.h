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


#ifndef __CAM_OS_UTIL_BITMAP_H__
#define __CAM_OS_UTIL_BITMAP_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CAM_OS_BIT_MASK(nr)		(1UL << ((nr) % CAM_OS_BITS_PER_LONG))
#define CAM_OS_BIT_WORD(nr)		((nr) / CAM_OS_BITS_PER_LONG)
#define CAM_OS_DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#define CAM_OS_BITS_TO_LONGS(nr)	CAM_OS_DIV_ROUND_UP(nr, CAM_OS_BITS_PER_LONG)

#define CAM_OS_DECLARE_BITMAP(name,bits) \
	unsigned long name[CAM_OS_BITS_TO_LONGS(bits)]

#define CAM_OS_BITMAP_CLEAR(name)	do { \
	memset((name), 0, sizeof(name)); \
} while (0)

unsigned long _CamOsFindFirstZeroBit(unsigned long *pAddr, unsigned long nSize,
				                     unsigned long nOffset);
#define CAM_OS_FIND_FIRST_ZERO_BIT(p,sz) _CamOsFindFirstZeroBit(p,sz,0)
#define CAM_OS_FIND_NEXT_ZERO_BIT(p,sz,of) _CamOsFindFirstZeroBit(p,sz,of)

static inline int CAM_OS_FFS(unsigned long x)
{
	return CAM_OS_FLS(x & -x);
}

static inline unsigned long _CAM_OS_FFS(unsigned long x)
{
	return CAM_OS_FFS(x) - 1;
}

#define CAM_OS_FFZ(x) _CAM_OS_FFS( ~(x) )

/* WARNING: bitmap operation is non atomic */
static inline void CAM_OS_SET_BIT(s32 nr, volatile unsigned long *addr)
{
	unsigned long mask = CAM_OS_BIT_MASK(nr);
	volatile unsigned long *p = (addr) + CAM_OS_BIT_WORD(nr);

	*p  |= mask;
}

static inline void CAM_OS_CLEAR_BIT(s32 nr, volatile unsigned long *addr)
{
	unsigned long mask = CAM_OS_BIT_MASK(nr);
	volatile unsigned long *p = (addr) + CAM_OS_BIT_WORD(nr);

	*p &= ~mask;
}

static inline void CAM_OS_CHANGE_BIT(s32 nr, volatile unsigned long *addr)
{
	unsigned long mask = CAM_OS_BIT_MASK(nr);
	volatile unsigned long *p = (addr) + CAM_OS_BIT_WORD(nr);

	*p ^= mask;
}

static inline s32 CAM_OS_TEST_AND_SET_BIT(s32 nr, volatile unsigned long *addr)
{
	unsigned long mask = CAM_OS_BIT_MASK(nr);
	volatile unsigned long *p = (addr) + CAM_OS_BIT_WORD(nr);
	volatile unsigned long old = *p;

	*p = old | mask;
	return (old & mask) != 0;
}

static inline s32 CAM_OS_TEST_AND_CLEAR_BIT(s32 nr, volatile unsigned long *addr)
{
	unsigned long mask = CAM_OS_BIT_MASK(nr);
	volatile unsigned long *p = (addr) + CAM_OS_BIT_WORD(nr);
	volatile unsigned long old = *p;

	*p = old & ~mask;
	return (old & mask) != 0;
}

static inline s32 CAM_OS_TEST_AND_CHANGE_BIT(s32 nr,
					    volatile unsigned long *addr)
{
	unsigned long mask = CAM_OS_BIT_MASK(nr);
	volatile unsigned long *p = (addr) + CAM_OS_BIT_WORD(nr);
	volatile unsigned long old = *p;

	*p = old ^ mask;
	return (old & mask) != 0;
}

static inline s32 CAM_OS_TEST_BIT(s32 nr, const volatile unsigned long *addr)
{
	return 1UL & (addr[CAM_OS_BIT_WORD(nr)] >> (nr & (CAM_OS_BITS_PER_LONG-1)));
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__CAM_OS_UTIL_BITMAP_H__
