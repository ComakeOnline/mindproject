#include "pcm2aac.h"

using namespace std;

PcmToAac::PcmToAac()
{
	// (1) Open FAAC engine
	hEncoder = faacEncOpen(nSampleRate, nChannels, &nInputSamples, &aacFrameBufMaxSize);
	if (hEncoder == NULL)
	{
		printf("[ERROR] Failed to call faacEncOpen()\n");
		exit(0);
	}
	pcmBufSize = nInputSamples * nPCMBitSize / 8;
	pcmBuf = new BYTE[pcmBufSize];
	aacFrameBuf = new BYTE[aacFrameBufMaxSize];
	// (2.1) Get current encoding configuration
	pConfiguration = faacEncGetCurrentConfiguration(hEncoder);
	pConfiguration->inputFormat = FAAC_INPUT_16BIT;
	// (2.2) Set encoding configuration
	int nRet = faacEncSetConfiguration(hEncoder, pConfiguration);
}
PcmToAac::~PcmToAac()
{
	faacEncClose(hEncoder);
	delete[]pcmBuf;
	delete[]aacFrameBuf;
}
bool PcmToAac::_EncoderAac(int pcmDataLen)
{
	nInputSamples = pcmDataLen / (nPCMBitSize / 8);
	// (3) Encode
	aacFrameLen = faacEncEncode(hEncoder, (int*)pcmBuf, nInputSamples, aacFrameBuf, aacFrameBufMaxSize);
	//fwrite(aacFrameBuf, 1, aacFrameLen, fpOut);
	return aacFrameLen > 0;
}

void PcmToAac::encode(BYTE* insertData, int insertDataLen, function<void(BYTE*, int)> callBackFun)
{
	int left = pcmBufSize - pcmDataLen;//ʣ��left��ô���ֽ����ݲ�����ȥ����
	//printf("pcmBufSize=%d left=%d insertDataLen=%d\n", pcmBufSize, left, insertDataLen);
	while (insertDataLen >= left)
	{
		memcpy(pcmBuf + pcmDataLen, insertData, left);
		insertDataLen -= left;
		insertData += left;
		bool ret = _EncoderAac(pcmBufSize);
		if (ret)
		{
			callBackFun(aacFrameBuf, aacFrameLen);
		}
		left = pcmBufSize;
		pcmDataLen = 0;
	}
	if (insertDataLen > 0)
	{
		memcpy(pcmBuf + pcmDataLen, insertData, insertDataLen);
		pcmDataLen += insertDataLen;
	}
}

