/*
用于Debug 阶段的定时器模块。
通过enableTimeTest() 和disableTimeTest() 函数启用和关闭定时器。
通过调用getIntervalUs(), getIntervalMs() 和 getIntervalSec()获取距离上次调用所经过的时长，
精度分别为微秒、毫秒、秒级别，结果取整，非四舍五入。
*/

#include "minigui_debug.hpp"
#include <stdio.h>
#include <string.h>

// 模块内使用的变量以static 修饰，实现封装后的隐蔽性。
static int __EN_TEST = 0;
static struct timeval st_time_last = {0};
static struct timeval st_time_curr = {0};

/*-----------------------------------------------------------------------------
描   述：使能时间测试模块，并记录当前系统时间。		
参数：无
返回值：成功返回0，失败返回-1, 这种情况下可以检测errno 获取错误信息。
-----------------------------------------------------------------------------*/
int enableTimeTest()
{
	int ret = 0;
	
	ret = gettimeofday(&st_time_last, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "In enableTimeTest(), fail to call gettimeofday().\n");
		return -1;
	}
	
	__EN_TEST = 1;

	return 0;
}

/*-----------------------------------------------------------------------------
描   述：禁用时间测试模块，清空计时器。
参数：无
返回值：无。
-----------------------------------------------------------------------------*/
void disableTimeTest()
{
	__EN_TEST = 0;

	memset(&st_time_last, 0, sizeof(struct timeval));
	memset(&st_time_curr, 0, sizeof(struct timeval));

	return;
}

/*-----------------------------------------------------------------------------
描   述：返回本次调用和上次调用之间的时间差，微秒级别。
参数：
返回值：成功返回时间差，失败返回-1.
注意：返回值取整，非四舍五入。
-----------------------------------------------------------------------------*/
suseconds_t getIntervalUs()
{
	int ret = 0;
	
	ret = gettimeofday(&st_time_curr, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "In getIntervalUs(), fail to call gettimeofday().\n");
		return -1;
	}

	suseconds_t sec = 0;
	suseconds_t us = 0;
	suseconds_t val = 0;
	sec = st_time_curr.tv_sec - st_time_last.tv_sec;
	us = st_time_curr.tv_usec - st_time_last.tv_usec;
	val = sec * 1000 * 1000 + us;

	memcpy(&st_time_last, &st_time_curr, sizeof(struct timeval));
	
	return val;
	
}

/*-----------------------------------------------------------------------------
描   述：获取本次调用和上次调用之间的时间差，毫秒级别。
参数：
返回值：成功返回时间差，失败返回-1.
注意：返回值取整，非四舍五入。
-----------------------------------------------------------------------------*/
suseconds_t getIntervalMs()
{
	int ret = 0;
	
	ret = gettimeofday(&st_time_curr, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "In getIntervalUs(), fail to call gettimeofday().\n");
		return -1;
	}

	suseconds_t sec = 0;
	suseconds_t us = 0;
	suseconds_t val = 0;
	sec = st_time_curr.tv_sec - st_time_last.tv_sec;
	us = st_time_curr.tv_usec - st_time_last.tv_usec;
	val = sec * 1000 + us / 1000;

	memcpy(&st_time_last, &st_time_curr, sizeof(struct timeval));
	
	return val;
	
}

/*-----------------------------------------------------------------------------
描   述：获取本次调用和上次调用之间的时间差，秒级别。
参数：
返回值：成功返回时间差，失败返回-1.
注意：返回值取整，非四舍五入。
-----------------------------------------------------------------------------*/
suseconds_t getIntervalSec()
{
	int ret = 0;
	
	ret = gettimeofday(&st_time_curr, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "In getIntervalUs(), fail to call gettimeofday().\n");
		return -1;
	}

	suseconds_t sec = 0;
	suseconds_t us = 0;
	suseconds_t val = 0;
	sec = st_time_curr.tv_sec - st_time_last.tv_sec;
	us = st_time_curr.tv_usec - st_time_last.tv_usec;
	val = sec;

	memcpy(&st_time_last, &st_time_curr, sizeof(struct timeval));
	
	return val;
	
}

