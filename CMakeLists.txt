cmake_minimum_required(VERSION 3.11)
project(sqdiscord-squirrel)

# Options
option(FORCE_32BIT_BIN "Create a 32-bit executable binary if the compiler defaults to 64-bit." OFF)
option(FORCE_DEBUG_BIN "Create debug binary." OFF)
option(USE_LOCAL_DPP "Use local copy of DPP instead of downloading" ON)

# Debug vs Release
if(FORCE_DEBUG_BIN)
    set(CMAKE_BUILD_TYPE Debug)
    add_definitions(-D_DEBUG)
    message("Configuring Debug builds.")
else()
    set(CMAKE_BUILD_TYPE Release)
    message("Configuring Release builds.")
endif()

# Force 32-bit flags
if(FORCE_32BIT_BIN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")
    message("FORCING 32 BIT BINARIES")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m64")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m64")
    message("FORCING 64 BIT BINARIES")
endif()

# Position-independent code for shared libs
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find OpenSSL
find_package(OpenSSL REQUIRED)
message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")

# Third-party libraries
set(DPP_LOCAL_PATH "${PROJECT_SOURCE_DIR}/DPP" CACHE PATH "Path to local DPP directory")

include(ExternalProject)
include(buildtools/cmake/DownloadProject.cmake)

# Disable unnecessary DPP components
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(DPP_BUILD_TEST OFF CACHE BOOL "" FORCE)
set(DPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(DPP_ENABLE_VOICE OFF CACHE BOOL "" FORCE)
set(DPP_USE_EXTERNAL_DATE OFF CACHE BOOL "" FORCE)

# Handle DPP integration
if(USE_LOCAL_DPP)
    message(STATUS "Using local DPP from: ${DPP_LOCAL_PATH}")

    if(NOT EXISTS ${DPP_LOCAL_PATH}/CMakeLists.txt)
        message(FATAL_ERROR "Could not find DPP at ${DPP_LOCAL_PATH}. Please clone it or use git submodule.")
    endif()

    add_subdirectory(${DPP_LOCAL_PATH})
    set(DPP_SOURCE_DIR ${DPP_LOCAL_PATH})

else()
    message(STATUS "Downloading DPP from GitHub...")
    download_project(
        DPP
        GIT_REPOSITORY https://github.com/brainboxdotcc/DPP.git
        GIT_TAG        v10.1.3
        GIT_SHALLOW    TRUE
        SOURCE_DIR     ${PROJECT_SOURCE_DIR}/DPP
    )
    add_subdirectory(${PROJECT_SOURCE_DIR}/DPP)
    set(DPP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/DPP)
endif()

# Add your plugin sources
add_subdirectory(src)
