/**
 * \file mgi.h
 * \author Wei Yongming <ymwei@minigui.org>
 * \date 2008/08/12
 *
 * This file includes input method interfaces of mGi. 
 */

/*
 * $Id: mgi.h 100 2006-01-17 02:21:25Z qzlong $
 *
 * mGi (MiniGUI Input Method) is a component of MiniGUI.
 *
 * MiniGUI is a compact cross-platform Graphics User Interface 
 * (GUI) support system for real-time embedded systems.
 *                  
 * Copyright (C) 2002-2008 FMSoft.
 */

#ifndef __MGI_H__
#define __MGI_H__

#include <minigui/common.h> 
#include <minigui/minigui.h> 
#include <minigui/gdi.h> 
#include <minigui/window.h> 
#include <minigui/control.h> 
#include <minigui/endianrw.h>

#ifdef __cplusplus
extern  "C"
{
#endif                          /* __cplusplus */

    /**
     * \addtogroup defs Defines
     * @{
     */

/** BackGround color of IME window*/
#define INPUT_BGCOLOR           8          /* LightBlack */
/** ForeGround color of IME window*/
#define INPUT_FGCOLOR           15         /* LightWhite */

#define INPUT_AREAY          ((18 * 24)+10)

#define MAX_INPUT_LENGTH        15

#define MAGIC_NUMBER            "CCEGB"
#define CIN_ENAME_LENGTH        24
#define CIN_CNAME_LENGTH        16
#define MAX_PHRASE_LENGTH       20
#define SELECT_KEY_LENGTH       16
#define END_KEY_LENGTH          16
#define MAX_SEL_LENGTH 			58
/** Max number of selections */
#define MAXDUPSEL               7

#define InputAreaX              16

/** Max length of IME's name */
#define MAX_IME_NAME    20

    /** @} end of defs */

    /**
     *  \defgroup container_win_ncs Notification codes of IME container window 
     *  @{
     */

/**
 * \def MGIN_SKIME
 * \brief The MGIN_SKIME notification message is sent when the user clicks
 *        the soft-keyboard icon on the IME container window.
 */
#define MGIN_SKIME      300

/**
 * \def MGIN_HWIME 
 * \brief The MGIN_HWIME notification message is sent when the user clicks
 *        the handwrite icon on the IME container window.
 */
#define MGIN_HWIME      301

    /** @} end of container_win_ncs */


    /**
     * \addtogroup Enums IME Windows adding status enums
     * @{
     */

/** 
 * \enum _IME_WINDOW_STATUS
 * \brief The enum specifies the status of adding a new IME window 
 */
enum _IME_WINDOW_STATUS {
    /**
     * IME window added successfully.
     */
    IME_WINDOW_SUCCESS,
    /**
     * the IME window that gonna add is invalid
     * or the container hasn't been created yet.
     */
    IME_WINDOW_INVALID,
    /**
     * the IME window has already added before. 
     */
    IME_WINDOW_EXISTED,
    /**
     * the IME window container is full. 
     */
    IME_WINDOW_FULL,
};

    /** @} end of Enums */

/* key of toggle input method */

typedef struct
{
    unsigned long key1;     /* sizeof(ITEM=12) */
    unsigned long key2;
    unsigned short ch;
    unsigned short frequency;
} ITEM;

typedef struct
{
    char    magic_number[sizeof(MAGIC_NUMBER)];     /* magic number */
    char    ename[CIN_ENAME_LENGTH];        /* ascii name */
    char    cname[CIN_CNAME_LENGTH];        /* prompt */
    char    selkey[SELECT_KEY_LENGTH];      /* select keys */

    char    last_full;      /* last full code need a more SPACE? */
    int     TotalKey;       /* number of keys needed */
    int     MaxPress;       /* Max len of keystroke */
    int     MaxDupSel;      /* how many keys used to select */
    int     TotalChar;      /* Defined characters */

    unsigned char KeyMap[128];      /* Map 128 chars to 64(6 bit) key index */
    unsigned char KeyName[64];      /* Map 64 key to 128 chars */
    unsigned short KeyIndex[64];    /* 64 key first index of TotalChar */

    int     PhraseNum;      /* Total Phrase Number */
    FILE   *PhraseFile;     /* *.tab.phr Phrase File */
    FILE   *AssocFile;      /* *.tab.lx LianXiang File */
    ITEM   *item;           /* item */
} mgime_input_table;

typedef struct
{
    short   l, t, r, b;
} hit_addr;

void    __mg_ime_outchar(unsigned char ch, LPARAM lParam);

    /**
     ** \addtogroup fns Functions
     ** @{
     **/

    /**
     ** \defgroup ime_container_fns IME Window container functions
     ** @{
     **/

/** IME Window Information */
typedef struct _IME_WIN {
    /** Handle of IME Window */
    HWND ime_hwnd;
    /** Name of IME Window */
    char ime_name[MAX_IME_NAME];
} IME_WIN;

/** IME Window Container Information */
typedef struct _IME_CONTAINER {
    /** The IME container window */
    HWND cont_hwnd;
    /** The max number of IME Windows that the container contains */
    int max_ime_windows;
    /** The current IME window ID */
    int cur_wins;
    /** IME Window Information */
    IME_WIN* ime_wins;
} IME_CONTAINER;

/**
 * \fn GHANDLE mgiCreateIMEContainer (int max_ime_wins, BOOL with_cont_win) 
 * \brief Create the container of input method windows.
 *
 * You can create a container of input method windows by this function, and
 * you can specify the max input method window numbers of this container.
 *
 * \param max_ime_wins  the max ime window numbers of this container.
 * \param with_cont_win if show the container window.
 *
 * \return Return the handle of this ime windows container.
 *
 * \sa mgiDestroyIMEContainer 
 */
GHANDLE mgiCreateIMEContainer (int max_ime_wins, BOOL with_cont_win);

/**
 * \fn int mgiAddIMEWindow (GHANDLE ime, HWND ime_hwnd, const char* ime_name) 
 * \brief Add a ime window to the container.
 *
 * You can add a input method window to the certain container by this function.
 *
 * \param ime       the handle of ime windows container.
 * \param ime_hwnd  the handle of this ime window.
 * \param ime_name  the name of this ime window.
 *
 * \return Return the handle of this ime windows container.
 *
 * \sa mgiRemoveIMEWindow 
 */
int mgiAddIMEWindow (GHANDLE ime, HWND ime_hwnd, const char* ime_name);

/**
 * \fn BOOL mgiRemoveIMEWindow (GHANDLE ime, const char* ime_name) 
 * \brief Remove a ime window from the container.
 *
 * You can remove a input method window from the certain container by this function.
 *
 * \param ime       the handle of ime windows container.
 * \param ime_name  the name of this ime window.
 *
 * \return Return TRUE if the ime window has been removed successfully,
 *         FALSE otherwise.
 *
 * \sa mgiAddIMEWindow 
 */
BOOL mgiRemoveIMEWindow (GHANDLE ime, const char* ime_name);

/**
 * \fn BOOL mgiSetActiveIMEWindow (GHANDLE ime, const char* ime_name)
 * \brief Set a ime window actived.
 *
 * You can set a input method window actived by this function.
 *
 * \param ime       the handle of ime windows container.
 * \param ime_name  the name of this ime window.
 *
 * \return Return TRUE if set actived successfully, FALSE otherwise.
 *
 */
BOOL mgiSetActiveIMEWindow (GHANDLE ime, const char* ime_name);

/**
 * \fn NOTIFPROC mgiSetNotificationCallback (GHANDLE ime, NOTIFPROC notif_proc) 
 * \brief Set the ime windows container window's notification callback.
 *
 * You can set the ime windows container window's notification callback
 * if you created the container window.
 *
 * \param ime           the handle of ime windows container.
 * \param notif_proc    the new notification callback of the container window
 *
 * \return Return old notification callback of the container window
 *
 * \sa mgiCreateIMEContainer 
 */
NOTIFPROC mgiSetNotificationCallback (GHANDLE ime, NOTIFPROC notif_proc);

/**
 * \fn BOOL mgiDestroyIMEContainer (GHANDLE ime)
 * \brief Destroy the ime windows container.
 *
 * You can destroy the ime windows container by this function.
 *
 * \param ime       the handle of ime windows container.
 *
 * \return Return TRUE if destroy successfully, FALSE otherwis.
 *
 * \sa mgiCreateIMEContainer 
 */
BOOL mgiDestroyIMEContainer (GHANDLE ime);


    /** @} end of ime_container_fns */

#define LEFT    0
#define TOP     36
#define MSG_MYIME_CLOSE	    (0x0000ffff + 200 + 1)
#define MSG_MYIME_OPEN	    (0x0000ffff + 200 + 2)
#define MSG_KEYBOARD_CLOSE   (0x0000ffff + 200 + 3)
#define MSG_KEYBOARD_OPEN    (0x0000ffff + 200 + 4)

#define OPS_NAME_LENGTH     20
#define NR_INPUTMETHOD      10

    /**
     * \addtogroup Enums soft-keyboard key type enums
     * @{
     */

/** 
 * The key of soft-keyboard's type.
 */
typedef enum {
    /**
     * Scancode type key.
     * You can click this kind of key to send a scancode.
     */
    SFT_SCANCODE,
    /**
     * String type key.
     * You can click this kind of key to send a string.
     */
    SFT_STRING,
} SOFTKEYTYPE;

    /** @} end of Enums */

    /**
     ** \addtogroup fns Functions
     ** @{
     **/

    /**
     ** \defgroup sk_fns Soft-keyboard input method functions
     ** @{
     **/

/** Each Soft-keyboard Key Information */
typedef struct _MGI_SOFTKEY_INFO {
    /** Key type */
    SOFTKEYTYPE type;
    /** The string that this key represents,
     *  available if type is SFT_STRING
     */
    char* str;
    /** The scancode that this key represents,
     *  available if type is SFT_SCANCODE
     */
    int scancode;

    /** The key rect in the keyboard bitmap */
    RECT rect;

    /** Key status */
    int status;
} MGI_SOFTKEY_INFO;

/**
 * \var typedef void (*MGI_CB_Translate_Init) (void) 
 * \brief Type of the translation method init callback procedure.
 *
 * This is the function type of translation method init callback procedure.
 * If you set a new translation method for the soft-keyboard, you might init
 * the resources in this function
 *
 */
typedef void (*MGI_CB_Translate_Init) (void);

/**
 * \var typedef void (*MGI_CB_Translate_Destroy) (void) 
 * \brief Type of the translation method destroy callback procedure.
 *
 * This is the function type of translation method destroy callback procedure.
 * If you set a new translation method for the soft-keyboard, you might destroy
 * the resources finally in this function.
 *
 */
typedef void (*MGI_CB_Translate_Destroy) (void);

/**
 * \var typedef void (*MGI_CB_Translate_Draw) (HWND hwnd, HDC hdc, void* context) 
 * \brief Type of the translation method draw callback procedure.
 *
 * This is the function type of translation method draw callback procedure.
 * If you set a new translation method for the soft-keyboard, you should draw
 * the relative contents in this function.
 *
 */
typedef void (*MGI_CB_Translate_Draw) (HWND hwnd, HDC hdc, void* context);

/**
 * \var typedef int (*MGI_CB_Translate_Alpha)(const char* origin_str, char* ret_value, LPARAM lParam, BOOL* write_directly)
 * \brief Type of the translation method alpha-translate callback procedure.
 *
 * This is the function type of translation method alpha-translate callback procedure.
 * If you set a new translation method for the soft-keyboard, you should translate 
 * alpha in this function.
 *
 */
typedef int (*MGI_CB_Translate_Alpha) (const char* origin_str, char* ret_value,
                                       LPARAM lParam, BOOL* write_directly);

/**
 * \var typedef int (*MGI_CB_Translate_Number) (const char* origin_str, char* ret_value, BOOL* write_directly)
 * \brief Type of the translation method number-translate callback procedure.
 *
 * This is the function type of translation method number-translate callback procedure.
 * If you set a new translation method for the soft-keyboard, you should translate 
 * number in this function.
 *
 */
typedef int (*MGI_CB_Translate_Number) (const char* origin_str, char* ret_value, 
                                        BOOL* write_directly);

/**
 * \var typedef int (*MGI_CB_Translate_Punc) (const char origin_str, char* ret_value) 
 * \brief Type of the translation method punctuation-translate callback procedure.
 *
 * This is the function type of translation method punctuation-translate callback procedure.
 * If you set a new translation method for the soft-keyboard, you should translate 
 * punctuation in this function.
 *
 */
typedef int (*MGI_CB_Translate_Punc) (const char origin_str, char* ret_value);

/**
 * \var typedef int (*MGI_CB_Associate) (const char* origin_str, char* ret_value) 
 * \brief Type of the translation method associate callback procedure.
 *
 * This is the function type of translation method associate callback procedure.
 * If you set a new translation method for the soft-keyboard, you should fetch
 * associate words in this function.
 *
 */
typedef int (*MGI_CB_Associate) (const char* origin_str, char* ret_value);

/** Translation Operation Information */
typedef struct _MGI_TRANSLATE_OPS {
    /** Name of this trans-ops */
    char ops_name[OPS_NAME_LENGTH];
    /** whether this trans-ops is for single byte charset */
    BOOL is_ascii;

    /** Initialization operation */
    MGI_CB_Translate_Init cb_init;
    /** Destroy operation */
    MGI_CB_Translate_Destroy cb_destroy;
    /** Draw operation */
    MGI_CB_Translate_Draw cb_draw;
    /** Alpha-translation operation */
    MGI_CB_Translate_Alpha cb_trans_alpha;
    /** Number-translation operation */
    MGI_CB_Translate_Number cb_trans_num;
    /** Punctuation-translation operation */
    MGI_CB_Translate_Punc cb_trans_punc;
    /** Associate operation */
    MGI_CB_Associate cb_associate;
} MGI_TRANSLATE_OPS;

/** Soft-keyboard Information */
typedef struct _MGI_SOFTKEYBOARD_INFO {
    /** The bitmap used by the soft-keyboard */
    BITMAP skb_bmp;
    /** All keys of the soft-keyboard */
    MGI_SOFTKEY_INFO* keys;
    /** Numbers of key */
    int nr_keys;
    /** Layout of keyboard */
    char* kbd_layout;

    /** Current translation ops index */
    int cur_trans_ops;
    /** Translation opses */
    MGI_TRANSLATE_OPS* trans_ops[NR_INPUTMETHOD];
} MGI_SOFTKEYBOARD_INFO;

typedef struct _KEYEVENT
{
    int     scancode;
    DWORD   status;
} KEYEVENT;

extern MGI_TRANSLATE_OPS en_trans_ops;
extern MGI_TRANSLATE_OPS quanpin_trans_ops;

/**
 * \fn HWND mgiCreateSoftKeyboardIME (MGI_SOFTKEYBOARD_INFO* sk_info, PLOGFONT ime_font) 
 * \brief Create the soft-keyboard input method window.
 *
 * You can create the soft-keyboard input method window by this function.
 *
 * \param sk_info  the soft-keyboard information.
 * \param ime_font the font that the IMEwindow uses.
 *
 * \return Return the handle of this ime window.
 *
 */
HWND mgiCreateSoftKeyboardIME (MGI_SOFTKEYBOARD_INFO* sk_info, PLOGFONT ime_font);

/**
 * \fn BOOL mgiAddSKBTranslateOps (HWND ime_hwnd, MGI_TRANSLATE_OPS* trans_ops) 
 * \brief Add a suite of translation operation to the soft-keyboard IME window.
 *
 * You can add a suite of translation operation to the soft-keyboard IME window
 * by this function.
 *
 * \param ime_hwnd  the soft-keyboard window handle.
 * \param trans_ops the suite of translation operation gonna add.
 *
 * \return Return TRUE if added successfully, FALSE otherwise.
 *
 */
BOOL mgiAddSKBTranslateOps (HWND ime_hwnd, MGI_TRANSLATE_OPS* trans_ops);

    /** @} end of sk_fns */

#if 0
extern int     init_soft_keyboard(void);
extern void    refresh_soft_keyboard_bk(HWND hwnd, HDC hdc);
extern void    refresh_soft_keyboard_key(HWND hwnd, HDC hdc);
extern int     soft_keyboard_mouse_down(HWND hWnd, int x, int y, KEYEVENT * ke);
extern int     soft_keyboard_down(HWND hWnd, int, KEYEVENT * ke);
extern int     soft_keyboard_up(HWND hWnd, KEYEVENT * ke);
extern void    move_ime_window(HWND hWnd, HWND target);
extern int     active_soft_key(void *context, HWND hWnd, int message, WPARAM wParam,
                    LPARAM lParam);
extern int cursor_direct(HWND hWnd, int scancode);
extern void update_input_window(HWND hWnd);
extern void destroy_soft_keyboard(void);
#endif

#define DEF_HW_WIDTH    280
#define DEF_HW_HEIGHT   150

#define CTRL_HWI    "hwi"
/* full screen hand write support */
#define SUPPORTS_FULLSCR_HW	1

#define REC_RANGE     ((HW_NUMERIC) | (HW_LCALPHA) | (HW_UCALPHA) | (HW_PUNCTUATION) | (HW_SYMBOLS) | (HW_CHINESE_COMMON) | (HW_CHINESE_RARE) |(HW_CHINESE_VARIANT))
#define FROM_HWINPUT 0x10000000
#define IS_CHINESE   0x01000000

#define NWORD        10
#define MAXSIZE      300
#define NALLOC       MAXSIZE 
#define THRESHHOLD_MIN    9
#define THRESHHOLD_MAX    169 

#define THRESHHOLD_FULLSCREEN	100

#define SPACE            2

#if SUPPORTS_FULLSCR_HW
  #define HW_MODE_NORMAL	0
  #define HW_MODE_FULLSCR	1

  #define HW_FULL_ON_WRITE	0
  #define HW_FULL_WRITE_OVER	1

//message to set or get the current handwrite mode
#define MSG_HW_SETMODE		(MSG_USER+20)
#define MSG_HW_GETMODE		(MSG_USER+21)
//message to show or hide the transparent window
#define MSG_HW_SHOW		(MSG_USER+22)
#define MSG_HW_SETWRITE		(MSG_USER+23)
#define MSG_HW_GETSTATUS	(MSG_USER+24)

//handwrite window height in the fullscreen mode
#define FULLSCR_HW_WND_H	16

#ifdef PDA_SIZE_160x160
#define FULL_NWORD        6
#else
#define FULL_NWORD        10 
#endif

#endif

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef DWORD
#define DWORD unsigned long
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

    /**
     * \addtogroup hw_fns Handwrite input method functions
     * @{
     */

/** Handwrite Engine Information */
typedef struct _MGI_HW_ENGINE
{
    /** Initialization operation of the engine */
    //int (*init)(void);
    int (*init)(int w, int h);
    /** Recognize operation of the engine */
    int (*recognize)(WORD* pTrace,char* pResult,int *nCands, int nPoints, WORD wRange);
    /** Exit operation of the engine */
    BOOL (*exit)(void);
} MGI_HW_ENGINE;

#define NUMBER_RECOG_RANGE           0x0001
#define UPPERCASE_RECOG_RANGE        0x0002
#define LOWERCASE_RECOG_RANGE        0x0004
#define INTERPUNCTION_RECOG_RANGE    0x0008
#define ADAPTATION_RECOG_RANGE       0x1000

/** Handwrite Information */
typedef struct _MGI_HANDWRITE_INFO {
    /** Handwrite window's left top x coordinate */
    int x;
    /** Handwrite window's left top y coordinate */
    int y;
    /** Handwrite window's width */
    int w;
    /** Handwrite window's height */
    int h;

    /** Whether the Handwrite window has two drawing area */
    BOOL two_wins;
    /** The handwrite recognization engine */
    MGI_HW_ENGINE* engine;
} MGI_HANDWRITE_INFO;

/**
 * \fn HWND GBIMEWindowEx (HWND hosting, int lx, int ty, int rx, int by, \
 *               BOOL two_lines)
 * \brief Creates a GB2312 input method window.
 *
 * This function creates a GB2312 (a charset for simplified Chinese) input 
 * method window. Before calling this window, you must create a main window 
 * which acts as the hosting main window of the IME window.
 *
 * \param hosting The hosting window of the IME window. 
 *        Do not set it to be HWND_DESKTOP??.
 * \param lx lx,ty,rx,by: The position and size of the IME window.
 * \param ty lx,ty,rx,by: The position and size of the IME window.
 * \param rx lx,ty,rx,by: The position and size of the IME window.
 * \param by lx,ty,rx,by: The position and size of the IME window.
 * \param two_lines Indicates whether the IME window contains two lines.
 *
 * \return The handle to the IME window.
 *
 * \note This function only define for MiniGUI-Processes, and can be 
 *       called only by the server of MiniGUI-Processes.
 *
 * \sa GBIMEWindow
 */
MG_EXPORT HWND GBIMEWindowEx (HWND hosting, int lx, int ty, int rx, int by, 
                BOOL two_lines);

/**
 * \fn HWND mgiCreateHandWriteIME (MGI_HANDWRITE_INFO* hw_info, PLOGFONT ime_font) 
 * \brief Create the handwrite input method window.
 *
 * You can create the handwrite input method window by this function.
 *
 * \param hw_info  the handwrite information.
 * \param ime_font the font that the IMEwindow uses.
 *
 * \return Return the handle of this ime window.
 *
 */
HWND mgiCreateHandWriteIME (MGI_HANDWRITE_INFO* hw_info, PLOGFONT ime_font);

/**
 * \fn BOOL mgiSetHandWriteEngine (HWND hw_ime_hwnd, MGI_HW_ENGINE* engine) 
 * \brief Set the handwrite input method recognization engine.
 *
 * You can set the handwrite input method recognization engine by this function.
 *
 * \param hw_ime_hwnd  the handle of handwrite window.
 * \param engine       the recognization engine gonna set.
 *
 * \return Return TRUE if set successfully, FALSE otherwise.
 *
 */
BOOL mgiSetHandWriteEngine (HWND hw_ime_hwnd, MGI_HW_ENGINE* engine);

    /** @} end of hw_fns */

    /**
     ** \defgroup new_sk_fns New Soft Keypad input method functions
     ** @{
     **/

HWND mgiCreateSoftKeypad (void (*cb)(BOOL SoftKeyWindowIsShown));

    /** @} end of new_sk_fns */

    /** @} end of fns */

/* struct defines */

typedef struct tagCHARINFO {
    BYTE    code[2];
    RECT    rc;
}CHARINFO;

typedef struct tagHWINFO {
    WORD *     m_data;
    int        m_size; 
    int        m_len; 

    BOOL       capture;
    int        old_x;
    int        old_y;

    BYTE       result[NWORD*3 + 1];
    //BITMAP     keybitmap;
    BYTE       fitchar[NWORD][4];
    HWND       phwnd;

    RECT       *rcDraw;
    //RECT       rcClear;
    CHARINFO   charinfo[NWORD];
    
    RECT       rcchar; 
/* added by snig for new hw */
    RECT       rcl;
    RECT       rcr;
    char       active_rc;
    int        nword;    /* words to recognize */
    HWND       hDrawWnd; /* handle of the current draw window */
    HWND       hHWI;
    int        width;
    int        height;
/****************************/

    BOOL two_wins;
    MGI_HW_ENGINE* engine;
} HWINFO;

typedef HWINFO* PHWINFO;

BOOL RegisterHWIControl (void);
void HWIControlCleanup (void);
int HWIMECtrlProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam);

void on_command (HWND, int);

extern MGI_HW_ENGINE hw_null_engine;
#ifdef HW_CHINESE
extern MGI_HW_ENGINE hw_zinnia_engine;
#endif

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif /* __MGI_H__ */
