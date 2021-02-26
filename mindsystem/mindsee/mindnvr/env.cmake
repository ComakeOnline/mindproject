message("compile:gcc v8.2 arm-linux-gnueabihf-gcc/g++ -std=c++17 -g")
set(CMAKE_SYSTEM_NAME Linux)

#设置编译器路径
# set(COMPILER_PATH /usr/local/toolchain/gcc-arm-8.2-2018.08-x86_64-arm-linux-gnueabihf/bin)
# set(PRE_FIX arm-linux-gnueabihf-)
# set(CMAKE_FIND_ROOT_PATH ${COMPILER_PATH})
# set(CMAKE_C_COMPILER ${COMPILER_PATH}/${PRE_FIX}gcc)
# set(CMAKE_CXX_COMPILER ${COMPILER_PATH}/${PRE_FIX}g++)
# set(CMAKE_STRIP ${COMPILER_PATH}/${PRE_FIX}strip)

#specify the cross compiler
set(CMAKE_C_COMPILER   /home/yu/workspace/toolchain/gcc-arm-8.2-2018.08-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /home/yu/workspace/toolchain/gcc-arm-8.2-2018.08-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)
set(CMAKE_STRIP /home/yu/workspace/toolchain/gcc-arm-8.2-2018.08-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-strip)

# where is the target environment 
set(CMAKE_FIND_ROOT_PATH  /home/yu/workspace/toolchain/gcc-arm-8.2-2018.08-x86_64-arm-linux-gnueabihf/bin)

# search for programs in the build host directories (not necessary)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories (not necessary)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# set C and CPP compile flag 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -o2 -std=c++17 -fPIC -Wno-psabi")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -o2 -fPIC")

# set install dir
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/build/install)
