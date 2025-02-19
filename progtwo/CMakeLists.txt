

cmake_minimum_required(VERSION 3.16)



set(CMAKE_CXX_STANDARD 20)

project(progtwo LANGUAGES CXX)
add_compile_options(-Wall -Wextra -Wpedantic)

# add_compile_options(-fsanitize=thread)


add_executable(progtwo src/main.cpp include/CharCount.h)


target_include_directories(progtwo PRIVATE ${CMAKE_SOURCE_DIR}/include)


