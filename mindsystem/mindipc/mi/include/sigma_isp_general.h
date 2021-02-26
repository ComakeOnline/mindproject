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


#ifndef MSTAR_ISP_GENERAL_H_
#define MSTAR_ISP_GENERAL_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef void* ISP_DBGBUF_HANDLE;

//#include <isp_types.h>
/// data type unsigned char, data length 1 byte
typedef unsigned char               MS_U8;                              // 1 byte
/// data type unsigned short, data length 2 byte
typedef unsigned short              MS_U16;                             // 2 bytes
/// data type unsigned int, data length 4 byte
typedef unsigned int                MS_U32;                             // 4 bytes
/// data type unsigned int, data length 8 byte
typedef unsigned long long          MS_U64;                             // 8 bytes
/// data type signed char, data length 1 byte
typedef signed char                 MS_S8;                              // 1 byte
/// data type signed short, data length 2 byte
typedef signed short                MS_S16;                             // 2 bytes
/// data type signed int, data length 4 byte
typedef signed int                  MS_S32;                             // 4 bytes
/// data type signed int, data length 8 byte
typedef signed long long            MS_S64;                             // 8 bytes
/// data type float, data length 4 byte
typedef float                       MS_FLOAT;                           // 4 bytes

typedef unsigned char               MI_U8;                              // 1 byte
typedef unsigned short              MI_U16;                             // 2 bytes
typedef unsigned int                MI_U32;                             // 4 bytes
typedef unsigned long long          MI_U64;                             // 8 bytes
typedef signed char                 MI_S8;                              // 1 byte
typedef signed short                MI_S16;                             // 2 bytes
typedef signed int                  MI_S32;                             // 4 bytes
typedef signed long long            MI_S64;                             // 8 bytes
/*
typedef enum __attribute__ ((aligned (4)))
{
    SS_FALSE = 0,
    SS_TRUE = !SS_FALSE,
    SS_BOOL_MAX
} MI_ISP_BOOL_e;

typedef enum __attribute__ ((aligned (4)))
{
    SS_OP_TYP_AUTO = 0,
    SS_OP_TYP_MANUAL = !SS_OP_TYP_AUTO,
    SS_OP_TYP_MODE_MAX
} MI_ISP_OP_TYPE_e;
*/
typedef enum __attribute__ ((aligned (4)))
{
    MS_FALSE = 0,
    MS_TRUE  = !MS_FALSE,
    MS_BOOL_MAX = 0x7FFFFFFF
} MS_CAM_BOOL;

#define FAIL                    (-1)
#define SUCCESS                 (0)

#if !defined(NULL)
#define NULL                    (0)
#endif

#if !defined(TRUE) && !defined(FALSE)
/// definition for TRUE
#define TRUE                        1
/// definition for FALSE
#define FALSE                       0
#endif

#define LIMIT(x, low, high)     (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

#ifdef sign
#undef sign
#endif
#define sign(a)         (((a) > 0) ? (1) : (-1))

//#define CAMERA_ISR_FREQ_SEC     (0)
//#define CAMERA_ISR_FREQ_NSEC    (10000000) //(1000*1000*1000) // 1sec

#define PACK_ALIGN_TAG(n)       __attribute__((packed, aligned(n)))
//#define LIMIT_BITS(x, bits)     ((x) > ((1<<(bits))-1) ? (((1<<(bits))-1)) : (x))
//typedef void *(*THREAD_P)(void *);

#define AE_WIN_MAX_WIDTH        (32)//(16)
#define AE_WIN_MAX_HEIGHT       (32)//(24)
#define AE_HIST_WIN_MAX_WIDTH   (128)
#define AE_HIST_WIN_MAX_HEIGHT  (90)
#define AE_HIST_BIN0            (40)
#define AE_HIST_BINX            (128)
#define AE_IR_HIST_BIN          (256)
#define AE_HIST_OUT_BIN         (256)

//#define HDR_STATS_LEN           (1024)

#define AWB_WIN_MAX_WIDTH       (128)
#define AWB_WIN_MAX_HEIGHT      (90)
#define AWB_BLOCK_MIN_VALID_X   (4)
#define AWB_BLOCK_MIN_VALID_Y   (2)

//#define AWB_LV_CT_TBL_NUM          18
//#define AWB_CT_TBL_NUM             10

typedef struct
{
    MS_U8 uAvgR;
    MS_U8 uAvgG;
    MS_U8 uAvgB;
    MS_U8 uAvgY;
} MS_AE_AVGS;

typedef struct
{
    MS_U8 uAvgR;
    MS_U8 uAvgG;
    MS_U8 uAvgB;
} MS_AWB_AVGS;

// AE functions

typedef struct
{
    MS_U16 Rgbir_stats[AE_IR_HIST_BIN];
} ISP_RGBIR_STATS;

typedef struct
{
    MS_U8  uAvgR;
    MS_U8  uAvgG;
    MS_U8  uAvgB;
    MS_U8  uAvgY;
} ISP_AE_AVGS;

typedef struct
{
    MS_U16 u2HistR[AE_HIST_BIN0];
    MS_U16 u2HistG[AE_HIST_BIN0];
    MS_U16 u2HistB[AE_HIST_BIN0];
} ISP_AE_HIST0;

typedef struct
{
    MS_U16 u2HistY[AE_HIST_BINX];
} ISP_AE_HISTX;

