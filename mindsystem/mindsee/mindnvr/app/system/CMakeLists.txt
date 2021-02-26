include_directories(codec)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(isystem ${DIR_SYSTEM_SRCS})

add_subdirectory(codec)

if(SDK_I2)
    add_subdirectory(I2)
    include_directories(I2)
    target_link_libraries(isystem i2_sys)
elseif(SDK_I2m)
    add_subdirectory(I2m)
    include_directories(I2m)
    target_link_libraries(isystem i2m_sys)
endif()

target_link_libraries(isystem codec)