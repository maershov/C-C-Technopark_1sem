cmake_minimum_required(VERSION 3.14)
project(MYIZ1 C)

set(CMAKE_C_STANDARD 11)

add_executable(MYIZ1 main.c)

add_custom_target(
        clangformat
        COMMAND /usr/bin/clang-format
        -style=LLVM
        -i
        ${ALL_SOURCE_FILES}
)