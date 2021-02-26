#ifndef __HOMEPAGE_H__
#define __HOMEPAGE_H__

#include "minigui_entry.hpp"

#define INVALID_OFFSET (-(9999))

typedef struct{
	int icon_startX;
	int icon_startY;
	int icon_width;
	int icon_height;
	PBITMAP icon_picture;
}st_iconInfo_t;

int UI_IconScreen(void);

#endif
