cmake_minimum_required(VERSION 3.10)
project(MTM_Recorder)

set(CMAKE_CXX_STANDARD 17)

# Add source files
add_executable(MTM_Recorder
    src/main.cpp
    src/recorder.cpp
    src/ui.cpp
)

# Include directories
include_directories(include)
