cmake_minimum_required(VERSION 3.10)
project(MTM_Recorder)

set(CMAKE_CXX_STANDARD 17)

# Platform check
if(NOT WIN32)
    message(FATAL_ERROR "This project can only be built on Windows systems")
endif()

# Find FFmpeg
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED libavcodec libavformat libavutil libswscale)

# Windows-specific configuration
set(Gdiplus_LIBRARIES gdiplus)
set(Gdiplus_INCLUDE_DIRS "")
add_library(Gdiplus::Gdiplus INTERFACE IMPORTED)
set_target_properties(Gdiplus::Gdiplus PROPERTIES
    INTERFACE_LINK_LIBRARIES "${Gdiplus_LIBRARIES}"
)

# Add source files
add_executable(MTM_Recorder
    src/main.cpp
    src/recorder.cpp
    src/ui.cpp
)

# Include directories
include_directories(include)

# Link libraries
target_link_libraries(MTM_Recorder
    PRIVATE
    ${Gdiplus_LIBRARIES}
    ${FFMPEG_LIBRARIES}
    winmm
    ole32
    uuid
)

target_include_directories(MTM_Recorder
    PRIVATE
    ${FFMPEG_INCLUDE_DIRS}
)
