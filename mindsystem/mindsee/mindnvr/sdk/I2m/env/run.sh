#!/bin/sh
rm core
ifconfig eth0 up
ifconfig eth0 192.168.1.10

insmod cfg80211.ko

export LD_LIBRARY_PATH=/customer/lib:$LD_LIBRARY_PATH


export PATH=$PATH:/customer/bin
ln -s /bin/busybox /customer/bin/killall

export WIFI_PATH_KO=/customer/
export WIFI_PATH_CONF=/customer/

#./wifilinknvr &
export MG_CFG_PATH=/customer/res/uicfg
export TSLIB_ROOT=/customer/res/uicfg
export TSLIB_CONFFILE=$TSLIB_ROOT/ts.conf
export TSLIB_PLUGINDIR=$TSLIB_ROOT/ts
export TSLIB_CALIBFILE=$TSLIB_ROOT/pointercal
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TSLIB_ROOT/

/bin/date -s "2019-01-01 00:00:00"
./nvr_demo &
sleep 3
ifconfig wlan0 192.168.7.10
route add default dev eth0
route add default gateway 192.168.7.1
iwpriv wlan0 fwcmd set_retry,1,50,4

sleep 3
cd /config/;./riu_r 101e;./riu_r 101e 0d 0c00;./riu_w 101e 0d 0c00;