typedef struct
{
    ISP_AE_HIST0    Hist0;
    ISP_AE_HISTX    Hist1;
    ISP_AE_HISTX    Hist2;
    ISP_RGBIR_STATS HistIR;
    ISP_AE_AVGS     Avgs[AE_WIN_MAX_WIDTH * AE_WIN_MAX_HEIGHT];
    ISP_AE_AVGS     Avgs1[AE_WIN_MAX_WIDTH * AE_WIN_MAX_HEIGHT];
} PACK_ALIGN_TAG(16) ISP_AE_STATIS;

#if 1
// crop config, isp_crop_cfg
typedef struct {
    int x_start;
    int y_start;
    int width;
    int height;
} ISP_CROP_WIN, IspCropWin_t;
#endif

#if 1
typedef struct {
    MS_U16     u2BlkNum_x;
    MS_U16     u2BlkNum_y;
    MS_U16     u2BlkVaild_x;
    MS_U16     u2BlkVaild_y;
    MS_U16     u2BlkValidLowLux_x;
    MS_U16     u2BlkValidLowLux_y;
    MS_U8      AccEnhanceRatio;
} ISP_AWB_WIN, IspAwbWin_t;
#endif

typedef struct
{
    MS_U16  nRgain;     /** R  channel */
    MS_U16  nGrgain;    /** Gr channel */
    MS_U16  nGbgain;    /** Gb channel */
    MS_U16  nBgain;     /** B  channel */
} IspAwbGain_t;

typedef struct {
    MS_U8  uAvgR;
    MS_U8  uAvgG;
    MS_U8  uAvgB;
} PACK_ALIGN_TAG(1) ISP_AWB_AVGS, IspAwbBlkAvg_t;

typedef struct {
    IspAwbBlkAvg_t tRow[AWB_WIN_MAX_WIDTH];
} PACK_ALIGN_TAG(16) IspAwbAvgRow_t;

typedef struct {
    ISP_AWB_AVGS Avgs[AWB_WIN_MAX_WIDTH * AWB_WIN_MAX_HEIGHT];
    //IspAwbAvgRow_t Avgs[AWB_WIN_MAX_HEIGHT];
} PACK_ALIGN_TAG(16) ISP_AWB_STATIS,IspAwbStatis_t;


#if 1 //AF
typedef struct
{
    MS_U8 high_iir[5*16];
    MS_U8 low_iir[5*16];
    MS_U8 luma_iir[4*16];
    MS_U8 sobel_v[5*16];
    MS_U8 sobel_h[5*16];
    MS_U8 ysat[3*16];
} ISP_AF_STATS;

typedef enum
{
    IQ_AF_ROI_MODE_NORMAL,
    IQ_AF_ROI_MODE_MATRIX
} ISP_AF_ROI_MODE;

typedef struct
{
    MS_S32 x_start;
    MS_S32 y_start;
    MS_S32 x_end;
    MS_S32 y_end;
} ISP_AF_WIN;

typedef struct {
    MS_U16 iir1_a0;
    MS_U16 iir1_a1;
    MS_U16 iir1_a2;
    MS_U16 iir1_b1;
    MS_U16 iir1_b2;
    MS_U16 iir1_1st_low_clip;
    MS_U16 iir1_1st_high_clip;
    MS_U16 iir1_2nd_low_clip;
    MS_U16 iir1_2nd_high_clip;

    MS_U16 iir2_a0;
    MS_U16 iir2_a1;
    MS_U16 iir2_a2;
    MS_U16 iir2_b1;
    MS_U16 iir2_b2;
    MS_U16 iir2_1st_low_clip;
    MS_U16 iir2_1st_high_clip;
    MS_U16 iir2_2nd_low_clip;
    MS_U16 iir2_2nd_high_clip;

    MS_U16 iir1_e1_en;
    MS_U16 iir1_e1_a0;
    MS_U16 iir1_e1_a1;
    MS_U16 iir1_e1_a2;
    MS_U16 iir1_e1_b1;
    MS_U16 iir1_e1_b2;
    MS_U16 iir1_e2_en;
    MS_U16 iir1_e2_a0;
    MS_U16 iir1_e2_a1;
    MS_U16 iir1_e2_a2;
    MS_U16 iir1_e2_b1;
    MS_U16 iir1_e2_b2;

    MS_U16 iir2_e1_en;
    MS_U16 iir2_e1_a0;
    MS_U16 iir2_e1_a1;
    MS_U16 iir2_e1_a2;
    MS_U16 iir2_e1_b1;
    MS_U16 iir2_e1_b2;
    MS_U16 iir2_e2_en;
    MS_U16 iir2_e2_a0;
    MS_U16 iir2_e2_a1;
    MS_U16 iir2_e2_a2;
    MS_U16 iir2_e2_b1;
    MS_U16 iir2_e2_b2;
} ISP_AF_FILTER_ATTR;

typedef struct
{
    MS_U8 bSobelYSatEn;
    MS_U16  u16SobelYThd;

    MS_U8 bIIRSquareAccEn;
    MS_U8 bSobelSquareAccEn;

    MS_U16  u16IIR1Thd;
    MS_U16  u16IIR2Thd;
    MS_U16  u16SobelHThd;
    MS_U16  u16SobelVThd;
    MS_U8   u8AFTblX[12];
    MS_U16  u16AFTblY[13];
} ISP_AF_FILTERSQ;

#endif

#ifdef __cplusplus
}
#endif
#endif /* MSTAR_ISP_GENERAL_H_ */
