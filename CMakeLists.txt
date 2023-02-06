# Copyright (c) 2023 Sebastian Pipping <sebastian@pipping.org>
# Licensed under GPL v3 or later

cmake_minimum_required(VERSION 3.6)
# CMake features used that need more than CMake 3.0:
# - pkg_check_modules([..] IMPORTED_TARGET [..]) needs >=3.6

project(AudioVisualiser VERSION 9.9.9)

include(FindPkgConfig)

pkg_check_modules(SFML sfml-all REQUIRED IMPORTED_TARGET)

add_executable(AudioVisualiser
    Code/Headerss/Menu.h
    Code/Headerss/Modes.h
    Code/Headerss/SongDatabase.h
    Code/Headerss/Vizualizer.h
    Code/Source\ Code/AudioVizualizer.cpp
    Code/Source\ Code/Buttons.cpp
    Code/Source\ Code/Menu.cpp
    Code/Source\ Code/Modes.cpp
    Code/Source\ Code/SongDatabase.cpp
    Code/Source\ Code/Vizualizer.cpp
)

target_link_libraries(AudioVisualiser
    PUBLIC
    PkgConfig::SFML
)
