/* Copyright (c) 2018-2019 Sigmastar Technology Corp.
 All rights reserved.
  
  Unless otherwise stipulated in writing, any and all information contained
 herein regardless in any format shall remain the sole proprietary of
 Sigmastar Technology Corp. and be kept in strict confidence
 (Sigmastar Confidential Information) by the recipient.
 Any unauthorized act including without limitation unauthorized disclosure,
 copying, use, reproduction, sale, distribution, modification, disassembling,
 reverse engineering and compiling of the contents of Sigmastar Confidential
 Information is unlawful and strictly prohibited. Sigmastar hereby reserves the
 rights to any and all damages, losses, costs and expenses resulting therefrom.
*/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include "mstarFb.h"

#include "minigui_entry.hpp"

/* get Color format fo framebuffer */
MI_FB_ColorFmt_e getFBColorFmt(struct fb_var_screeninfo *var)
{
    MI_FB_ColorFmt_e fmt = E_MI_FB_COLOR_FMT_INVALID;

    switch (var->bits_per_pixel)
    {
        case 16:
        {
            if (var->transp.length == 0) //RGB565
            {
                fmt = E_MI_FB_COLOR_FMT_RGB565;
            }
            else if (var->transp.length ==1) //ARGB 1555
            {
                fmt = E_MI_FB_COLOR_FMT_ARGB1555;
            }
            else if (var->transp.length == 4) //ARGB4444
            {
                fmt = E_MI_FB_COLOR_FMT_ARGB4444;
            }
        }
        break;
        //ARGB8888
        case 32:
        {
            fmt = E_MI_FB_COLOR_FMT_ARGB8888;
        }
        break;
        default:
            fmt = E_MI_FB_COLOR_FMT_INVALID;
            break;
    }
    return fmt;
}

/* Convert RGB chanel from ARGB1555 to ARGB8888 */
static MI_U32 convert1555To8888(MI_U8 u8Red, MI_U8 u8Green, MI_U8 u8Blue)
{
    MI_U32 fullclr = 0;
    MI_U16 clr = (0x1 << 15) | ((u8Red & 0x1f) << 10)
        | ((u8Green&0x1f) << 5) | (u8Blue & 0x1f);
    //amplify
    fullclr = ((clr>>10)&0x1f)<<3; //R Component
    fullclr |= ((clr>>12)&0x7);
    fullclr = fullclr << 8;
    fullclr |= ((clr>>5)&0x1f)<<3;      // G Component
    fullclr |= ((clr>>7)&0x7);
    fullclr = fullclr << 8;
    fullclr |= (clr&0x1f)<<3;           // B
    fullclr |= ((clr>>2)&0x7);
    return fullclr;
}

/* Convert RGB565 chanel from RGB565 to ARGB8888 */
static MI_U32 convert565To8888(MI_U8 u8Red, MI_U8 u8Green, MI_U8 u8Blue)
{
    MI_U32 fullclr = 0;
    MI_U16 clr = ((u8Red & 0x1f) << 11) | ((u8Green & 0x3f)<<5)
        | (u8Blue & 0x1f);
    //amplify to ARGB8888
    fullclr = ((clr >> 11)&0x1f) << 3; //R
    fullclr |= ((clr >> 13)&0x7);
    fullclr = fullclr << 8;
    fullclr |= ((clr >> 5)&0x3f) << 2; //G
    fullclr |= ((clr >> 9) &0x3);
    fullclr = fullclr << 8;
    fullclr |= (clr&0x1f) << 3; //B
    fullclr |= ((clr>>2)&0x7);
    return fullclr;
}

/* Convert ARGB4444 chanel from ARGB4444 to ARGB8888 */
static MI_U32 convert4444To8888(MI_U8 u8Red, MI_U8 u8Green, MI_U8 u8Blue)
{
    MI_U32 fullclr = 0;
    MI_U16 clr = ((u8Red & 0xf) << 8) | ((u8Green & 0xf) << 4)
        |((u8Blue & 0xf));
    //amplify to ARGB8888
    fullclr = ((clr >> 8) & 0xf) << 4; //R
    fullclr |= ((clr >> 8) & 0xf);
    fullclr = fullclr << 8;
    fullclr |= ((clr >> 4) & 0xf) << 4; //G
    fullclr |= ((clr >> 4) & 0xf);
    fullclr = fullclr << 8;
    fullclr |= (clr & 0xf) << 4; //B
    fullclr |= (clr & 0xf);
    return fullclr;
}

