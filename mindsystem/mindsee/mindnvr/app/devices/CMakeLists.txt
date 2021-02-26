aux_source_directory(. DIR_SRC_SRCS)
add_library(ipc ${DIR_SRC_SRCS})

target_link_libraries(ipc priprotocol)