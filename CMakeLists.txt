cmake_minimum_required(VERSION 3.10)
project(CameraApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Append project cmake script directory
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(enum_cli_hdr)
include(enum_cli_src)
include(enum_crsdk_hdr)

### Define output target ###
set(CameraApp "${PROJECT_NAME}")
add_executable(${CameraApp}
    ${cli_hdrs}
    ${cli_srcs}
    ${crsdk_hdrs}
)

set_target_properties(${remotecli} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    BUILD_RPATH "$ORIGIN"
    INSTALL_RPATH "$ORIGIN"
)

## Specify char is signed-char to fix mismatch with Raspbian
target_compile_options(${CameraApp}
    PUBLIC
        -fstack-protector-all
    PRIVATE
        -fsigned-char
)

### Configure external library directories ###
set(ldir ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(cr_ldir ${ldir}/crsdk)

### Link CRSDK library
find_library(camera_remote Cr_Core HINTS ${cr_ldir})
target_link_libraries(${CameraApp}
    PRIVATE
        ${camera_remote}
)

### Windows specific configuration ###
if(WIN32)
    ## Build with unicode on Windows
    target_compile_definitions(${CameraApp} PRIVATE UNICODE _UNICODE)
endif(WIN32)

### Linux specific configuration ###
if(UNIX AND NOT APPLE)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8)
            # Must use std::experimental namespace if older than GCC8
            message("[${PROJECT_NAME}] GCC version less than 8. Using std::experimental namespace.")
            target_compile_definitions(${CameraApp} PRIVATE USE_EXPERIMENTAL_FS)
        endif()

        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
            # Must explicitly link separate std::filesystem if older than GCC9
            message("[${PROJECT_NAME}] GCC version less than 9. Explicitly linking separate std::filesystem library.")
            target_link_libraries(${CameraApp} PRIVATE stdc++fs)
        endif()
    endif()
endif(UNIX AND NOT APPLE)

## Copy required library binaries
if(WIN32)
    add_custom_command(TARGET ${CameraApp} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${cr_ldir} $<TARGET_FILE_DIR:${CameraApp}>
    )
endif(WIN32)

if(UNIX AND NOT APPLE)
    add_custom_command(TARGET ${CameraApp} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${cr_ldir} $<TARGET_FILE_DIR:${CameraApp}>
    )
endif(UNIX AND NOT APPLE)

## Install application
## '.' means, install to the root directory of CMAKE_INSTALL_PREFIX
install(TARGETS ${CameraApp} DESTINATION .)
install(DIRECTORY ${cr_ldir}/ DESTINATION .)