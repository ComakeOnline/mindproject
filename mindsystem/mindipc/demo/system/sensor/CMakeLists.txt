aux_source_directory(. DIR_APP_SRC)

add_library(sensor ${DIR_APP_SRC})

target_link_libraries(sensor mi_sensor)

