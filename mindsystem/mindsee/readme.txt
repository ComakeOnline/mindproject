1、mindsee: NVR端运行demo，会在/customer目录下生成一个nvr_cfg.json，里面是配置ipc设备的ip等信息

已支持主子码流传输，关键信息就是"url": "192.168.7.101/0"，格式是ip/channel
/customer # cat nvr_cfg.json
{
    "ipc_manager": {
        "ipc_number": 1,
        "ipc": [
            {
                "ip": "192.168.7.101",
                "url": "192.168.7.101/0",
                "name": "ipc000",
                "chn": 0
            },
            {
                "ip": "192.168.7.101",
                "url": "192.168.7.101/1",
                "name": "ipc001",
                "chn": 1
            },
            {
                "ip": "172.19.24.149",
                "url": "172.19.24.149/1",
                "name": "ipc002",
                "chn": 2
            },
            {
                "ip": "172.19.24.149",
                "url": "172.19.24.149/0",
                "name": "ipc003",
                "chn": 3
            }
        ]
    }
}



2、mindbase是基站的简易demo，目前也支持接受主子码流，会将主子码流信息打印出来
	若需要做保存功能，可以在回调里将码流信息保存即可