cmake_minimum_required(VERSION 3.5)
project(AquiumOS)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Find Qt packages
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)

# Include directories for Qt
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS})

# Find Wayland libraries and scanner
find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND_SCANNER REQUIRED wayland-scanner)
pkg_check_modules(WLR REQUIRED wlroots-0.18)
pkg_check_modules(WAYLAND REQUIRED wayland-server wayland-client xkbcommon)

# Find wayland-scanner
find_program(WAYLAND_SCANNER_EXECUTABLE wayland-scanner)
if (NOT WAYLAND_SCANNER_EXECUTABLE)
    message(FATAL_ERROR "wayland-scanner not found. Install Wayland development libraries.")
endif()

# Set Wayland protocol path for Linux and FreeBSD
if (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    set(WAYLAND_PROTOCOLS_PATH "/usr/local/share/wayland-protocols")
    set(WLROOTS_INCLUDE_DIR "/usr/local/include/wlroots")
else()
    set(WAYLAND_PROTOCOLS_PATH "/usr/share/wayland-protocols")
    set(WLROOTS_INCLUDE_DIR "/usr/include/wlroots")
endif()

# Ensure the protocol file exists
if (NOT EXISTS "${WAYLAND_PROTOCOLS_PATH}/stable/xdg-shell/xdg-shell.xml")
    message(FATAL_ERROR "xdg-shell.xml not found in ${WAYLAND_PROTOCOLS_PATH}")
endif()

# Include wlroots directory
include_directories(${WLROOTS_INCLUDE_DIR})

# Define output file for Wayland protocol
set(XDG_SHELL_PROTOCOL_OUTPUT "${CMAKE_BINARY_DIR}/xdg-shell-protocol.h")

# Generate xdg-shell protocol header
add_custom_command(
    OUTPUT ${XDG_SHELL_PROTOCOL_OUTPUT}
    COMMAND ${WAYLAND_SCANNER_EXECUTABLE} server-header
            ${WAYLAND_PROTOCOLS_PATH}/stable/xdg-shell/xdg-shell.xml
            ${XDG_SHELL_PROTOCOL_OUTPUT}
    DEPENDS ${WAYLAND_PROTOCOLS_PATH}/stable/xdg-shell/xdg-shell.xml
    COMMENT "Generating xdg-shell-protocol.h"
)

# Add a custom target to ensure the header file is generated
add_custom_target(generate_xdg_shell_protocol_header ALL
    DEPENDS ${XDG_SHELL_PROTOCOL_OUTPUT}
)

# Include generated header
include_directories(${CMAKE_BINARY_DIR})

# Define source files for each executable
set(gamer_compositor_SOURCES src/gamer_compositor.c ${XDG_SHELL_PROTOCOL_OUTPUT})
set(startAQ_SOURCES src/Window_Manager.cpp)
set(installer_dep_SOURCES src/installer_dep.cpp)
set(init_gameros_SOURCES src/init_window.cpp)
set(launchd_SOURCES src/launchd.cpp)
set(System_Services_SOURCES src/System_Services.cpp)

# Add executables
add_executable(gamer_compositor.so ${gamer_compositor_SOURCES})
add_executable(startAQ.so ${startAQ_SOURCES})
add_executable(installer_dep.so ${installer_dep_SOURCES})
add_executable(init_gameros.so ${init_gameros_SOURCES})
add_executable(launchd.so ${launchd_SOURCES})
add_executable(System_Services.so ${System_Services_SOURCES})

# Link libraries individually for each executable
target_link_libraries(gamer_compositor.so ${WLR_LIBRARIES} ${WAYLAND_LIBRARIES})
target_link_libraries(startAQ.so Qt5::Widgets Qt5::Gui Qt5::Core X11 Xrandr Xcursor GLEW GL ${WLR_LIBRARIES} ${WAYLAND_LIBRARIES})
target_link_libraries(installer_dep.so Qt5::Widgets Qt5::Gui Qt5::Core X11 Xrandr Xcursor GLEW GL ${WLR_LIBRARIES} ${WAYLAND_LIBRARIES})
target_link_libraries(init_gameros.so Qt5::Widgets Qt5::Gui Qt5::Core X11 Xrandr Xcursor GLEW GL ${WLR_LIBRARIES} ${WAYLAND_LIBRARIES})
target_link_libraries(launchd.so Qt5::Widgets Qt5::Gui Qt5::Core X11 Xrandr Xcursor GLEW GL ${WLR_LIBRARIES} ${WAYLAND_LIBRARIES})
target_link_libraries(System_Services.so Qt5::Widgets Qt5::Gui Qt5::Core X11 Xrandr Xcursor GLEW GL ${WLR_LIBRARIES} ${WAYLAND_LIBRARIES})

# Add dependencies for gamer_compositor to ensure header generation
add_dependencies(gamer_compositor.so generate_xdg_shell_protocol_header)

# Set output suffix for FreeBSD compatibility
set_target_properties(gamer_compositor.so startAQ.so installer_dep.so init_gameros.so launchd.so System_Services.so PROPERTIES SUFFIX ".so")
