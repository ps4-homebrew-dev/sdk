cmake_minimum_required(VERSION 3.12)

project(hello_cmake C)
add_executable(hello_cmake main.c)
orbis_add_self(hello_cmake)
