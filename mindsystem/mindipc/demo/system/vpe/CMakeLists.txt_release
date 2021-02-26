aux_source_directory(. DIR_APP_SRC)

add_library(vpe ${DIR_APP_SRC})

include_directories(
	../sensor
)

if(325 STREQUAL ${SDK_VERSION})
target_link_libraries(vpe mi_vpe mi_isp cus3a)
elseif (333 STREQUAL ${SDK_VERSION} OR 335 STREQUAL ${SDK_VERSION} OR 337 STREQUAL ${SDK_VERSION})
target_link_libraries(vpe mi_vpe mi_isp cus3a ispalgo cam_os_wrapper)
endif()

