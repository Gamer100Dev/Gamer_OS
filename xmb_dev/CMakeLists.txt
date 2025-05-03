cmake_minimum_required(VERSION 3.16)

project(XMBPrototype)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set source files
set(SOURCES
    src/main.cpp
    external/glad/src/glad.c
    
    external/glad/include/glad/glad.h     # <-- add glad.c here
)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find SDL2
find_package(SDL2 REQUIRED)

# Find SDL2_ttf
find_package(SDL2_ttf REQUIRED)

# Find FreeType
find_package(Freetype REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}  # SDL_ttf include directory
    ${FREETYPE_INCLUDE_DIRS}
    external/glad/include     # <-- add glad includes
)

# Add executable
add_executable(xmb-prototype ${SOURCES})

# Link the necessary libraries
target_link_libraries(xmb-prototype
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}  # Link SDL_ttf
    ${FREETYPE_LIBRARIES}
    OpenGL::GL
)
