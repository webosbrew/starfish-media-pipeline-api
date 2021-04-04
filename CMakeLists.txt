cmake_minimum_required(VERSION 3.6)
project(starfish-media-pipeline C CXX)

set (CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB REQUIRED glib-2.0)

add_library(playerAPIs SHARED "src/StarfishMediaAPIs_stub.cpp")

set_target_properties(playerAPIs PROPERTIES
    PUBLIC_HEADER         ${CMAKE_CURRENT_SOURCE_DIR}/StarfishMediaAPIs.h
    SOVERSION             "1"
)
target_include_directories(playerAPIs SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(playerAPIs SYSTEM PRIVATE ${GLIB_INCLUDE_DIRS})

# Don't include samples if used as library
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_library(playerAPIs_wrapper_c SHARED "src/StarfishMediaAPIs_wrapper_c.c")
    target_link_libraries(playerAPIs_wrapper_c dl)
    target_compile_definitions(playerAPIs_wrapper_c PUBLIC _GNU_SOURCE)

    # Add folder where are supportive functions
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/utils/cmake)

    add_subdirectory(sample)
endif()