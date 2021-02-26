#include "ircut.h"

static void setGpioValue(char *port, char *value)
{
	char com[100] = {0};
	sprintf(com, "echo %s > /sys/class/gpio/export", port);
	system(com);

	sprintf(com, "echo out > /sys/class/gpio/gpio%s/direction", port);
	system(com);

	sprintf(com, "echo %s >  /sys/class/gpio/gpio%s/value", value, port);
	system(com);

	sprintf(com, "echo %s > /sys/class/gpio/unexport", port);
	system(com);
}

IrCut::IrCut()
{

}

IrCut* IrCut::getInstance()//获取GPIO对象指针
{
	static IrCut irCut;
	return &irCut;
}

void IrCut::setGpio(int aInPoint, int bInPoint)
{
	IrCut_AIN_Point = aInPoint;
	IrCut_BIN_Point = bInPoint;
}

int IrCut::getStatus()//获取滤波片的开关状态
{
	return status;
}

void IrCut::openFilter()//打开滤波片  晚上要打开
{
	char pointA[10] = "";
	sprintf(pointA, "%d", IrCut_AIN_Point);
	setGpioValue(pointA, "1");
	usleep(400000);
	setGpioValue(pointA, "0");
	if(-1 != status)
		status = 1;
}

void IrCut::closeFilter()//关闭滤波片 白天要关闭
{
	char pointA[10] = "";
	sprintf(pointA, "%d", IrCut_AIN_Point);
	char pointB[10] = "";
	sprintf(pointB, "%d", IrCut_BIN_Point);
	setGpioValue(pointA, "0");
	setGpioValue(pointB, "1");
	usleep(400000);
	setGpioValue(pointB, "0");
	if(-1 != status)
		status = 0;
}

void IrCut::resetFilter()//刚开机的时候，打开并关闭滤波片
{
	openFilter();
	closeFilter();
	status = 0;
}