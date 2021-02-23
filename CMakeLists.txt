cmake_minimum_required(VERSION 3.6)
project(starfish-player-api C CXX)

set (CMAKE_CXX_STANDARD 11)

set(PROJECT_NAME "starfish-player-api")
set(PROJECT_DESCRIPTION "Native API Development Library for webOS")
set(PROJECT_AUTHOR "Mariotaku Lee")
set(PROJECT_URL "https://github.com/webosbrew/libndl")

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

set(BIN_INSTALL_DIR bin CACHE STRING "Binary installation directory relative to the install prefix")
set(LIB_INSTALL_DIR lib CACHE STRING "Library installation directory relative to the install prefix")
set(INCLUDE_INSTALL_DIR include CACHE STRING "Header installation directory relative to the install prefix")

set(HEADERS "include/Acb.h")

add_library(Acb SHARED IMPORTED)

set_target_properties(Acb PROPERTIES
    PUBLIC_HEADER         ${HEADERS}
    VERSION               ${PROJECT_VERSION}
    IMPORTED_LOCATION     ${CMAKE_SOURCE_DIR}/lib/libAcb.so.1
)
target_include_directories(Acb SYSTEM INTERFACE ${CMAKE_SOURCE_DIR}/include)

# Add folder where are supportive functions
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/utils/cmake)

# Don't include samples if used as library
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(samples)
endif()