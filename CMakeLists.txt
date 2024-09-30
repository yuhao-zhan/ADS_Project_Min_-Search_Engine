cmake_minimum_required(VERSION 3.27)
project(MiniSearchEngine C)

set(CMAKE_C_STANDARD 11)

add_executable(MiniSearchEngine
        source_files/functions_in_threshold_test.c
        header_files/functions_in_threshold_test.h
        header_files/index.h
        header_files/Query.h
        source_files/index.c
        source_files/Query.c
        source_files/test.c
        header_files/esa.h
        header_files/esa_rules.h
        #[[source_files/esaExample.c]]
        source_files/esa.c
        header_files/stmr.h
        source_files/stmr.c
        #[[source_files/main.c]]
)
