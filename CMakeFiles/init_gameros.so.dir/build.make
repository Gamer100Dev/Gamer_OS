# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gamer/Desktop/AquiumOS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gamer/Desktop/AquiumOS

# Include any dependencies generated for this target.
include CMakeFiles/init_gameros.so.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/init_gameros.so.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/init_gameros.so.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/init_gameros.so.dir/flags.make

CMakeFiles/init_gameros.so.dir/src/init_window.cpp.o: CMakeFiles/init_gameros.so.dir/flags.make
CMakeFiles/init_gameros.so.dir/src/init_window.cpp.o: src/init_window.cpp
CMakeFiles/init_gameros.so.dir/src/init_window.cpp.o: CMakeFiles/init_gameros.so.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/gamer/Desktop/AquiumOS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/init_gameros.so.dir/src/init_window.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/init_gameros.so.dir/src/init_window.cpp.o -MF CMakeFiles/init_gameros.so.dir/src/init_window.cpp.o.d -o CMakeFiles/init_gameros.so.dir/src/init_window.cpp.o -c /home/gamer/Desktop/AquiumOS/src/init_window.cpp

CMakeFiles/init_gameros.so.dir/src/init_window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/init_gameros.so.dir/src/init_window.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gamer/Desktop/AquiumOS/src/init_window.cpp > CMakeFiles/init_gameros.so.dir/src/init_window.cpp.i

CMakeFiles/init_gameros.so.dir/src/init_window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/init_gameros.so.dir/src/init_window.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gamer/Desktop/AquiumOS/src/init_window.cpp -o CMakeFiles/init_gameros.so.dir/src/init_window.cpp.s

# Object files for target init_gameros.so
init_gameros_so_OBJECTS = \
"CMakeFiles/init_gameros.so.dir/src/init_window.cpp.o"

# External object files for target init_gameros.so
init_gameros_so_EXTERNAL_OBJECTS =

build/init_gameros.so: CMakeFiles/init_gameros.so.dir/src/init_window.cpp.o
build/init_gameros.so: CMakeFiles/init_gameros.so.dir/build.make
build/init_gameros.so: /usr/lib/libQt5Widgets.so.5.15.14
build/init_gameros.so: /usr/lib/libQt5Gui.so.5.15.14
build/init_gameros.so: /usr/lib/libQt5Core.so.5.15.14
build/init_gameros.so: CMakeFiles/init_gameros.so.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/gamer/Desktop/AquiumOS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable build/init_gameros.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/init_gameros.so.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/init_gameros.so.dir/build: build/init_gameros.so
.PHONY : CMakeFiles/init_gameros.so.dir/build

CMakeFiles/init_gameros.so.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/init_gameros.so.dir/cmake_clean.cmake
.PHONY : CMakeFiles/init_gameros.so.dir/clean

CMakeFiles/init_gameros.so.dir/depend:
	cd /home/gamer/Desktop/AquiumOS && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gamer/Desktop/AquiumOS /home/gamer/Desktop/AquiumOS /home/gamer/Desktop/AquiumOS /home/gamer/Desktop/AquiumOS /home/gamer/Desktop/AquiumOS/CMakeFiles/init_gameros.so.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/init_gameros.so.dir/depend

