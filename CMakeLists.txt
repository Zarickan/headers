cmake_minimum_required(VERSION 3.10)
project(headers C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(src
        src/bitmap.h
        src/color.h
        src/cur.h
        src/heaps.h
        src/ico.h
        src/linkedlist.h
        src/list.h
        src/main.c
        src/memory.h
        src/stack.h
        src/types.h)
