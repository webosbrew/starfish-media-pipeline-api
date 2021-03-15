cmake_minimum_required(VERSION 3.6)

include(WebOSPackage)

project(starfish-sdl LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(OpenGL REQUIRED IMPORTED_TARGET egl glesv2)
pkg_check_modules(GStreamer REQUIRED IMPORTED_TARGET gstreamer-1.0 gstreamer-app-1.0)
pkg_check_modules(json REQUIRED IMPORTED_TARGET json-c pbnjson_cpp)

add_executable(starfish-sdl 
src/main.c
src/gst_sample.c
src/StarfishDirectMediaPlayer.cpp
src/SDL_webOS.c
)

target_compile_definitions(starfish-sdl PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>" _GNU_SOURCE)
target_include_directories(starfish-sdl PRIVATE ${CMAKE_SOURCE_DIR}/utils/include)

target_link_libraries(starfish-sdl PRIVATE
 PkgConfig::SDL2 PkgConfig::OpenGL PkgConfig::GStreamer PkgConfig::json
 StarfishMediaAPIs
 m dl
)

set_target_properties(starfish-sdl PROPERTIES
    WEBOS_APPINFO_ID org.webosbrew.sample.starfish-player.gst-sdl
    WEBOS_APPINFO_VERSION 0.0.1
    WEBOS_APPINFO_VENDOR "Mariotaku Lee"
    WEBOS_APPINFO_ICON ${CMAKE_CURRENT_SOURCE_DIR}/icon.png
    WEBOS_APPINFO_TITLE "Starfish"
)

target_webos_package(starfish-sdl)