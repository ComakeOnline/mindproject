# 编译和运行说明

## 编译I2m项目(yume.liu)
1. 复制根目录env.cmake文件到env目录下(env/*.cmake文件不会被git收录)
```shell
cp env.cmake env/i2m.cmake #名字可自定义
```
2. 修改env/i2m.cmake内gcc-8.2.1的交叉编译工具链的路径为你的电脑环境里工具链路径,并且修改安装目录和其他参数。
3. 在根目录创建build文件夹执行cmake和make命令如下
```shell
mkdir build
cd build
cmake .. -DENV_FILE=./env/i2m.cmake
make clean;make install
```
4. 在你指定的安装目录获取目标程序和依赖文件。

#运行说明(lison.guo)
1.将编译好的二进制文件nvr_demo 放在开发板的/customer 下；
	如果文件体积太大，使用如下命令剪裁：
	arm-linux-gnueabihf-strip ./nvr_demo
2.将mindsee/ui/res 文件夹拷贝（软连接也可以）到开发板的/customer 下;
	res 是minigui 运行时所需的资源文件，必须有。
3.在开发板上导入环境变量：
export TSLIB_ROOT=/customer/res/uicfg
export TSLIB_CONFFILE=$TSLIB_ROOT/ts.conf
export TSLIB_PLUGINDIR=$TSLIB_ROOT/ts
export TSLIB_CALIBFILE=$TSLIB_ROOT/pointercal
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TSLIB_ROOT/
这些环境变量是触摸校准所依赖的环境，必须指定。
4.运行程序：
	cd /customer && ./nvr_demo
