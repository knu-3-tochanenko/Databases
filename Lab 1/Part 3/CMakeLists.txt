cmake_minimum_required(VERSION 3.14)
project(Part_3 C)

set(CMAKE_C_STANDARD 11)

add_executable(Part_3 main.c
        structures.h structures.c
        indexTable.h indexTable.c
        run.h run.c
        io.h io.c
        insert.h insert.c
        delete.h delete.c
        get.h get.c
        update.h update.c
        count.h count.c)