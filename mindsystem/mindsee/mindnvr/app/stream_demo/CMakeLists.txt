aux_source_directory(. DIR_SRC_SRCS)
add_library(priprotocol ${DIR_SRC_SRCS})

target_link_libraries(priprotocol codec stream)