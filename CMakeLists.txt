cmake_minimum_required(VERSION 3.6)
project(starfish-media-pipeline C CXX)

set (CMAKE_CXX_STANDARD 11)

set(PROJECT_NAME "starfish-media-pipeline")
set(PROJECT_DESCRIPTION "Native API Development Library for webOS")
set(PROJECT_AUTHOR "Mariotaku Lee")
set(PROJECT_URL "https://github.com/webosbrew/libndl")

set(BIN_INSTALL_DIR bin CACHE STRING "Binary installation directory relative to the install prefix")
set(LIB_INSTALL_DIR lib CACHE STRING "Library installation directory relative to the install prefix")
set(INCLUDE_INSTALL_DIR include CACHE STRING "Header installation directory relative to the install prefix")

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB REQUIRED glib-2.0)

add_library(playerAPIs SHARED "src/StarfishMediaAPIs_stub.cpp")

set_target_properties(playerAPIs PROPERTIES
    PUBLIC_HEADER         ${CMAKE_CURRENT_SOURCE_DIR}/StarfishMediaAPIs.h
    SOVERSION             "1"
)
target_include_directories(playerAPIs SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(playerAPIs SYSTEM PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(playerAPIs INTERFACE ${GLIB_LIBRARIES})

# Don't include samples if used as library
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # Add folder where are supportive functions
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/utils/cmake)

    add_subdirectory(sample)
endif()