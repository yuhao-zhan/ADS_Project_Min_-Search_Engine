cmake_minimum_required(VERSION 3.27)
project(MiniSearchEngine C)

set(CMAKE_C_STANDARD 11)

add_executable(MiniSearchEngine
        source_files/functions_in_threshold_test.c
        source_files/index.c
        source_files/Query.c
        #[[source_files/test.c]]
        source_files/newStmr.c
        source_files/removepunc.c
        header_files/AllFunctions.h
        source_files/main.c
)

# Manually set the path for json-c library
include_directories(/opt/homebrew/Cellar/json-c/0.18/include/json-c)
link_directories(/opt/homebrew/Cellar/json-c/0.18/lib)

# Link the json-c library
target_link_libraries(MiniSearchEngine /opt/homebrew/Cellar/json-c/0.18/lib/libjson-c.dylib)

