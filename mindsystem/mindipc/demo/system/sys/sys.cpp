/*---------------------------------------------------------------- 
sigma star版权所有。
作者：lison.guo
时间：2020.7.10
----------------------------------------------------------------*/

#include "sys.h"
#include "vpe.h"
#include "iostream"
#include "string.h"

using namespace std;

Sys::Sys()
{
	MI_SYS_Init();
}

Sys::~Sys()
{
	MI_SYS_Exit();
}

/*-----------------------------------------------------------------------------
描--述：SYS 模块获取实例的唯一接口函数。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
Sys* Sys::getInstance()
{
	static Sys sys;
	return &sys;
}

/*-----------------------------------------------------------------------------
描--述：绑定MI 模块间的通道和端口
参--数：pstSrcChnPort 指向源通道/端口的结构体的指针；pstDstChnPort 指向目标通道/端口的结构体的指针；
		u32SrcFrmrate 源帧率；u32DstFrmrate 目标帧率
返回值：参数无效，返回-1; 成功，返回0; 失败，返回错误码。
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Sys::bindChnPort(MI_SYS_ChnPort_t *pstSrcChnPort, MI_SYS_ChnPort_t *pstDstChnPort, MI_U32 u32SrcFrmrate,  MI_U32 u32DstFrmrate)
{
	cout << "Call Sys::bindChnPort()." << endl;

	if(NULL == pstSrcChnPort || NULL == pstDstChnPort || 0 == u32SrcFrmrate || 0 == u32DstFrmrate)
	{
		cerr << "Fail to call Sys::bindChnPort(), argument has null or zero value!" << endl;
		return -1;
	}
	
	#if 1	// debug
	cout << "Bind module, src: " << pstSrcChnPort->eModId << pstSrcChnPort->u32ChnId << \
		pstSrcChnPort->u32DevId << pstSrcChnPort->u32PortId << endl;
	cout << "dst: " << pstDstChnPort->eModId << pstDstChnPort->u32ChnId << \
		pstDstChnPort->u32DevId << pstDstChnPort->u32PortId << endl;
	#endif
	
	//MI_S32 MI_SYS_BindChnPort(MI_SYS_ChnPort_t *pstSrcChnPort, MI_SYS_ChnPort_t *pstDstChnPort, MI_U32 u32SrcFrmrate,  MI_U32 u32DstFrmrate);
	MI_S32 s32Ret = 0;
	
	s32Ret = MI_SYS_BindChnPort(pstSrcChnPort, pstDstChnPort, u32SrcFrmrate,  u32DstFrmrate);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_SYS_BindChnPort(), errno = " << s32Ret << endl;
	}

	cout << "Call Sys::bindChnPort() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：绑定MI 模块间的通道和端口
参--数：pstSrcChnPort 指向源通道/端口的结构体的指针；pstDstChnPort 指向目标通道/端口的结构体的指针；
		u32SrcFrmrate 源帧率；u32DstFrmrate 目标帧率
		eBindType ；u32BindParam ；
返回值：参数无效，返回-1; 成功，返回0; 失败，返回错误码。
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Sys::bindChnPort2(MI_SYS_ChnPort_t *pstSrcChnPort, MI_SYS_ChnPort_t *pstDstChnPort, MI_U32 u32SrcFrmrate, MI_U32 u32DstFrmrate, \
							MI_SYS_BindType_e eBindType, MI_U32 u32BindParam)
{
	cout << "Call Sys::bindChnPort2()." << endl;

	if(NULL == pstSrcChnPort || NULL == pstDstChnPort || 0 == u32SrcFrmrate || 0 == u32DstFrmrate || 0 == eBindType)
	{
		cerr << "Fail to call Sys::bindChnPort2(), argument has null or zero value!" << endl;
		return -1;
	}
	
	// MI_S32 bindChnPort2(MI_SYS_ChnPort_t *pstSrcChnPort, MI_SYS_ChnPort_t *pstDstChnPort,MI_U32 u32SrcFrmrate,  MI_U32 u32DstFrmrate, MI_SYS_BindType_e eBindType, MI_U32 u32BindParam);
	MI_S32 s32Ret = 0;
	
	s32Ret = MI_SYS_BindChnPort2(pstSrcChnPort, pstDstChnPort, u32SrcFrmrate, u32DstFrmrate, eBindType, u32BindParam);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_SYS_BindChnPort(), errno = " << s32Ret << endl;
	}

	cout << "Call Sys::bindChnPort() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：解绑MI 模块间的通道和端口
参--数：pstSrcChnPort 指向源通道/端口的结构体的指针；pstDstChnPort 指向目标通道/端口的结构体的指针；
返回值：参数无效，返回-1; 成功，返回0; 失败，返回错误码。
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Sys::unBindChnPort(MI_SYS_ChnPort_t *pstSrcChnPort, MI_SYS_ChnPort_t *pstDstChnPort)
{
	cout << "Call Sys::unBindChnPort()." << endl;

	if(NULL == pstSrcChnPort || NULL == pstDstChnPort)
	{
		cerr << "Fail to call Sys::unBindChnPort(), argument has null or zero value!" << endl;
		return -1;
	}
	
	// MI_S32 MI_SYS_UnBindChnPort(MI_SYS_ChnPort_t *pstSrcChnPort, MI_SYS_ChnPort_t *pstDstChnPort);
	MI_S32 s32Ret = 0;
	
	s32Ret = MI_SYS_UnBindChnPort(pstSrcChnPort, pstDstChnPort);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_SYS_UnBindChnPort(), errno = " << s32Ret << endl;
	}

	cout << "Call Sys::unBindChnPort() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：绑定VIF 到VPE
参--数：u32VifPortID VIF端口号，一般取0; u32VpeChnID VPE通道号，一般取0; u32SrcFrmrate 源帧率；
		u32DstFrmrate 目标帧率；eBindType 绑定类型，用realtime.
返回值：成功，返回0; 失败，返回错误码。
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Sys::bindVif2Vpe(MI_U32 u32VifPortID, MI_U32 u32VpeChnID, MI_U32 u32SrcFrmrate, MI_U32 u32DstFrmrate, MI_SYS_BindType_e eBindType, MI_U32 u32BindParam)
{
	cout << "Call Sys::bindVif2Vpe()." << endl;

	MI_SYS_ChnPort_t stSrcChnPort;
	memset(&stSrcChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stSrcChnPort.eModId = E_MI_MODULE_ID_VIF;
	stSrcChnPort.u32DevId = 0;
	stSrcChnPort.u32ChnId = 0;
	stSrcChnPort.u32PortId = u32VifPortID;

	MI_SYS_ChnPort_t stDstChnPort;
	memset(&stDstChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stDstChnPort.eModId = E_MI_MODULE_ID_VPE;
	stSrcChnPort.u32DevId = 0;
	stSrcChnPort.u32ChnId = u32VpeChnID;
	stSrcChnPort.u32PortId = 0;
	
	//MI_S32 MI_SYS_BindChnPort(MI_SYS_ChnPort_t *pstSrcChnPort, MI_SYS_ChnPort_t *pstDstChnPort, MI_U32 u32SrcFrmrate,  MI_U32 u32DstFrmrate);
	MI_S32 s32Ret = 0;
	s32Ret = MI_SYS_BindChnPort2(&stSrcChnPort, &stDstChnPort, u32SrcFrmrate, u32DstFrmrate, eBindType, u32BindParam);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_SYS_BindChnPort2(), errno = " << s32Ret << endl;
	}

	cout << "Call Sys::bindVif2Vpe() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：绑定VPE 到VENC
参--数：u32VpePortID VPE端口号; u32VencChnID VENC通道号；u32SrcFrmrate 源帧率；u32DstFrmrate 目标帧率。
返回值：成功，返回0; 失败，返回错误码。
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Sys::bindVpe2Venc(MI_U32 u32VpePortID, MI_U32 u32VencChnID, MI_U32 u32SrcFrmrate,  MI_U32 u32DstFrmrate, MI_SYS_BindType_e eBindType, MI_U32 u32BindParam)
{
	cout << "Call Sys::bindVpe2Venc()." << endl;

	MI_SYS_ChnPort_t stSrcChnPort;
	memset(&stSrcChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stSrcChnPort.eModId = E_MI_MODULE_ID_VPE;
	stSrcChnPort.u32DevId = 0;
	stSrcChnPort.u32ChnId = 0;
	stSrcChnPort.u32PortId = u32VpePortID;

	MI_SYS_ChnPort_t stDstChnPort;
	memset(&stDstChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stDstChnPort.eModId = E_MI_MODULE_ID_VENC;
	if(Vpe::vpeJpegPort == u32VpePortID)
	{
		stDstChnPort.u32DevId = 1;
	}
	else
	{
		stDstChnPort.u32DevId = 0;
	}
	stDstChnPort.u32ChnId = u32VencChnID;
	stDstChnPort.u32PortId = 0;

	// MI_S32 MI_SYS_BindChnPort(MI_SYS_ChnPort_t *pstSrcChnPort, MI_SYS_ChnPort_t *pstDstChnPort, MI_U32 u32SrcFrmrate,  MI_U32 u32DstFrmrate);
	MI_S32 s32Ret = 0;	
	//s32Ret = MI_SYS_BindChnPort2(&stSrcChnPort, &stDstChnPort, u32SrcFrmrate, u32DstFrmrate, eBindType, 0);
	s32Ret = MI_SYS_BindChnPort(&stSrcChnPort, &stDstChnPort, u32SrcFrmrate, u32DstFrmrate);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_SYS_BindChnPort2(), errno = " << s32Ret << endl;
	}

	cout << "Call Sys::bindVpe2Venc() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：绑定VDEC 到DISP.
参--数：u32SrcChnID VDEC的通道号；u32DstPortID DISP的port号
返回值：成功，返回0; 失败，返回错误码。
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Sys::bindVdecChn2DispPort(MI_U32 u32SrcChnID, MI_U32 u32DstPortID)
{
	cout << "Call Sys::bindVdecChn2DispPort()." << endl;

	MI_SYS_ChnPort_t stSrcChnPort;
	memset(&stSrcChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stSrcChnPort.eModId = E_MI_MODULE_ID_VDEC;
	stSrcChnPort.u32ChnId = u32SrcChnID;
	stSrcChnPort.u32DevId = 0;
	stSrcChnPort.u32PortId = 0;
	
	MI_SYS_ChnPort_t stDstChnPort;
	memset(&stDstChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stDstChnPort.eModId = E_MI_MODULE_ID_DISP;
	stDstChnPort.u32ChnId = 0;
	stDstChnPort.u32DevId = u32DstPortID;		// 注意，实际绑定的是Chn
	stDstChnPort.u32PortId = 0;
	
	MI_S32 s32Ret = 0;
	MI_U32 u32SrcFrmrate = 30;
	MI_U32 u32DstFrmrate = 30;
	s32Ret = bindChnPort(&stSrcChnPort, &stDstChnPort, u32SrcFrmrate, u32DstFrmrate);
	if(0 != s32Ret)
	{
		cerr << "Fail to call bindChnPort()." << endl;
	}

	cout << "Call Sys::bindVdecChn2DispPort() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：绑定若干VDEC 到DISP.
参--数：u32PortNum 端口数量。
返回值：返回成功绑定的端口数量。
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Sys::bindVdecChn2DispPort_N(MI_U32 u32PortNum)
{
	cout << "Call Sys::bindVdecChn2DispPort_N()." << endl;

	MI_SYS_ChnPort_t stSrcChnPort;
	memset(&stSrcChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stSrcChnPort.eModId = E_MI_MODULE_ID_VDEC;
	
	MI_SYS_ChnPort_t stDstChnPort;
	memset(&stDstChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stDstChnPort.eModId = E_MI_MODULE_ID_DISP;
	
	MI_U32 u32SrcFrmrate = 30;
	MI_U32 u32DstFrmrate = 30;
	
	int i = 0;
	MI_U32 u32BindedPortNum = 0;
	for(i = 0; i < u32PortNum; ++i)
	{
		stSrcChnPort.u32ChnId = i;
		stDstChnPort.u32ChnId = i;	// 按理说应该绑定DISP 的video layer 的port, 然而实际上绑定chn 才对。
		
		MI_S32 s32Ret = 0;
		s32Ret = bindChnPort(&stSrcChnPort, &stDstChnPort, u32SrcFrmrate, u32DstFrmrate);
		if(0 != s32Ret)
		{
			cerr << "Fail to call bindChnPort()." << endl;
		}
		else
		{
			++u32BindedPortNum;
		}
	}

	cout << "Call Sys::bindVdecChn2DispPort_N() end. Success to bind " << u32BindedPortNum << " ports!" << endl;
	return u32BindedPortNum;
}

/*-----------------------------------------------------------------------------
描--述：绑定VDEC 到DIVP.
参--数：u32SrcChnID VDEC的通道号；u32DstChnID DIVP的通道号。
返回值：成功返回0, 失败返回错误码。
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Sys::bindVdecChn2DivpChn(MI_U32 u32SrcChnID, MI_U32 u32DstChnID)
{
	cout << "Call Sys::bindVdecChn2DivpChn()." << endl;
	
	MI_SYS_ChnPort_t stSrcChnPort;
	memset(&stSrcChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stSrcChnPort.eModId = E_MI_MODULE_ID_VDEC;
	stSrcChnPort.u32ChnId = u32SrcChnID;
	stSrcChnPort.u32DevId = 0;
	stSrcChnPort.u32PortId = 0;
	
	MI_SYS_ChnPort_t stDstChnPort;
	memset(&stDstChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stDstChnPort.eModId = E_MI_MODULE_ID_DIVP;
	stDstChnPort.u32ChnId = u32DstChnID;
	stDstChnPort.u32DevId = 0;
	stDstChnPort.u32PortId = 0;
	
	MI_S32 s32Ret = 0;
	MI_U32 u32SrcFrmrate = 30;
	MI_U32 u32DstFrmrate = 30;
	s32Ret = bindChnPort(&stSrcChnPort, &stDstChnPort, u32SrcFrmrate, u32DstFrmrate);
	if(0 != s32Ret)
	{
		cerr << "Fail to call bindChnPort()." << endl;
	}
	
	cout << "Call Sys::bindVdecChn2DivpChn() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：绑定若干VDEC 到DIVP.
参--数：u32ChnNum 通道数量。
返回值：返回成功绑定的通道数量。
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Sys::bindVdecChn2DivpChn_N(MI_U32 u32ChnNum)
{
	cout << "Call Sys::bindVdecChn2DivpChn_N()." << endl;

	MI_SYS_ChnPort_t stSrcChnPort;
	memset(&stSrcChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stSrcChnPort.eModId = E_MI_MODULE_ID_VDEC;
	
	MI_SYS_ChnPort_t stDstChnPort;
	memset(&stDstChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stDstChnPort.eModId = E_MI_MODULE_ID_DIVP;
	
	MI_U32 u32SrcFrmrate = 30;
	MI_U32 u32DstFrmrate = 30;
	
	int i = 0;
	MI_U32 u32BindedChnNum = 0;
	for(i = 0; i < u32ChnNum; ++i)
	{
		stSrcChnPort.u32ChnId = i;
		stDstChnPort.u32ChnId = i;
		
		MI_S32 s32Ret = 0;
		s32Ret = bindChnPort(&stSrcChnPort, &stDstChnPort, u32SrcFrmrate, u32DstFrmrate);
		if(0 != s32Ret)
		{
			cerr << "Fail to call bindChnPort()." << endl;
		}
		else
		{
			++u32BindedChnNum;
		}
	}
	
	cout << "Call Sys::bindVdecChn2DivpChn_N() end. Success to bind " << u32BindedChnNum << " ports!" << endl;
	return u32BindedChnNum;
}

/*-----------------------------------------------------------------------------
描--述：绑定DIVP 到DISP.
参--数：u32SrcChnID DIVP的通道号；u32DstPortID DISP的端口号。
返回值：成功返回0, 失败返回错误码。
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Sys::bindDivpChn2DispPort(MI_U32 u32SrcChnID, MI_U32 u32DstPortID)
{
	cout << "Call Sys::bindDivpChn2DispPort()." << endl;
	
	MI_SYS_ChnPort_t stSrcChnPort;
	memset(&stSrcChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stSrcChnPort.eModId = E_MI_MODULE_ID_DIVP;
	stSrcChnPort.u32ChnId = u32SrcChnID;
	stSrcChnPort.u32DevId = 0;
	stSrcChnPort.u32PortId = 0;
	
	MI_SYS_ChnPort_t stDstChnPort;
	memset(&stDstChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stDstChnPort.eModId = E_MI_MODULE_ID_DISP;
	stDstChnPort.u32ChnId = u32DstPortID;		// 注意，实际绑定的是chn
	stDstChnPort.u32DevId = 0;
	stDstChnPort.u32PortId = 0;
	
	MI_S32 s32Ret = 0;
	MI_U32 u32SrcFrmrate = 30;
	MI_U32 u32DstFrmrate = 30;
	s32Ret = bindChnPort(&stSrcChnPort, &stDstChnPort, u32SrcFrmrate, u32DstFrmrate);
	if(0 != s32Ret)
	{
		cerr << "Fail to call bindChnPort()." << endl;
	}
	
	cout << "Call Sys::bindDivpChn2DispPort() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：绑定若干DIVP 到DISP.
参--数：u32PortNum 端口数量。
返回值：返回成功绑定的端口数量。
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Sys::bindDivpChn2DispPort_N(MI_U32 u32PortNum)
{
	cout << "Call Sys::bindDivpChn2DispPort_N()." << endl;

	MI_SYS_ChnPort_t stSrcChnPort;
	memset(&stSrcChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stSrcChnPort.eModId = E_MI_MODULE_ID_DIVP;
	
	MI_SYS_ChnPort_t stDstChnPort;
	memset(&stDstChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stDstChnPort.eModId = E_MI_MODULE_ID_DISP;
	
	MI_U32 u32SrcFrmrate = 30;
	MI_U32 u32DstFrmrate = 30;
	
	int i = 0;
	MI_U32 u32BindedPortNum = 0;
	for(i = 0; i < u32PortNum; ++i)
	{
		stSrcChnPort.u32ChnId = i;
		stDstChnPort.u32ChnId = i;	// 按理说应该绑定DISP 的video layer 的port, 然而实际上绑定chn 才对。
		
		MI_S32 s32Ret = 0;
		s32Ret = bindChnPort(&stSrcChnPort, &stDstChnPort, u32SrcFrmrate, u32DstFrmrate);
		if(0 != s32Ret)
		{
			cerr << "Fail to call bindChnPort()." << endl;
		}
		else
		{
			++u32BindedPortNum;
		}
	}
	
	cout << "Call Sys::bindDivpChn2DispPort_N() end. Success to bind " << u32BindedPortNum << " ports!" << endl;
	return u32BindedPortNum;
}
