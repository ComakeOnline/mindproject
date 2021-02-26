cmake_minimum_required(VERSION 3.0)

PROJECT(NVRDEMO)

# 执行git命令，并把结果重定向到自定义变量（此处是我的变量<GIT_VERSION>）。
execute_process(
	COMMAND	git log -1 --format=%H 		#git 命令
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} 
	OUTPUT_VARIABLE GIT_VERSION
)
execute_process(
	COMMAND git remote -v 				#git 命令
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} 
	OUTPUT_VARIABLE GIT_REMOTE_PATH
)

# 对得到的结果进行处理，尤其注意 \n,\t,\r之类的特殊字符，在cmake时没问题，但是生成的.cmake文件有问题，导致make出错。
#string (REGEX REPLACE ".*\t| .*" "" GIT_PATH ${GIT_REMOTE_PATH})
#string (REGEX REPLACE "[\n\t\r]" "" GIT_VERSION ${GIT_VERSION})

# 增加编译选项，把宏导入源码
add_definitions( -DGIT_VERSION=\"${GIT_VERSION}\")
add_definitions( -DGIT_PATH=\"${GIT_PATH}\")

if(ENV_FILE)
    include(${ENV_FILE})
else()
    include(env.cmake)
endif()
add_definitions(-DDISABLE_HDMI)
add_definitions(-DENABLE_MI_I2M)
add_definitions(-DENABLE_TOUCH)
set(SDK_I2m yes)


#[[		// lison: if use 621
include_directories(sdk/I2m/include)
link_directories(sdk/I2m/dynamic)
link_directories(third/I2m/lib)
]]

#		// lison: if use 202
include_directories(sdk/P2/include)
link_directories(sdk/P2/dynamic)
link_directories(third/P2/lib)
#		// lison: endif

link_directories(app/ui/thirdparty/lib)
link_directories(third/lib)

include_directories(third/include)
# include_directories(guilite)
# include_directories(third/live555/include/BasicUsageEnvironment)
# include_directories(third/live555/include/groupsock)
# include_directories(third/live555/include/liveMedia)
# include_directories(third/live555/include/UsageEnvironment)
include_directories(third/include/faac)
include_directories(third/include/faad)
include_directories(third/include/octopus)



#link_directories(third/live555/lib)


add_subdirectory(app)

#install
install(PROGRAMS ${PROJECT_SOURCE_DIR}/sdk/I2m/env/run.sh DESTINATION .)
install(PROGRAMS ${PROJECT_SOURCE_DIR}/sdk/I2m/env/nvr.sh DESTINATION .)
install(PROGRAMS ${PROJECT_SOURCE_DIR}/sdk/I2m/env/gdb_run.sh DESTINATION .)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/sdk/I2m/env/bin DESTINATION . 
PATTERN "*"
PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
        GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/sdk/I2m/env/lib DESTINATION .
PATTERN "*"
PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
        GROUP_READ WORLD_READ)
install(PROGRAMS ${PROJECT_SOURCE_DIR}/sdk/I2m/env/clock DESTINATION .)
install(FILES ${PROJECT_SOURCE_DIR}/sdk/I2m/env/hostapd.conf DESTINATION .)
install(FILES ${PROJECT_SOURCE_DIR}/sdk/I2m/env/ssw101_wifi_usb.ko DESTINATION .)
install(FILES ${PROJECT_SOURCE_DIR}/sdk/I2m/env/cfg80211.ko DESTINATION .)
# install(FILES ${PROJECT_SOURCE_DIR}/third/I2m/lib/libOnvif.so DESTINATION .)

# ADD_EXECUTABLE(HelloParticle
# 			main.cpp
#             std_io.cpp)

#cmake -D CMAKE_C_COMPILER="/home/yu/workspace/toolchain/arm-linux-gnueabihf-4.8.3-201404/bin/arm-linux-gnueabihf-gcc" -D CMAKE_CXX_COMPILER="/home/yu/workspace/toolchain/arm-linux-gnueabihf-4.8.3-201404/bin/arm-linux-gnueabihf-g++" -D TARGET_ARCH="ARM" ..

#export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
