#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "faac.h"
#include <list>
#include <memory.h>
#include <functional>

class PcmToAac
{
	typedef unsigned long   ULONG;
	typedef unsigned int    UINT;
	typedef unsigned char   BYTE;
	typedef char            _TCHAR;

	ULONG nSampleRate = 16000;
	UINT nChannels = 1;
	UINT nPCMBitSize = 16;
	ULONG nInputSamples = 0;
	

	faacEncHandle hEncoder;
	faacEncConfigurationPtr pConfiguration;

	int nBytesRead;

	int pcmBufSize = 0;//pcm数据缓冲区的空间大小，插入EncoderPcm的pcm数据长度要等于该值
	BYTE* pcmBuf = NULL;//pcm数据
	int pcmDataLen = 0;//插入到pcmBuf的数据长度，要达到pcmBufSize后才能拿去encodeAac
	ULONG aacFrameBufMaxSize = 0;//aac数据缓冲区的空间大小，注意：并不等于一帧aac数据帧的长度，这个只是最大长度

	BYTE* aacFrameBuf = NULL;//编码出来的aac数据
	int aacFrameLen = 0;//编码出来的aac数据帧长度

	bool _EncoderAac(int pcmDataLen);
public:
	//list<string> aacFrameList;//编码出来的aac数据帧最终插入到此list中

public:
	PcmToAac();
	~PcmToAac();
	void encode(BYTE* insertData, int insertDataLen, std::function<void(BYTE*, int)> callBackFun);
};



