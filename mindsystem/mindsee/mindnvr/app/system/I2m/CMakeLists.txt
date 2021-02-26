aux_source_directory(. DIR_I2M_SRCS)
add_library(i2m_sys ${DIR_I2M_SRCS})

target_link_libraries(i2m_sys pthread)