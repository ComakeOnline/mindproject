aux_source_directory(. DIR_SRC_SRCS)
add_library(manager ${DIR_SRC_SRCS})

target_link_libraries(manager isystem ipc)
target_link_libraries(manager isystem ipc ui)
