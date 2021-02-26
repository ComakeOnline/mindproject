#add_subdirectory(resource)

aux_source_directory(. DIR_SRC_SRCS)
aux_source_directory(src DIR_SRC_SRCS)
add_library(ui ${DIR_SRC_SRCS} ${DIR_RES_SRCS})
include_directories(inc)
include_directories(../../third/include)

#MG4.0.4
#target_link_libraries(ui manager minigui_ths freetype jpeg png ts z)
#target_link_libraries(ui manager mgncs mgutils mgplus minigui_ths freetype harfbuzz jpeg png ts z)
#MG3.0.12
target_link_libraries(ui manager minigui_ths freetype jpeg png ts z)

#install
install(DIRECTORY res DESTINATION . )
