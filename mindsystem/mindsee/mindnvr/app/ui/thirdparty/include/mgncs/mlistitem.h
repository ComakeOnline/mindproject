/**
 * \file mlistitem.h
 * \author XiaoweiYan
 * \date 2009/02/24
 *
 * This file includes the definition of mListItem.
 *
 \verbatim

    Copyright (C) 2009 Feynman Software.

    All rights reserved by Feynman Software.

    This file is part of mgncs, which is new control 
    set of MiniGUI.

  	\endverbatim
 */

/**
 * $Id: mlistitem.h 635 2009-10-28 02:19:04Z wangjian $
 *
 *      Copyright (C) 2009 Feynman Software.
 */

#ifndef _MGUI_WIDGET_LSTITM_H
#define _MGUI_WIDGET_LSTITM_H
 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Widget_ListItem mListItem
 * @{
 */

typedef struct _mListItem mListItem;
typedef struct _mListItemClass mListItemClass;

/**
 * \def NCSF_LSTITM_FOLD
 * \brief fold item.
 */
#define NCSF_LSTITM_FOLD          (0x0001<<NCSF_ITMMNG_SHIFT)

/**
 * \def NCSF_LSTITM_PRIVBKCOL
 * \brief background for item.
 */
#define NCSF_LSTITM_PRIVBKCOL     (0x0002<<NCSF_ITMMNG_SHIFT)

/**
 * \def NCSF_LSTITM_PRIVFGCOL
 * \brief foreground for item.
 */
#define NCSF_LSTITM_PRIVFGCOL     (0x0004<<NCSF_ITMMNG_SHIFT)

/**
 * \def NCSF_LSTITM_SHIFT
 * \brief The bits used by mListItem in flags.
 */
#define NCSF_LSTITM_SHIFT         (NCSF_ITMMNG_SHIFT + 3)

#define mListItemHeader(className)  \
	mItemManagerHeader(className)   \
    mListItem *parent;      \
    HICON   foldIcon;       \
    HICON   unfoldIcon;     \
    int     showHeight;     \
    int     depth;          \
    int     bkColor;        \
    int     fgColor;        \
    int     nrChild;

/**
 * \struct mListItem
 * \brief The structure of mListItem. It's used as item by mListView.
 *
 *  - parent\n
 *    The pointer to parent item. \n
 *
 *  - foldIcon\n
 *    The icon used when item is folded.
 *
 *  - unfoldIcon\n
 *    The icon used when item is unfolded.
 *
 *  - showHeight\n
 *    The height when item can be visible.
 *    
 *  - depth\n
 *    The depth.
 *
 *  - bkColor\n
 *    The background color.
 *
 *  - fgColor\n
 *    The foreground color.
 *
 *  - nrChild\n
 *    The number of children.
 */
struct _mListItem
{
	mListItemHeader(mListItem)
};

#define mListItemClassHeader(clsName, superCls)             \
	mItemManagerClassHeader(clsName, superCls)              \
    void (*setBackground)(clsName*, int index, int* color); \
    void (*setForeground)(clsName*, int index, int* color); \
    int (*getBackground)(clsName*, int index, int* color);  \
    int (*getForeground)(clsName*, int index, int* color);  \
    BOOL (*addChild)(clsName*, clsName *child);             \
    BOOL (*delChild)(clsName*, clsName *child);             \
    BOOL (*setFold)(clsName*, BOOL fold);                   \
    mListItem* (*getParent)(clsName*);                      \
    int (*getChildCount)(clsName*);                         \
    int (*getDepth)(clsName*);                              \
    BOOL (*isFold)(clsName*);                               \


/**
 * \struct mListItemClass
 * \brief The virtual function table of mListItem, which derived from
 *        mItemManagerClass.
 *
 * - void (*\b setBackground)(mListItem *self, int index, int* color); \n
 *   Set the background of the specified index item. If the specfied 
 *   index is invalid, it will set the item's background. When color
 *   is NULL, it will cancel previous setting.
 *
 * - void (*\b setForeground)(mListItem *self, int index, int* color); \n
 *   Set the foreground of the specified index item. If the specfied 
 *   index is invalid, it will set the item's background. When color
 *   is NULL, it will cancel previous setting.
 *
 * - int (*\b getBackground)(mListItem *self, int index, int* color); \n
 *   Get the background of the specified index item. If the specified 
 *   index is invalid, it will return the item's background.
 *
 * - int (*\b getForeground)(mListItem *self, int index, int* color); \n
 *   Get the foreground of the specified index item. If the specified 
 *   index is invalid, it will return the item's foreground.
 *
 * - mListItem* (*\b getParent)(mListItem *self);\n
 *   Get the pointer to parent.
 *
 * - int (*\b getChildCount)(mListItem *self);\n
 *   Get the number of children.
 *
 * - int (*\b getDepth)(mListItem *self);\n
 *   Get depth.
 *
 * - BOOL (*\b isFold)(mListItem *self); \n
 *   Get whether folded or not.
 *
 * - BOOL (*\b setFold)(mListItem *self, BOOL fold);\n
 *   Fold item.
 *
 * - BOOL (*\b addChild)(mListItem *self, mListItem *child); \n
 *   Add the specified child.
 *
 * - BOOL (*\b delChild)(mListItem *self, mListItem *child);\n
 *   Delete the specified child.
 */
struct _mListItemClass
{
	mListItemClassHeader(mListItem, mItemManager)
};

/**
 * \enum mListItemProp
 * \brief The properties id of mListItem.
 */
enum mListItemProp
{
    /**
     * The number of children.
     */
    NCSP_LSTITM_NRCHILD = NCSP_ITMMNG_MAX + 1,

    /**
     * The depth.
     */
    NCSP_LSTITM_DEPTH,

    /**
     * The height when item can be visible.
     */
    NCSP_LSTITM_RHEIGHT,

    /**
     * The maximum value of listitem properties id.
     */
	NCSP_LSTITM_MAX,
};

/**
 * \var g_stmListItemCls
 * \brief The instance of mListItemClass.
 *
 * \sa mListItemClass
 */
MGNCS_EXPORT extern mListItemClass g_stmListItemCls;

    /** @} end of Widget_ListItem */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_WIDGET_LSTITM_H */

