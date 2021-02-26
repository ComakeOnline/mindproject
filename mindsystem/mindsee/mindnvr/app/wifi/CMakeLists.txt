aux_source_directory(. DIR_SRC_SRCS)
add_library(wifi ${DIR_SRC_SRCS})

target_link_libraries(wifi wifilink)