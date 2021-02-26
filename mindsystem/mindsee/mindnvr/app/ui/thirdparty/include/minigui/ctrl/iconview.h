/**
 * \file iconview.h
 * \author Wei Yongming <ymwei@minigui.org>
 * \date 2002/01/06
 * 
 \verbatim

    Copyright (C) 2002-2008 Feynman Software
    Copyright (C) 1998-2002 Wei Yongming

    All rights reserved by Feynman Software.

    This file is part of MiniGUI, a compact cross-platform Graphics 
    User Interface (GUI) support system for real-time embedded systems.

 \endverbatim
 */

/*
 * $Id: iconview.h 12037 2009-08-13 01:25:52Z houhuihua $
 *
 *             MiniGUI for Linux/uClinux, eCos, uC/OS-II, VxWorks, 
 *                     pSOS, ThreadX, NuCleus, OSE, and Win32.
 *
 *             Copyright (C) 2002-2008 Feynman Software.
 *             Copyright (C) 1998-2002 Wei Yongming.
 */

#ifndef EXT_ICONVIEW_H
#define EXT_ICONVIEW_H


#ifdef  __cplusplus
extern "C" {
#endif

    /**
     * \addtogroup controls
     * @{
     */

    /**
     * \defgroup mgext_ctrl_iconview IconView control
     * @{
     */
/** 
 * \def CTRL_ICONVIEW
 * \brief The class name of iconview control.
 */
#define CTRL_ICONVIEW       ("IconView")

/** Iconview return value */
#define IV_OKAY                 0
/** Iconview return value */
#define IV_ERR                  (-1)
/** Iconview return value */
#define IV_ERRSPACE             (-2)
    
/**
 * \var typedef GHANDLE HIVITEM
 * \brief Icon view item handle
 */
typedef GHANDLE HIVITEM;

/** 
 * Structure of the iconview item info, contains information about an item.
 * This structure is used for creating or retrieving an item. 
 */
typedef struct _IVITEMINFO
{
    /**
     * The Index of the item
     */
    int nItem;

    /**
     * The bitmap icon of the item
     */
    PBITMAP bmp;

    /**
     * The text label of the item
     */
    const char *label;

    /** Attached additional data of this item */
    DWORD addData;

    /**
     * Reserved
     */
    DWORD dwFlags;

} IVITEMINFO;
typedef IVITEMINFO* PIVITEMINFO;

/** Iconview bitmap flags, reserved */
#define IVFLAG_BITMAP	0x0001
/** Iconview icon flags, reserved */
#define IVFLAG_ICON	0x0002


/**
 * \fn int iconview_is_item_hilight (HWND hWnd, GHANDLE hivi)
 * \brief Uses this to get an iconview item is hilighted or not.
 *
 * \returns TRUE when item is hilighted, or FALSE. 
 */
MG_EXPORT int iconview_is_item_hilight (HWND hWnd, GHANDLE hivi);

/**
 * \fn DWORD iconview_get_item_bitmap (GHANDLE hivi)
 * \brief Uses this to get bitmap handle from an iconview item. 
 *
 * \returns the bitmap handle of iconview item.
 */
MG_EXPORT PBITMAP iconview_get_item_bitmap (GHANDLE hivi);

/**
 * \fn DWORD iconview_get_item_label (GHANDLE hivi)
 * \brief Uses this to get text label from an iconview item.
 *
 * \returns the text label of iconview item.
 */
MG_EXPORT const char* iconview_get_item_label (GHANDLE hivi);

/** 
 * \fn DWORD iconview_get_item_adddata (GHANDLE hivi)
 * \brief Uses this to get additional data from an iconview item.
 *
 * \returns the additional data of iconview item.
 */
MG_EXPORT DWORD iconview_get_item_adddata (GHANDLE hivi);

MG_EXPORT GHANDLE iconview_get_item(HWND hwnd, int index);

MG_EXPORT BOOL iconview_set_item_lable(GHANDLE hivi, const char* strLable);

MG_EXPORT PBITMAP iconview_set_item_bitmap(GHANDLE hivi, PBITMAP pbmp);


    /**
     * \defgroup mgext_ctrl_iconview_styles Styles of iconview control
     * @{
     */

/* reserved */
/**
 * \def IVS_NOTIFY
 * \brief The notify style of iconview control
 * \sa SVS_NOTIFY
 */ 
#define IVS_NOTIFY              SVS_NOTIFY

/**
 * \def IVS_UPNOTIFY
 * \brief The upnotify style of iconview control
 * \sa SVS_UPNOTIFY
 */
#define IVS_UPNOTIFY            SVS_UPNOTIFY

/**
 * \def IVS_AUTOSORT
 * \brief The auto sort style of iconview control
 * \sa SVS_AUTOSORT
 */ 
#define IVS_AUTOSORT            SVS_AUTOSORT

/**
 * \def IVS_LOOP
 * \brief The loop style of iconview control
 * \sa SVS_LOOP
 */ 
#define IVS_LOOP                SVS_LOOP


    /** @} end of mgext_ctrl_iconview_styles */

    /**
     * \defgroup mgext_ctrl_iconview_msgs Messages of ListView control
     * @{
     */

/**
 *  \def IVM_ADDITEM
 *  \brief Adds a item to iconview. 
 *  
 *  \code 
 *  IVM_ADDITEM
 *  PIVITEMINFO p
 *
 *  p =(LPARAM)lParam;
 *  \endcode
 *
 *  \param p Pointes to a IVITEMINFO structure that contains the information of 
 *           the new item to be added. nItem member of the IVITEMINFO struct speficied 
 *           the item position in its parent item, beginning with zero.
 *
 *  \return Returns the handle of the new item if successful, or 0 otherwise.
 */
#define IVM_ADDITEM            0xF300

/**
 * \def IVM_SETITEMSIZE
 * \brief Sets the item width and height of an iconview control
 *
 * All the items have the same item size. This message should be sent
 * before you add items to iconview control.
 *
 * \code
 * IVM_SETITEMSIZE
 * int width;
 * int height;
 *
 * wParam = width;
 * lParam = height;
 * \endcode
 *
 * \param width  Width of an item.
 * \param height Height of an item.
 *
 * \return 0.
 */
#define IVM_SETITEMSIZE           0xF436


/**
 * The same common control messages as scrollview control
 * \sa SVM_RESETCONTENT
 */
#define IVM_RESETCONTENT        SVM_RESETCONTENT
/**
 * The same common control messages as scrollview control
 * \sa SVM_DELITEM
 */
#define IVM_DELITEM             SVM_DELITEM
/**
 * The same common control messages as scrollview control
 * \sa SVM_SETITEMDRAW
 */
#define IVM_SETITEMDRAW         SVM_SETITEMDRAW
/**
 * The same common control messages as scrollview control
 * \sa SVM_SETCONTWIDTH
 */
#define IVM_SETCONTWIDTH        SVM_SETCONTWIDTH
/**
 * The same common control messages as scrollview control
 * \sa SVM_SETCONTHEIGHT
 */
#define IVM_SETCONTHEIGHT       SVM_SETCONTHEIGHT
/**
 * The same common control messages as scrollview control
 * \sa SVM_SETITEMOPS
 */
#define IVM_SETITEMOPS          SVM_SETITEMOPS
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETMARGINS
 */
#define IVM_GETMARGINS          SVM_GETMARGINS
/**
 * The same common control messages as scrollview control
 * \sa SVM_SETMARGINS
 */
#define IVM_SETMARGINS          SVM_SETMARGINS
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETLEFTMARGIN
 */
#define IVM_GETLEFTMARGIN       SVM_GETLEFTMARGIN
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETTOPMARGIN
 */
#define IVM_GETTOPMARGIN        SVM_GETTOPMARGIN
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETRIGHTMARGIN
 */
#define IVM_GETRIGHTMARGIN      SVM_GETRIGHTMARGIN
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETBOTTOMMARGIN
 */
#define IVM_GETBOTTOMMARGIN     SVM_GETBOTTOMMARGIN
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETVISIBLEWIDTH
 */
#define IVM_GETVISIBLEWIDTH     SVM_GETVISIBLEWIDTH
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETVISIBLEHEIGHT
 */
#define IVM_GETVISIBLEHEIGHT    SVM_GETVISIBLEHEIGHT
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETCONTWIDTH
 */
#define IVM_GETCONTWIDTH        SVM_GETCONTWIDTH
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETCONTHEIGHT
 */
#define IVM_GETCONTHEIGHT       SVM_GETCONTHEIGHT
/**
 * The same common control messages as scrollview control
 * \sa SVM_SETCONTRANGE
 */
#define IVM_SETCONTRANGE        SVM_SETCONTRANGE
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETCONTENTX
 */
#define IVM_GETCONTENTX         SVM_GETCONTENTX
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETCONTENTY
 */
#define IVM_GETCONTENTY         SVM_GETCONTENTY
/**
 * The same common control messages as scrollview control
 * \sa SVM_SETCONTPOS
 */
#define IVM_SETCONTPOS          SVM_SETCONTPOS
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETCURSEL
 */
#define IVM_GETCURSEL           SVM_GETCURSEL
/**
 * The same common control messages as scrollview control
 * \sa SVM_SELECTITEM
 */
#define IVM_SELECTITEM          SVM_SELECTITEM
/**
 * The same common control messages as scrollview control
 * \sa SVM_SHOWITEM
 */
#define IVM_SHOWITEM            SVM_SHOWITEM
/**
 * The same common control messages as scrollview control
 * \sa SVM_CHOOSEITEM
 */
#define IVM_CHOOSEITEM          SVM_CHOOSEITEM
/**
 * The same common control messages as scrollview control
 * \sa SVM_SETCURSEL
 */
#define IVM_SETCURSEL           SVM_SETCURSEL
/**
 * The same common control messages as scrollview control
 * \sa SVM_SETITEMINIT
 */
#define IVM_SETITEMINIT         SVM_SETITEMINIT
/**
 * The same common control messages as scrollview control
 * \sa SVM_SETITEMDESTROY
 */
#define IVM_SETITEMDESTROY      SVM_SETITEMDESTROY
/**
 * The same common control messages as scrollview control
 * \sa SVM_SETITEMCMP
 */
#define IVM_SETITEMCMP          SVM_SETITEMCMP
/**
 * The same common control messages as scrollview control
 * \sa SVM_MAKEPOSVISIBLE
 */
#define IVM_MAKEPOSVISIBLE      SVM_MAKEPOSVISIBLE
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETHSCROLLVAL
 */
#define IVM_GETHSCROLLVAL       SVM_GETHSCROLLVAL
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETVSCROLLVAL
 */
#define IVM_GETVSCROLLVAL       SVM_GETVSCROLLVAL
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETHSCROLLPAGEVAL
 */
#define IVM_GETHSCROLLPAGEVAL   SVM_GETHSCROLLPAGEVAL
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETVSCROLLPAGEVAL
 */
#define IVM_GETVSCROLLPAGEVAL   SVM_GETVSCROLLPAGEVAL
/**
 * The same common control messages as scrollview control
 * \sa SVM_SETSCROLLVAL
 */
#define IVM_SETSCROLLVAL        SVM_SETSCROLLVAL
/**
 * The same common control messages as scrollview control
 * \sa SVM_SETSCROLLPAGEVAL
 */
#define IVM_SETSCROLLPAGEVAL    SVM_SETSCROLLPAGEVAL
/**
 * The same common control messages as scrollview control
 * \sa SVM_SORTITEMS
 */
#define IVM_SORTITEMS           SVM_SORTITEMS
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETITEMCOUNT
 */
#define IVM_GETITEMCOUNT        SVM_GETITEMCOUNT
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETITEMADDDATA
 */
#define IVM_GETITEMADDDATA      SVM_GETITEMADDDATA
/**
 * The same common control messages as scrollview control
 * \sa SVM_SETITEMADDDATA
 */
#define IVM_SETITEMADDDATA      SVM_SETITEMADDDATA
/**
 * The same common control messages as scrollview control
 * \sa SVM_REFRESHITEM
 */
#define IVM_REFRESHITEM         SVM_REFRESHITEM
/**
 * The same common control messages as scrollview control
 * \sa SVM_GETFIRSTVISIBLEITEM
 */
#define IVM_GETFIRSTVISIBLEITEM SVM_GETFIRSTVISIBLEITEM

    /** @} end of mgext_ctrl_iconview_msgs */


    /**
     * \defgroup mgext_ctrl_iconview_ncs Notification code of IconView control
     * @{
     */
/**
 * The notification messages as scrollview control
 * \sa SVN_CLICKED
 */
#define IVN_CLICKED            SVN_CLICKED

/**
 * The  notification messages as scrollview control
 * \sa SVN_SELCHANGED
 */
#define IVN_SELCHANGED         SVN_SELCHANGED


    /** @} end of mgext_ctrl_iconview_ncs */

    /** @} end of mgext_ctrl_iconview */

    /** @} end of controls */

#ifdef  __cplusplus
}
#endif

#endif /* EXT_ICONVIEW_H */

