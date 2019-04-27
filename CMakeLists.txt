cmake_minimum_required(VERSION 3.13)
project(V1_Exp4 C)

set(CMAKE_C_STANDARD 99)

add_executable(V1_Exp4 main.c extmem.h extmem.c utils.c utils.h operations.c operations.h)