#include "aac2pcm.h"
#include "global.hpp"
using namespace std;


AacToPcm::AacToPcm()
{
	//open decoder
	decoder = NeAACDecOpen();
	//防止采样频率加倍
#if 1
	NeAACDecConfigurationPtr conf = NeAACDecGetCurrentConfiguration(decoder);
	conf->dontUpSampleImplicitSBR = 1;
	NeAACDecSetConfiguration(decoder, conf);
#endif
}

AacToPcm::~AacToPcm()
{
	NeAACDecClose(decoder);
}

bool AacToPcm::initDecoder(unsigned char* aacData, int aacDataLen)
{
	NeAACDecInit(decoder, aacData, aacDataLen, &samplerate, &channels);
	return true;
}

void AacToPcm::decode(unsigned char* aacData, int aacDataLen, function<void(BYTE*, int)> callBackFun)
{
	unsigned char* outPcmData = NULL;
	int outPcmDataLen = 0;
	bool ret = _DecodeAac(aacData, aacDataLen, outPcmData, outPcmDataLen);
	if (ret)
	{
		callBackFun(outPcmData, outPcmDataLen);
	}
}

bool AacToPcm::_DecodeAac(unsigned char* aacData, int aacDataLen, unsigned char*& outPcmData, int& outPcmLen)
{
	outPcmData = NULL;
	outPcmLen = 0;
	if (!hasInitDecoder)
	{
		hasInitDecoder = initDecoder(aacData, aacDataLen);
		if (!hasInitDecoder)
		{
			//PrintError();
			LOGGER_ERROR("AAC InitDecoder ERROR");
			exit(0);
			return false;
		}
	}
	unsigned char* pcm_data = (unsigned char*)NeAACDecDecode(decoder, &frame_info, aacData, aacDataLen);
	if (frame_info.error > 0)
	{
		printf("%s\n", NeAACDecGetErrorMessage(frame_info.error));
		return false;
	}
	else if (pcm_data && frame_info.samples > 0)
	{
#if 0
		printf("frame info: bytesconsumed %ld, channels %d, header_type %d\
			             object_type %d, samples %ld, samplerate %ld\n",
			frame_info.bytesconsumed,
			frame_info.channels, frame_info.header_type,
			frame_info.object_type, frame_info.samples,
			frame_info.samplerate);
#endif
		//从双声道的数据中提取单通道
		for (int i = 0, j = 0; i < 4096 && j < 2048; i += 4, j += 2)
		{
			aacFrame_mono[j] = pcm_data[i];
			aacFrame_mono[j + 1] = pcm_data[i + 1];
		}
		outPcmData = aacFrame_mono;
		outPcmLen = frame_info.samples;
		return true;
	}
	return false;
}
