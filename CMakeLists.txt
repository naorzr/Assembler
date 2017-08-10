cmake_minimum_required(VERSION 3.6)
project(EndProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_STANDARD 90)

set(SOURCE_FILES
        main.c
        logger.c
        assembler.c content_validation.c content_validation.h data_struct.c data_struct.h error_handler.c error_handler.h helpers.c helpers.h)
add_executable(EndProject ${SOURCE_FILES})