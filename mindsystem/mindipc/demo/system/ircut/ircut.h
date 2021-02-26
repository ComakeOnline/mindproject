/*
*
*此文件用于控制IrCut的滤波片打开或关闭   Jerry.Zhu 2020-08-19
*
*/

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

class IrCut
{
	//IrCut的两个GPIO引脚编号，如果默认值与实际不符，请调用setGpio设置
	int IrCut_AIN_Point = 78;
	int IrCut_BIN_Point = 79;
	int status = -1;//滤波片状态，0为滤波片关闭，1为打开，-1为未知状态，需要开机的时候调用resetFilter复位滤波片为关闭状态后，status的状态值才有效
	IrCut();
public:
	static IrCut* getInstance();//获取GPIO对象指针
	int getStatus();//获取滤波片的开关状态
	void setGpio(int aInPoint, int bInPoint);//设置gpio引脚
	void openFilter();//打开滤波片  晚上要打开
	void closeFilter();//关闭滤波片 白天要关闭
	void resetFilter();//刚开机的时候需要调用此函数，以复位滤波片为关闭状态
};