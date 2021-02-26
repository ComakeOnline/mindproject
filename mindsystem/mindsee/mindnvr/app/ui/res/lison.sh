#!/bin/sh

en_mount=1

cd /customer && ls

echo ---------------lison.guo configure NETWORK --------------------------
ifconfig eth0 hw ether 00:11:22:33:44:aa
ifconfig eth0 192.168.0.200
ifconfig eth0 up && ifconfig lo up && ifconfig
sleep 0.1

if [ 1 -eq $en_mount ]; then
echo ------------------- lison.guo MOUNT  -------------------------------
mount -o nolock 192.168.0.7:/home/linux/ /mnt && cd /mnt/ && ls
rm /customer/nvr_demo /customer/res -r
ln -s /mnt/Workspace/mindsoft/mindsee/build/install/nvr_demo /customer/nvr_demo
ln -s /mnt/Workspace/mindsoft/mindsee/build/install/res /customer/res
else
echo ------------------- lison.guo NO MOUNT  -------------------------------
fi

echo ------------------- lison.guo EXPORT  -------------------------------
export MG_CFG_PATH=/customer/res/uicfg
export TSLIB_ROOT=/customer/res/uicfg
export TSLIB_CONFFILE=$TSLIB_ROOT/ts.conf
export TSLIB_PLUGINDIR=$TSLIB_ROOT/ts   
export TSLIB_CALIBFILE=$TSLIB_ROOT/pointercal
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TSLIB_ROOT/
echo MG_CFG_PATH = $MG_CFG_PATH
echo LD_LIBRARY_PATH = $LD_LIBRARY_PATH

echo ------------------- lison.guo RUN  -------------------------------
cd /customer && ls
./nvr_demo  &
