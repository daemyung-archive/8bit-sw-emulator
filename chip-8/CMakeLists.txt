
add_library(Chip8 include/chip-8/chip-spec.h
                  include/chip-8/chip.h
                  source/types.h
                  source/chip.cpp)

target_include_directories(Chip8 PUBLIC include)

set_target_properties(Chip8 PROPERTIES CXX_STANDARD          17
                                       CXX_STANDARD_REQUIRED ON)

add_executable(Chip8Demo demo/main.cpp)

target_link_libraries(Chip8Demo Stb
                                Platform
                                Chip8)

set_target_properties(Chip8Demo PROPERTIES CXX_STANDARD          17
                                           CXX_STANDARD_REQUIRED ON)

