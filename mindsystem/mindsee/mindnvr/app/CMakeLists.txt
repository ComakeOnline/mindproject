# app CMakeLists.txt

include_directories(config)
include_directories(system)
include_directories(public)
include_directories(devices)
include_directories(ui)
include_directories(ui/inc)
include_directories(ui/thirdparty/include)
include_directories(manager)
include_directories(stream)
include_directories(stream_demo)
include_directories(system/codec)
include_directories(wifi)


add_subdirectory(system)
add_subdirectory(devices)
add_subdirectory(ui)
add_subdirectory(manager)
add_subdirectory(stream_demo)
add_subdirectory(wifi)


aux_source_directory(. DIR_APP_SRCS)
add_executable(nvr_demo ${DIR_APP_SRCS})

target_link_libraries(nvr_demo isystem ipc
ui manager priprotocol wifi
mi_disp 
#mi_hdmi 
mi_vdec mi_divp mi_panel mi_sys mi_common mi_ao mi_ai dl pthread)

#operties(nvr_demo PROPERTIES INSTALL_RPATH "./")
install(TARGETS nvr_demo
        RUNTIME DESTINATION .
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

INSTALL(CODE "execute_process(COMMAND ${CMAKE_STRIP} ${CMAKE_INSTALL_PREFIX}/nvr_demo)")
