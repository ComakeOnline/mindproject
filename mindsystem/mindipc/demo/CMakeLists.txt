#设置cmake 最低版本
cmake_minimum_required(VERSION 3.1)

#设置MI_SDK 平台
set(SDK_VERSION 335)

#设置系统平台
set(CMAKE_SYSTEM_NAME Linux)

# set C and CPP compile flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-pointer-arith")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

#设置toolchain
#SET(ToolPath "/opt/arm-buildroot-linux-uclibcgnueabihf-4.9.4/bin")
SET(ToolPath "/usr/local/toolchain/arm-buildroot-linux-uclibcgnueabihf-4.9.4/bin")
SET(CMAKE_C_COMPILER  	${ToolPath}/arm-buildroot-linux-uclibcgnueabihf-gcc)
SET(CMAKE_CXX_COMPILER 	${ToolPath}/arm-buildroot-linux-uclibcgnueabihf-g++)
SET(CMAKE_STRIP 		${ToolPath}/arm-buildroot-linux-uclibcgnueabihf-strip)

#设置头文件搜索目录
include_directories(../mi/include)
include_directories(../mi/include/drivers/cam)
include_directories(../mindsdk/include)
include_directories(system/ircut)
include_directories(../mindapp/system/osd/rgn)
include_directories(../mindapp/system/osd/common)
include_directories(../libfaac/include)
include_directories(../libfaad/include)

#设置库文件搜索目录
link_directories(../mi/dynamic)
link_directories(../mindsdk/lib)
link_directories(wifi/lib/uclibc)
link_directories(../libfaac/lib)
link_directories(../libfaad/lib)

add_subdirectory(aac)
add_subdirectory(aad)
add_subdirectory(wifi)
add_subdirectory(system)
add_subdirectory(testing)

include_directories(
	system/sensor
	system/vif
	system/vpe
	system/venc
	system/sys
	system/audio_ai
	system/audio_ao
	avtp
	aac
	aad
	system/rgn
	system/rgn/st_rgn
	testing
	wifi
	wifi/include
)

link_directories(
	system/sensor
	system/vif
	system/vpe
	system/venc
	system/sys
	system/audio_ai
	system/audio_ao
	avtp
	system/rgn
	system/st_rgn
	testing
	wifi
	wifi/include
)

aux_source_directory(. DIR_SRC)
#设置编译类型
add_executable(mindeye ${DIR_SRC}) # 生成可执行文件

#设置 target 需要链接的库
target_link_libraries(mindeye mi_sys sensor vif vpe venc sys ai ao mindSdk aac aad testing rgn wifi ircut)

