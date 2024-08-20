## Script for enumerating CameraApp source files
set(__cli_src_dir ${CMAKE_CURRENT_SOURCE_DIR}/Cameraremote_SDK/include)

### Enumerate CameraApp source files ###
message("[${PROJECT_NAME}] Indexing source files..")
set(__cli_srcs
    main.cpp
    ${__cli_src_dir}/CameraDevice.cpp
    ${__cli_src_dir}/ConnectionInfo.cpp
    ${__cli_src_dir}/PropertyValueTable.cpp
    ${__cli_src_dir}/Text.cpp
    ${__cli_src_dir}/MessageDefine.cpp
)

## Use cli_srcs in project CMakeLists
set(cli_srcs ${__cli_srcs})