# Specify the Qt modules your project requires
QT += widgets core 

# Define the target and template type
TARGET = startAQ
TEMPLATE = app

# Source files to be compiled
SOURCES += \
    ./src/Window_Manager.cpp

# Link against external libraries (X11, Xrandr, Xcursor, OpenGL)
LIBS += \
    -lX11 \
    -lXrandr \
    -lXcursor \
    -lGLEW \
    -lGL  # Link against OpenGL library (libGL)

# Additional include directories if needed
INCLUDEPATH += \
    /usr/include/qt # Add path to Qt includes if not in default location
