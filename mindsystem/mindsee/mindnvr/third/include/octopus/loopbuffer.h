#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define DE_BufferLen 256

class LoopBuffer
{
public:
	LoopBuffer()
	{
		buffer = new char[DE_BufferLen];
		bufferLen = DE_BufferLen;
		leftSpaceLen = DE_BufferLen;
		read = write = 0;
		outBuffer = nullptr;
		outBufferLen = 0;
		dataLen = 0;
	}
	~LoopBuffer()
	{
		delete[]buffer;
		buffer = nullptr;
		if (outBuffer)
		{
			delete[]outBuffer;
			outBuffer = nullptr;
		}
	}
public:
	bool insertData(const char* insertData, int insertDataLen)
	{
		if (insertDataLen < leftSpaceLen)
		{
			for (int i = 0; i < insertDataLen; ++i)
			{
				buffer[write++] = insertData[i];
				if (write == bufferLen)
					write = 0;
			}
			leftSpaceLen -= insertDataLen;
			dataLen += insertDataLen;
		}
		else
		{
			getMoreSize(insertDataLen);//申请更大的空间，以插入新的数据
			for (int i = 0; i < insertDataLen; ++i)
			{
				buffer[write++] = insertData[i];
				if (write == bufferLen)
					write = 0;
			}
			leftSpaceLen -= insertDataLen;
			dataLen += insertDataLen;
		}
		return true;
	}
	bool analyzePackage()
	{
		//判断剩余数据能否足够识别包长度
		if (dataLen >= sizeof(unsigned short))
		{
			pacakgeLen = 0;//用来记录下一个数据包的长度
			bool ret = readPackageLen((char*)& pacakgeLen, sizeof(unsigned short));//读下一个数据包的长度
			if (!ret)
				return false;
			if (dataLen >= pacakgeLen)//剩余数组足够组装下一个数据包
			{
				if (pacakgeLen > outBufferLen)//不足够容纳下一个数据包
				{
					if (outBuffer)
					{
						delete[]outBuffer;
					}
					outBuffer = new char[pacakgeLen];
					outBufferLen = pacakgeLen;
				}
				return readData(outBuffer, pacakgeLen);
			}
			else//剩余数组不足够组装下一个数据包
			{
				pacakgeLen = 0;
				return false;
			}
		}
		return false;
	}
private:
	bool readPackageLen(char* dst, int len)
	{
		if (dataLen < len)
			return false;
		int read = this->read;
		while (len-- > 0)
		{
			*dst++ = buffer[read++];
			if (read == bufferLen)
				read = 0;
		}
		return true;
	}
	bool readData(char* dst, int len)
	{
		if (dataLen < len)
			return false;
		dataLen -= len;
		leftSpaceLen += len;
		if (read + len < bufferLen)
		{
			memcpy(dst, buffer + read, len);
			read += len;
			return true;
		}
		while (len-- > 0)
		{
			*dst++ = buffer[read++];
			if (read == bufferLen)
				read = 0;
		}
		return true;
	}

	void getMoreSize(int insertDataLen)
	{
		int moreSize = insertDataLen - leftSpaceLen + 100;
		char* tmp = new char[moreSize + bufferLen];
		copyLeftData(tmp);//拷贝剩余数据
		delete[]buffer;
		buffer = tmp;
		bufferLen += moreSize;
		leftSpaceLen += moreSize;
		read = 0;
		write = dataLen;
	}
	void copyLeftData(char* dst)
	{
		int read = this->read;
		int write = this->write;
		while (read != write)
		{
			*dst++ = buffer[read++];
			if (read == bufferLen)
				read = 0;
		}
	}
private:
	char* buffer;
	int bufferLen;//buffer总长度
	int leftSpaceLen;//buffer剩余空间长度
	int dataLen;//剩余数据长度 （有效数据长度）
	int read;
	int write;
	int outBufferLen;//outBuffer缓冲区长度
public:
	unsigned short pacakgeLen;//解析到的package的长度，analyzePackage()返回true时才有效
	char* outBuffer;
};
