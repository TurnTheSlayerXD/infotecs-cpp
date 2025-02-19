

cmake_minimum_required(VERSION 3.16)



set(CMAKE_CXX_STANDARD 20)

project(progone LANGUAGES CXX)
add_compile_options(-Wall -Wextra -Wpedantic)

# add_compile_options(-fsanitize=thread)


add_executable(progone src/main.cpp include/Buffer.h include/ipc_interaction.h)


target_include_directories(progone PRIVATE ${CMAKE_SOURCE_DIR}/include)


