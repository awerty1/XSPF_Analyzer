cmake_minimum_required(VERSION 3.26)
project(XSPF_Analyzer)

set(CMAKE_CXX_STANDARD 23)
# -Wall - максимальное количество предупреждений
# -g - включает генерацию отладочной информации в исполняемом файле
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

add_executable(XSPF_Analyzer main.cpp
        playlist_analyzer.h
        playlist_analyzer.cpp
        config.h
        config_example.h)
