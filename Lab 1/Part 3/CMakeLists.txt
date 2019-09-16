cmake_minimum_required(VERSION 3.14)
project(Part_3 C)

set(CMAKE_C_STANDARD 11)

add_executable(Part_3 main.c
        tables.h
        indexTable.h indexTable.c
        application.h application.c
        inputoutput.h inputoutput.c
        database.h database.c)