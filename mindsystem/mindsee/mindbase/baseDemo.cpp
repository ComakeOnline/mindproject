#include <iostream>
#include <unistd.h>
#include "stream.hpp"
using namespace std;

int main()
{
    Stream testStream1;
    testStream1.SetCtrlCb([](std::string _cmd, void* userdata){return;}); // the cmd callback function must be set, you can just return without doing anything
    testStream1.SetFrameCb([](Stream::sFrame_t& frame, void* userdata){
        // TODO
        // save frame to file
        // just test here
        printf("the %s Stream send %s data, CODEC typeis %s, size is %d\n",
                "master",
                (Stream::CHANNEL_VIDEO == frame.channel_type)?"video":"other data",
                (Stream::VIDEO_CODEC_H265 == frame.codec_type)?"h265":"other type",
                frame.data.c_str()
        );
    });

    testStream1.Start("172.19.24.95/0"); //the url fotmat: ip/channel

    Stream testStream2;
    testStream2.SetCtrlCb([](std::string _cmd, void* userdata){return;});
    testStream2.SetFrameCb([](Stream::sFrame_t& frame, void* userdata){
        // TODO
        // save frame to file
        // just test here
        printf("the %s Stream send %s data, CODEC typeis %s, size is %d\n",
                "slave",
                (Stream::CHANNEL_VIDEO == frame.channel_type)?"video":"other data",
                (Stream::VIDEO_CODEC_H265 == frame.codec_type)?"h265":"other type",
                frame.data.c_str()
        );
    });

    testStream2.Start("172.19.24.95/1");
    while(true)
    {
        printf("looping...\n");
        sleep(1);
    }

    return 0;
}
