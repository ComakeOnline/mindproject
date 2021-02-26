/*---------------------------------------------------------------- 
sigma star版权所有。
作者：lison.guo
时间：2020.7.10
----------------------------------------------------------------*/

#ifndef __SENSOR_HPP__
#define __SENSOR_HPP__

/* 单例方法：
1.构造函数、析构函数、拷贝构造函数、赋值运算符的重载函数，四个函数声明为私有。
2.其中，拷贝构造函数、赋值运算的重载函数，只声明不定义。
3.实例的唯一接口函数声明为static 类型，public的，返回static 修饰的类对象的指针。
*/

/* Sensor 流程：
MI_SNR_SetPlaneMode -> MI_SNR_SetRes ->	MI_SNR_SetFps/MI_SNR_SetOrien -> MI_SNR_Enable

需要关注：sensor 的FPS.
*/

#include "mi_sensor.h"

class Sensor{
public:
	static Sensor *getInstance(){
		static Sensor sensor;
		return &sensor;
	}

	int getFps(MI_U32 *pFps);
	int setFps(MI_U32 u32Fps);
	int queryResCount(MI_U32 *pu32ResCount);
	int getRes();
	int setRes(MI_U8 u8ResIdx);
	int sensorEnable();
	int sensorDisable();
	int enable(MI_U32 u32Fps = 30);
	int disable();
	int getPadInfo(MI_SNR_PADInfo_t *pstPadInfo);
	int setPadInfo();
	int getPlaneInfo(MI_SNR_PlaneInfo_t *pstPlaneInfo);
	int setPlaneInfo();
	int getPlaneMode();
	int setPlaneMode();
	
private:
	Sensor();
	~Sensor();
	Sensor(const Sensor&);
	Sensor& operator=(const Sensor&);

	MI_SNR_PAD_ID_e ePADId;
	MI_U32 u32PlaneID;
	MI_U32 u32ResCnt;
	bool bEnable;
};

#endif
