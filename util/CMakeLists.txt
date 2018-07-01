cmake_minimum_required(VERSION 3.11)

add_library(Util INTERFACE)

target_include_directories(Util INTERFACE include)

add_executable(UtilDemo demo/main.cpp)

target_link_libraries(UtilDemo Util)

set_target_properties(UtilDemo PROPERTIES CXX_STANDARD          17
                                          CXX_STANDARD_REQUIRED ON)