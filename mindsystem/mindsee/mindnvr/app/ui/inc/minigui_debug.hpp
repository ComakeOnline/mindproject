#ifndef __MINIGUI_DEBUG_H__
#define __MINIGUI_DEBUG_H__

#include <sys/time.h>

int enableTimeTest();
void disableTimeTest();
suseconds_t getIntervalUs();
suseconds_t getIntervalMs();
suseconds_t getIntervalSec();

#endif
