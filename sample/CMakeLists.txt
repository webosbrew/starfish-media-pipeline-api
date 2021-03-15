cmake_minimum_required(VERSION 3.6)

include(WebOSPackage)

project(player-sample LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(OpenGL REQUIRED IMPORTED_TARGET egl glesv2)
pkg_check_modules(json REQUIRED IMPORTED_TARGET json-c pbnjson_cpp)
pkg_check_modules(PmLogLib REQUIRED IMPORTED_TARGET PmLogLib)

add_executable(player-sample 
src/main.c
src/StarfishDirectMediaPlayer.cpp
src/SDL_webOS.c
)

target_compile_definitions(player-sample PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>" _GNU_SOURCE)
target_include_directories(player-sample PRIVATE ${CMAKE_SOURCE_DIR}/utils/include)

target_link_libraries(player-sample PRIVATE
 PkgConfig::SDL2 PkgConfig::OpenGL PkgConfig::json PkgConfig::PmLogLib
 playerAPIs m dl pthread
)

set_target_properties(player-sample PROPERTIES
    WEBOS_APPINFO_ID org.webosbrew.sample.player-apis
    WEBOS_APPINFO_VERSION 0.0.1
    WEBOS_APPINFO_VENDOR "Mariotaku Lee"
    WEBOS_APPINFO_ICON ${CMAKE_CURRENT_SOURCE_DIR}/icon.png
    WEBOS_APPINFO_TITLE "playerAPIs"
)

target_webos_package(player-sample)