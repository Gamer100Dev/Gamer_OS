cmake_minimum_required(VERSION 3.5)
project(AquiumOS)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

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

# Source files for startAQ executable
set(startAQ_SOURCES
    src/Window_Manager.cpp
)
set(installer_dep_SOURCES
    src/installer_dep.cpp
)

# Executable for startAQ
add_executable(startAQ.so
    ${startAQ_SOURCES}
)
add_executable(installer_dep.so
    ${installer_dep_SOURCES}
)

# Link libraries for startAQ
target_link_libraries(startAQ.so
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    -lX11
    -lXrandr
    -lXcursor
    -lGLEW
    -lGL  # Link against OpenGL library (libGL)
)

# Source files for init_gameros executable
set(init_gameros_SOURCES
    src/init_window.cpp
)

# Executable for init_gameros
add_executable(init_gameros.so
    ${init_gameros_SOURCES}
)

# Link libraries for init_gameros
target_link_libraries(init_gameros.so
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    -lX11
    -lXrandr
    -lXcursor
    -lGLEW
    -lGL  # Link against OpenGL library (libGL)
)
