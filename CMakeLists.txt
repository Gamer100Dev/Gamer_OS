cmake_minimum_required(VERSION 3.5)
project(AquiumOS)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Specify GCC as the compiler (adjust path if necessary)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")

# Find Qt packages
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)

# Add include directories for Qt
include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
)

# Find X11 package (if needed)
find_package(X11 REQUIRED)

# Source files for startAQ executable
set(startAQ_SOURCES
    src/Window_Manager.cpp
)

# Executable for startAQ
add_executable(startAQ.so ${startAQ_SOURCES})

# Link libraries for startAQ
target_link_libraries(startAQ.so
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    X11::X11  # Use X11 library found by CMake
    Xrandr
    Xcursor
    GLEW
    GL  # Link against OpenGL library (libGL)
)
