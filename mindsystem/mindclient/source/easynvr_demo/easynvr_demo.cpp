// easynvr_demo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stream.hpp"
#include "opencv2/opencv.hpp"

extern "C"
{
	#include "libavcodec/avcodec.h"
}

#include <iostream>
#include <thread>


using namespace std;

#pragma comment(lib,"opencv_world400.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"stream.lib")

//编解码变量
AVCodecContext* videoDecoder;
AVPacket* pkt;
AVFrame* s_frame;
char* YuvBuf;

int main(int argc, char** argv)
{
	//初始化变量
	pkt = av_packet_alloc();
	if (!pkt)
	{
		printf("av_packet_alloc failed\n");
		exit(1);
	}
	s_frame = av_frame_alloc();
	if (!s_frame) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
	}
	YuvBuf = new char[4 * 1000 * 1000];

	//初始化h265解码
	static auto codec = avcodec_find_decoder(AV_CODEC_ID_H265);
	if (!codec) {
		fprintf(stderr, "Codec not found\n");
		exit(1);
	}
	videoDecoder = avcodec_alloc_context3(codec);
	if (!videoDecoder) {
		fprintf(stderr, "Could not allocate video codec context\n");
		exit(1);
	}
	/* open it */
	if (avcodec_open2(videoDecoder, codec, NULL) < 0) {
		fprintf(stderr, "Could not open codec\n");
		exit(1);
	}
	cout << "init:" << videoDecoder << endl;

	//获取ip
	string ip;
	if (argc == 2) {
		ip = string{ argv[1] };
	}
	else
	{
		cout << "./easynvr <ipaddr> \nex: ./easynvr 192.168.1.101\n";
		cout << "输入ip地址:\n" << endl;
		cin >> ip;
	}
	//创建连接对象
	Stream pri;
	pri.SetUserdata(nullptr);

	//设置帧处理回调
	pri.SetFrameCb([](Stream::sFrame_t& frame, void* userdata) {
		/*
			* 本demo为了方便演示，所以解码和显示都在此线程中进行，但这是错误的。
			* 在该线程中进行耗时的任何操作(例如解码和显示)都会影响到数据的收发。
			* 所以正常使用时，不应在此线程中进行任何耗时的操作。解码与显示应使用其他线程完成。
		*/
		if (1)
		{
			//该demo只解码h265,可以根据自己需求修改
			if (frame.codec_type == Stream::CODEC_TYPE::VIDEO_CODEC_H265)
			{
				//解码
				pkt->data = (uint8_t*)frame.data.c_str();
				pkt->size = frame.data.size();
				auto ret = avcodec_send_packet(videoDecoder, pkt);
				if (ret != 0)
				{
					printf("%d,ret = %d ,%s %d avcodec_send_packet fail\n", AVERROR(EINVAL), ret, __func__, __LINE__);
					return;
				}
				if (avcodec_receive_frame(videoDecoder, s_frame))
				{
					printf("%s %d avcodec_receive_frame fail\n", __func__, __LINE__);
					return;
				}
				//yuv420p 处理和显示
				auto m_YuvBuf = YuvBuf;
				int a = 0, i;
				for (i = 0; i < s_frame->height; i++)
				{
					memcpy(m_YuvBuf + a, s_frame->data[0] + i * s_frame->linesize[0], s_frame->width);
					a += s_frame->width;
				}
				for (i = 0; i < s_frame->height / 2; i++)
				{
					memcpy(m_YuvBuf + a, s_frame->data[1] + i * s_frame->linesize[1], s_frame->width / 2);
					a += s_frame->width / 2;
				}
				for (i = 0; i < s_frame->height / 2; i++)
				{
					memcpy(m_YuvBuf + a, s_frame->data[2] + i * s_frame->linesize[2], s_frame->width / 2);
					a += s_frame->width / 2;
				}
				//cout << frame->width<< "," << frame->height <<" size="<<a<< endl;
				//cv::Mat yuv{ cv::Size{frame->width,frame->height},CV_8UC3,m_YuvBuf };
				cv::Mat yuvImg;
				yuvImg.create(s_frame->height * 3 / 2, s_frame->width, CV_8UC1);
				memcpy(yuvImg.data, m_YuvBuf, a * sizeof(unsigned char));
				cv::Mat rgbimg;
				cv::cvtColor(yuvImg, rgbimg, cv::COLOR_YUV2BGR_I420);
				cv::namedWindow("camera", cv::WINDOW_NORMAL);
				cv::imshow("camera", rgbimg);
				cv::waitKey(10);
			}
			if (frame.codec_type == Stream::CODEC_TYPE::VIDEO_CODEC_H264)
			{
				cout << "h264 decoder empty" << endl;
			}
		}
	});

	//设置消息处理回调
	pri.SetCtrlCb([](std::string _cmd, void* userdata) {return; });
	pri.Start(ip);
	while (true)
	{
		if (!pri.Status())
		{
			pri.Stop();
			pri.SetUserdata(nullptr);
			pri.Start(ip);
		}
		//pri.SendCmd("cmd_test_1");
		//printf("send cmd_test_1\n");
		this_thread::sleep_for(chrono::seconds(1));
	}

	//释放
	avcodec_free_context(&videoDecoder);
	av_frame_free(&s_frame);
	av_packet_free(&pkt);
	delete[] YuvBuf;
}