int SetFbClrKey(unsigned int fullClrKey)
{
	int fd = 0;
	long int screensize = 0;
	char *frameBuffer = NULL;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
    MI_FB_ColorFmt_e efmt = E_MI_FB_COLOR_FMT_INVALID;
	
    /* Open the file for reading and writing */
    fd = open("/dev/fb0", O_RDWR);
    if(-1 == fd)
    {
        perror("Error: cannot open framebuffer device");
        return -1;
    }
	
    //get fb_fix_screeninfo
    if(-1 == ioctl(fd, FBIOGET_FSCREENINFO, &finfo))
    {
        perror("Error reading fixed information");
        return -1;
    }

    //get fb_var_screeninfo
    if(-1 == ioctl(fd, FBIOGET_VSCREENINFO, &vinfo))
    {
        perror("Error reading variable information");
        return -1;
    }

    /* Figure out the size of the screen in bytes */
    screensize = finfo.smem_len;

    /* Map the device to memory */
    frameBuffer = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(MAP_FAILED == frameBuffer)
    {
        perror ("Error: Failed to map framebuffer device to memory");
        return -1;
    }

	//unmap buffer
    munmap(frameBuffer, screensize);

    //Pandisplay
    if(-1 == ioctl(fd, FBIOPAN_DISPLAY, &vinfo))
	{
        perror("Error: failed to FBIOPAN_DISPLAY");
        return -1;
    }
	
    //test FBIOSET_COLORKEY
    MI_FB_ColorKey_t colorKeyInfo;
    colorKeyInfo.bKeyEnable = TRUE;
    colorKeyInfo.u8Red = fullClrKey & 0x0000ff;
    colorKeyInfo.u8Green = (fullClrKey >> 8) & 0x0000ff;
    colorKeyInfo.u8Blue = (fullClrKey >> 16) & 0x0000ff;
    efmt = getFBColorFmt(&vinfo);
	
    /*	Convert colorkey value from RGB88 to current format
    	In order to avoid tolerance result from different format */
    switch (efmt)
    {
        case E_MI_FB_COLOR_FMT_RGB565:
        {
        	UI_print("E_MI_FB_COLOR_FMT_RGB565.");
            colorKeyInfo.u8Red =  (colorKeyInfo.u8Red >> 3)&(0x1f);
            colorKeyInfo.u8Green = (colorKeyInfo.u8Green >> 2)&(0x3f);
            colorKeyInfo.u8Blue = (colorKeyInfo.u8Blue >> 3)&(0x1f);
        }
        break;
        case E_MI_FB_COLOR_FMT_ARGB4444:
        {
	        UI_print("E_MI_FB_COLOR_FMT_ARGB4444.");
            colorKeyInfo.u8Red = (colorKeyInfo.u8Red >> 4)&(0xf);
            colorKeyInfo.u8Green = (colorKeyInfo.u8Green >> 4)&(0xf);
            colorKeyInfo.u8Blue = (colorKeyInfo.u8Blue >> 4)&(0xf);
        }
        break;
        case E_MI_FB_COLOR_FMT_ARGB1555:
        {
	        UI_print("E_MI_FB_COLOR_FMT_ARGB1555.");
            colorKeyInfo.u8Red =  (colorKeyInfo.u8Red >> 3)&(0x1f);
            colorKeyInfo.u8Green = (colorKeyInfo.u8Green >> 3)&(0x1f);
            colorKeyInfo.u8Blue = (colorKeyInfo.u8Blue >> 3)&(0x1f);
        }
        break;
        default:
			UI_print("default.");
            printf("convertColorKeyByFmt colorfmt is %d\n",efmt);
            break;
    }
	
    /*	Convert color key expressed as current format to ARGB888
     	Because GOP hw can only support ARGB888 format	*/
     switch (efmt)
     {
        case E_MI_FB_COLOR_FMT_RGB565:
        {
	        UI_print("E_MI_FB_COLOR_FMT_RGB565.");
            fullClrKey = convert565To8888(colorKeyInfo.u8Red, colorKeyInfo.u8Green, colorKeyInfo.u8Blue);
        }
        break;
        case E_MI_FB_COLOR_FMT_ARGB4444:
        {
	        UI_print("E_MI_FB_COLOR_FMT_ARGB4444.");
            fullClrKey = convert4444To8888(colorKeyInfo.u8Red, colorKeyInfo.u8Green, colorKeyInfo.u8Blue);
        }
        break;
        case E_MI_FB_COLOR_FMT_ARGB1555:
        {
        	UI_print("E_MI_FB_COLOR_FMT_ARGB1555.");
            fullClrKey = convert1555To8888(colorKeyInfo.u8Red, colorKeyInfo.u8Green, colorKeyInfo.u8Blue);
        }
        break;
        case E_MI_FB_COLOR_FMT_ARGB8888:
        {
        	UI_print("E_MI_FB_COLOR_FMT_ARGB8888.");
            fullClrKey = (0xff << 24) | (colorKeyInfo.u8Red << 16) | (colorKeyInfo.u8Red << 8) | (colorKeyInfo.u8Red);
        }
        break;
        default :
        {
        	UI_print("default.");
            printf("generateClrKeyFromRGB888 invalid clrfmt=%d\n", efmt);
        }
        break;
     }
	 
    /* Set RGB chanel according to fullClrKey */
    colorKeyInfo.u8Red = (fullClrKey >> 16) & 0xff;
    colorKeyInfo.u8Green = (fullClrKey >> 8) & 0xff;
    colorKeyInfo.u8Blue = fullClrKey & 0xff;
    printf("FBIOSET_COLORKEY enable colorkey the filter color[0x%x,0x%x,0x%x]\n", \
        		colorKeyInfo.u8Red, colorKeyInfo.u8Green, colorKeyInfo.u8Blue);
	
    if(-1 == ioctl(fd, FBIOSET_COLORKEY, &colorKeyInfo))
	{
        perror("Error: failed to FBIOGET_COLORKEY");
        return -1;
    }
	
    close (fd);
    return 0;
}
