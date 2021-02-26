#pragma once
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <functional>
#include"faad.h"
//using namespace std;

#define FRAME_MAX_LEN  1024*5
#define BUFFER_MAX_LEN  1024*1024

class AacToPcm
{
	typedef unsigned long   ULONG;
	typedef unsigned int    UINT;
	typedef unsigned char   BYTE;
	typedef char            _TCHAR;

	NeAACDecHandle decoder = 0;
	unsigned long samplerate = 0;
	unsigned char channels = 0;
	NeAACDecFrameInfo frame_info;
	//unsigned char aacFrame[FRAME_MAX_LEN];
	unsigned char aacFrame_mono[FRAME_MAX_LEN];
	//LoopBuffer loopBuffer;
	bool hasInitDecoder = false;

	bool _DecodeAac(unsigned char* aacData, int aacDataLen, unsigned char*& outPcmData, int& outPcmLen);
public:
	AacToPcm();
	~AacToPcm();
	//initialize decoder
	bool initDecoder(unsigned char* aacData, int aacDataLen);
	void decode(unsigned char* insertData, int insertDataLen, std::function<void(BYTE*, int)> callBackFun);
};
