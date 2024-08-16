## Script for enumerating CameraApp header files
set(__cli_hdr_dir ${CMAKE_CURRENT_SOURCE_DIR}/Cameraremote_SDK/include)

### Enumerate CameraApp header files ###
message("[${PROJECT_NAME}] Indexing header files..")
set(__cli_hdrs
    ${__cli_hdr_dir}/CameraDevice.h
    ${__cli_hdr_dir}/ConnectionInfo.h
    ${__cli_hdr_dir}/PropertyValueTable.h
    ${__cli_hdr_dir}/Text.h
    ${__cli_hdr_dir}/MessageDefine.h
)

## Use cli_srcs in project CMakeLists
set(cli_hdrs ${__cli_hdrs})
