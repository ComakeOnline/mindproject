/**
 * \file control.h
 * \author Wei Yongming <ymwei@minigui.org>
 * \date 2001/12/29
 * 
 * This file includes interfaces of standard controls of MiniGUI.
 *
 \verbatim

    Copyright (C) 2002-2012 FMSoft.
    Copyright (C) 1998-2002 Wei Yongming.

    All rights reserved by FMSoft (http://www.fmsoft.cn).

    This file is part of MiniGUI, a compact cross-platform Graphics 
    User Interface (GUI) support system for real-time embedded systems.

 \endverbatim
 */

/*
 * $Id: control.h 13822 2012-01-15 10:56:34Z weiym $
 *
 *             MiniGUI for Linux/uClinux, eCos, uC/OS-II, VxWorks, 
 *                     pSOS, ThreadX, NuCleus, OSE, and Win32.
 *
 *             Copyright (C) 2002-2012 FMSoft.
 *             Copyright (C) 1999-2002 Wei Yongming.
 */

#ifndef _MGUI_CONTROL_H
#define _MGUI_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "ctrl/ctrlhelper.h"

    /**
     * \defgroup controls Standard controls
     * @{
     */

/* NOTE: control messages start from 0xF000 to 0xFFFF */
#define MSG_FIRSTCTRLMSG    0xF000
#define MSG_LASTCTRLMSG     0xFFFF

    /** @} end of controls */

#ifdef _MGCTRL_STATIC
#include "ctrl/static.h"
#endif /* _MGCTRL_STATIC */

#ifdef _MGCTRL_BUTTON
#include "ctrl/button.h"
#endif /* _MGCTRL_BUTTON */

#if defined(_MGCTRL_SLEDIT) || defined(_MGCTRL_MLEDIT)
#include "ctrl/edit.h"
#endif /* _MGCTRL_EDIT || _MGCTRL_MEDIT */

#ifdef _MGCTRL_PROGRESSBAR
#include "ctrl/progressbar.h"
#endif /* _MGCTRL_PROGRESSBAR */

#ifdef _MGCTRL_LISTBOX
#include "ctrl/listbox.h"
#endif /* _MGCTRL_LISTBOX */

#ifdef _MGCTRL_PROPSHEET
#include "ctrl/propsheet.h"
#endif /* _MGCTRL_PROPSHEET*/

#ifdef _MGCTRL_COMBOBOX
#include "ctrl/combobox.h"
#endif /* _MGCTRL_COMBOBOX */

#ifdef _MGCTRL_NEWTOOLBAR
#include "ctrl/newtoolbar.h"
#endif /* _MGCTRL_NEWTOOLBAR */

#ifdef _MGCTRL_MENUBUTTON
#include "ctrl/menubutton.h"
#endif /* _MGCTRL_MENUBUTTON */

#ifdef _MGCTRL_TRACKBAR
#include "ctrl/trackbar.h"
#endif /* _MGCTRL_TRACKBAR */

#ifdef _MGCTRL_SCROLLVIEW
#include "ctrl/scrollview.h"
#endif /* _MGCTRL_SCROLLVIEW */

#ifdef _MGCTRL_SCROLLBAR
#include "ctrl/scrollbar.h"
#endif /* _MGCTRL_SCROLLBAR */

#ifdef _MGCTRL_TEXTEDIT
#include "ctrl/textedit.h"
#endif /* _MGCTRL_TEXTEDIT */

#ifdef _MGCTRL_SPINBOX
#include "ctrl/spinbox.h"
#endif /* _MGCTRL_SPINBOX */

#ifdef _MGCTRL_MONTHCAL
#include "ctrl/monthcal.h"
#endif /* _MGCTRL_MONTHCAL */

#ifdef _MGCTRL_COOLBAR
#include "ctrl/coolbar.h"
#endif /* _CTRL_COOLBAR */

#ifdef _MGCTRL_LISTVIEW
#include "ctrl/listview.h"
#endif  /* _MGCTRL_LISTVIEW */

#ifdef _MGCTRL_TREEVIEW
#include "ctrl/treeview.h"
#endif /* _MGCTRL_TREEVIEW */

#ifdef _MGCTRL_GRIDVIEW
#include "ctrl/gridview.h"
#endif /* _MGCTRL_GRIDVIEW */

#ifdef _MGCTRL_ICONVIEW
#include "ctrl/iconview.h"
#endif  /* _MGCTRL_ICONVIEW */

#ifdef _MGCTRL_ANIMATION
#include "ctrl/animation.h"
#endif  /* _MGCTRL_ANIMATION */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_CONTROL_H */

