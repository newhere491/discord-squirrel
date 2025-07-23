add_library(sqdiscord SHARED
    main.hpp
    main.cpp
    includes.hpp
    CSession.hpp
    CSession.cpp

)

# Define _SQ64 if 64-bit
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_definitions(sqdiscord PRIVATE _SQ64)
    message("DEFAULTING TO 64 BIT BINARIES")
endif()

# Plugin name without "lib" prefix
set_target_properties(sqdiscord PROPERTIES PREFIX "")

# Plugin output name
if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT FORCE_32BIT_BIN)
    set(PLUGIN_NAME "discord04rel64")
else()
    set(PLUGIN_NAME "discord04rel32")
endif()

set_target_properties(sqdiscord PROPERTIES OUTPUT_NAME ${PLUGIN_NAME})

# Include headers
target_include_directories(sqdiscord
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/readerwriterqueue
        ${DPP_SOURCE_DIR}/include
        ${OPENSSL_INCLUDE_DIR}
)

# Link static libcurl + OpenSSL + Windows libs
target_link_libraries(sqdiscord
    PRIVATE
        ${OPENSSL_LIBRARIES}
        dpp
        -static
)

# Static linking macro
target_compile_definitions(sqdiscord PRIVATE DPP_STATIC)
