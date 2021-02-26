
aux_source_directory(. DIR_CODEC_SRCS)
add_library(codec ${DIR_CODEC_SRCS})

target_link_libraries(codec m pthread dl faac faad octopus)
