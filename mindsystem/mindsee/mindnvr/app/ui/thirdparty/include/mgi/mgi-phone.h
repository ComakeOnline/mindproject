/**
 * \file mgi-phone.h
 * \author He Jian <hejian@minigui.org>
 * \date 2008/08/12
 *
 * This file includes input method interfaces of mGi-phone. 
 */

/*
 * $Id: mgime.h 100 2006-01-17 02:21:25Z qzlong $
 * mGi (MiniGUI Inputmethod) is a component of MiniGUI.
 *
 * MiniGUI is a compact cross-platform Graphics User Interface 
 * (GUI) support system for real-time embedded systems.
 *                  
 * Copyright (C) 2002-2008 FMSoft.
 */

#ifndef _MGI_PHONE_H
#define _MGI_PHONE_H

#ifdef __cplusplus
extern "c" {
#endif /*__cplusplus*/

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#ifdef _MGRM_STANDALONE
#include <pthread.h>
#include <semaphore.h>
#endif

#define PHONE_SCREEN_WIDTH_240

#ifdef PHONE_SCREEN_WIDTH_128
#define PHONE_RESULT_BUFF_LONG      16
#endif

#ifdef PHONE_SCREEN_WIDTH_176
#define PHONE_RESULT_BUFF_LONG      20
#endif

#ifdef PHONE_SCREEN_WIDTH_240
#define PHONE_RESULT_BUFF_LONG      28
#endif

/* */
typedef struct _MGI_PHONE_KEY_MAP {
    int scancode;
    const char* letters;
} MGI_PHONE_KEY_MAP;

/* text color*/
typedef struct _MGI_PHONE_IME_TEXTCOLOR
{
    DWORD firstline_text_color;
    DWORD firstline_focus_text_color;
    DWORD firstline_focus_color;
    DWORD secondline_text_color;
    DWORD secondline_focus_text_color;
    DWORD secondline_focus_color;
}MGI_PHONE_IME_TEXTCOLOR;

typedef int (* MGICB_ON_PHONE_IME_SWITCHED)(void *method_data, int new_method_id);

/*
 * fn   : HWND mgiCreatePhoneKeyPadIME (RECT *rect，BITMAP *bk_bmp, MGI_PHONEPAD_TEXTCOLOR *textcolor)
 * brief: create phonepad ime.
 */

#define MAX_PHONE_KEY_NUM   18

HWND mgiCreatePhoneKeyPadIME (RECT *rect, BITMAP *bkgnd_bmp, PLOGFONT log_font, const MGI_PHONE_IME_TEXTCOLOR *textcolor, \
       const MGI_PHONE_KEY_MAP(*key_map)[MAX_PHONE_KEY_NUM], MGICB_ON_PHONE_IME_SWITCHED on_ime_switched);


void mgiDestroyPhoneKeyPadIME(HWND hwnd);

/*
 * var  : typedef BOOL (*MGICB_PHONE_IME_CLEAN)(void *method_data);
 * brief: type of exit to phonepad ime callback
 */
 typedef BOOL (* MGICB_PHONE_IME_CLEAN)(void *method_data);
/*
 * var  : typedef int (*MGI_CB_PHONEMATCHKEYSTROKES)(char *strokes, char *buffer, int buffer_len, int index);
 * brief: type of phonepad ime callback which match word by strokes
 */
 typedef int (*MGICB_PHONE_IME_MATCH_KEYSTROKES)(void *method_data, const char *strokes, char *buffer, int buffer_len, int index);

/*
 * var  : typedef int (*MGICB_PHONE_IME_TRANSLATE_WORD)(char *strokes, char *buffer, int buffer_len, int index);
 * brief: type of phonepad ime callback which translate strokes to word
 */
typedef int (*MGICB_PHONE_IME_TRANSLATE_WORD)(void *method_data, const char *strokes, char *buffer, int buffer_len, int index);

/*
 * var  : typedef int (*MGICB_PHONE_IME_PREDICT_WORD)(char *loofor_str, char *buffer, int buffer_len, int index);
 * brief: type of phonepad ime callback which predict word by loofor_str
 */
typedef int (*MGICB_PHONE_IME_PREDICT_WORD)(void *method_data, const char *lookfor_str, char *buffer, int buffer_len, int index);

/*
 * var  : typedef int (* MGI_CB_PHONECHANGESTATUS)(void);
 * brief: type of phonepad ime callback which change input mothod's status
 */
typedef int (* MGICB_PHONE_IME_STATUS_CHANGED)(void *method_data, DWORD status);

/**/
typedef int (* MGICB_PHONE_IME_ACTIVED)(void *method_data, BOOL is_active);

/* */
typedef int (* MGICB_PHONE_IME_GET_SYMBOL)(void *method_data, char *buffer, int buffer_len);

#define PHONE_NAME_MAX 32
typedef struct _MGI_PHONE_IME_METHOD {
    int                                 method_id;
    char                                method_name[PHONE_NAME_MAX];

    MGICB_PHONE_IME_CLEAN               clean;
    MGICB_PHONE_IME_MATCH_KEYSTROKES    match_keystrokes;
    MGICB_PHONE_IME_TRANSLATE_WORD      translate_word;
    MGICB_PHONE_IME_PREDICT_WORD        predict_word;
    MGICB_PHONE_IME_STATUS_CHANGED      status_changed;
    MGICB_PHONE_IME_ACTIVED             actived;
    MGICB_PHONE_IME_GET_SYMBOL          get_symbol;

    void                                *private_data;
    struct _MGI_PHONE_IME_METHOD        *next;
}MGI_PHONE_IME_METHOD;

/*
 * fn   : BOOL mgiPhoneKeyPadAddMethod (HWND ime_hwnd, MGI_PHONE_IME_METHOD *method_data);
 * brief: add input method to phone keypad ime
 */
BOOL mgiPhoneKeyPadAddMethod (HWND ime_hwnd, MGI_PHONE_IME_METHOD *method_data);

/*
 * fn   : BOOL mgiPhoneKeyPadGetMethodCode (char *code, int method_id);
 * brief: get code of method in phone keypad ime
 */
BOOL mgiPhoneKeyPadGetMethodCode (char *code, int code_len, int method_id);

/*
 * fn   : BOOL mgiPhoneKeyPadRemoveMethod (HWND ime_hwnd, int method_id);
 * brief: remove method by method_id from phone keypad ime
 */
BOOL mgiPhoneKeyPadRemoveMethod (HWND ime_hwnd, int method_id);

/*
 * fn   : int mgiPhoneKeyPadSetCurrMethod (int method_id);
 * brief: set current method of phone keypad ime
 */
int mgiPhoneKeyPadSetCurrMethod (int method_id);

/*
 * fn   : int mgiPhoneKeyPadGetCurrMethod (void);
 * brief: get current method of phone keypad ime
 */
int mgiPhoneKeyPadGetCurrMethod (void);


#if defined(_LITE_VERSION) && !defined(_STAND_ALONE)
BOOL mgiPhoneKeyPadServerInstallRequest(void);
#endif

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif

