cmake_minimum_required(VERSION 3.5)
project(AquiumOS)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
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

# Executable for startAQ
add_executable(startAQ.so ${startAQ_SOURCES})

# Link libraries for startAQ
target_link_libraries(startAQ.so
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    X11
    Xrandr
    Xcursor
    GLEW
    GL  # Link against OpenGL library (libGL)
)

# Source files for installer_dep executable
set(installer_dep_SOURCES
    src/installer_dep.cpp
)

# Executable for installer_dep
add_executable(installer_dep.so ${installer_dep_SOURCES})

# Link libraries for installer_dep
target_link_libraries(installer_dep.so
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    X11
    Xrandr
    Xcursor
    GLEW
    GL  # Link against OpenGL library (libGL)
)

# Source files for init_gameros executable
set(init_gameros_SOURCES
    src/init_window.cpp
)

# Executable for init_gameros
add_executable(init_gameros.so ${init_gameros_SOURCES})

# Link libraries for init_gameros
target_link_libraries(init_gameros.so
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    X11
    Xrandr
    Xcursor
    GLEW
    GL  # Link against OpenGL library (libGL)
)
set(launchd_SOURCES
    src/launchd.cpp
)
add_executable(launchd.so ${launchd_SOURCES})
target_link_libraries(launchd.so
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    X11
    Xrandr
    Xcursor
    GLEW
    GL  # Link against OpenGL library (libGL)
)
set(System_Services_SOURCES
    src/System_Services.cpp
)
add_executable(System_Services.so ${System_Services_SOURCES})
target_link_libraries(System_Services.so
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    X11
    Xrandr
    Xcursor
    GLEW
    GL  # Link against OpenGL library (libGL)
)
