cmake_minimum_required(VERSION 3.10)

project(hello_cmake C)
add_executable(hello_cmake.elf main.c)

add_custom_target(hello_cmake.self ALL
    COMMAND ${ELF2SELF} ${CMAKE_CURRENT_BINARY_DIR}/hello_cmake.elf ${CMAKE_CURRENT_BINARY_DIR}/hello_cmake.self
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/hello_cmake.elf
)
