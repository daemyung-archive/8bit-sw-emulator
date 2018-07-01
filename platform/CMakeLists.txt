cmake_minimum_required(VERSION 3.00)

add_library(Platform STATIC include/platform/display.h
                            include/platform/window.h
                            include/platform/pixmap.h
                                      source/display.cpp
                                      source/window.cpp
                                      source/pixmap.cpp)

if (CMAKE_SYSTEM_NAME MATCHES Windows)

target_compile_definitions(Platform PUBLIC PLATFORM_WIN32
                                           NOMINMAX)

else ()

message(FATAL_ERROR "[Platform] Only building for Win32 is supported!!!")

endif ()

target_include_directories(Platform PUBLIC include)

target_link_libraries(Platform PUBLIC Util)

set_target_properties(Platform PROPERTIES CXX_STANDARD          17
                                          CXX_STANDARD_REQUIRED ON)

add_executable(PlatformDemo demo/demo.cpp)

target_link_libraries(PlatformDemo Platform)

set_target_properties(PlatformDemo PROPERTIES CXX_STANDARD          17
                                              CXX_STANDARD_REQUIRED ON)