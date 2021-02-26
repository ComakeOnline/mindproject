while true
do
    ps -ef | grep "nvr_demo" | grep -v "grep"
    if [ "$?" -eq 1 ]
        then
        ./nvr_demo & #启动应用，修改成自己的启动应用脚本或命令
        echo "process has been restarted!"
        sleep 3
        cd /config/;./riu_r 101e;./riu_r 101e 0d 0c00;./riu_w 101e 0d 0c00;
    else
        echo "process already started!"
    fi
    sleep 15