cmake_minimum_required(VERSION 3.26)
project(XSPF_Analyzer)

set(CMAKE_CXX_STANDARD 23)

add_executable(XSPF_Analyzer main.cpp
        playlist_analyzer.h
        playlist_analyzer.cpp
        config.h
        config_example.h)
