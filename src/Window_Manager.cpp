#include <iostream>
#include <string>
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>
#include <QTextEdit>
#include <QOpenGLWidget>
#include <chrono>
#include <QDockWidget>
#include <QScreen>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <X11/Xcursor/Xcursor.h>

// Global variable for resolution
std::string Reso;
QString Get_Active_Time(){
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);
    std::cout << "The current time is " <<  std::ctime(&current_time_t) << std::endl;
    return std::ctime(&current_time_t);
}
// Function to parse resolution string
void parseResolution(const std::string& reso, int& width, int& height) {
    size_t xPos = reso.find('x');
    if (xPos == std::string::npos) {
        std::cerr << "Error: Resolution string format is incorrect." << std::endl;
        return;
    }

    std::string widthStr = reso.substr(0, xPos);
    std::string heightStr = reso.substr(xPos + 1);

    width = std::stoi(widthStr);
    height = std::stoi(heightStr);
}

// Function to log XRandR error status
void logXRRErrorStatus(int status) {
    switch (status) {
        case RRSetConfigSuccess:
            std::cout << "RRSetConfigSuccess: Screen configuration set successfully." << std::endl;
            break;
        case RRSetConfigInvalidConfigTime:
            std::cerr << "RRSetConfigInvalidConfigTime: Invalid configuration time." << std::endl;
            break;
        case RRSetConfigInvalidTime:
            std::cerr << "RRSetConfigInvalidTime: Invalid time." << std::endl;
            break;
        case RRSetConfigFailed:
            std::cerr << "RRSetConfigFailed: Configuration failed." << std::endl;
            break;
        default:
            std::cerr << "Unknown RRSetConfig status: " << status << std::endl;
            break;
    }
}

// Function to query screen information
Display* queryScreenInformation() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Error: Could not open X display" << std::endl;
        return nullptr;
    }
    std::cout << "Display: Successfully opened X display." << std::endl;

    int screen = DefaultScreen(display);
    std::cout << "Screen: Default screen number obtained." << std::endl;

    XRRScreenResources* screenResources = XRRGetScreenResources(display, RootWindow(display, screen));
    if (!screenResources) {
        std::cerr << "Error: Failed to get screen resources" << std::endl;
        XCloseDisplay(display);
        return nullptr;
    }
    std::cout << "ScreenResources: Successfully obtained screen resources." << std::endl;

    int bestModeIndex = -1;
    unsigned int bestWidth = 0;
    unsigned int bestHeight = 0;

    for (int i = 0; i < screenResources->nmode; ++i) {
        XRRModeInfo* mode = &screenResources->modes[i];
        if (mode->width > bestWidth || mode->height > bestHeight) {
            bestModeIndex = i;
            bestWidth = mode->width;
            bestHeight = mode->height;
        }
    }

    if (bestModeIndex == -1) {
        std::cerr << "Error: No valid mode found" << std::endl;
        XRRFreeScreenResources(screenResources);
        XCloseDisplay(display);
        return nullptr;
    }

    RRMode targetMode = screenResources->modes[bestModeIndex].id;
    std::cout << "Mode: Selected target mode ID: " << targetMode << std::endl;

    char* primaryOutput = nullptr;
    for (int i = 0; i < screenResources->noutput; ++i) {
        XRROutputInfo* outputInfo = XRRGetOutputInfo(display, screenResources, screenResources->outputs[i]);
        if (outputInfo) {
            if (outputInfo->connection == RR_Connected && outputInfo->crtc) {
                primaryOutput = outputInfo->name;
                XRRFreeOutputInfo(outputInfo);
                break;
            }
            XRRFreeOutputInfo(outputInfo);
        }
    }

    if (!primaryOutput) {
        std::cerr << "Error: No connected primary output found" << std::endl;
        XRRFreeScreenResources(screenResources);
        XCloseDisplay(display);
        return nullptr;
    }

    int rate = 60;
    std::string xrandr_cmd = "xrandr --output ";
    xrandr_cmd += primaryOutput;
    xrandr_cmd += " --mode ";
    xrandr_cmd += std::to_string(screenResources->modes[bestModeIndex].width);
    xrandr_cmd += "x";
    xrandr_cmd += std::to_string(screenResources->modes[bestModeIndex].height);
    xrandr_cmd += " --rate ";
    xrandr_cmd += std::to_string(rate);

    std::cout << "LOG: Command is " << xrandr_cmd << std::endl;
    int ret = system(xrandr_cmd.c_str());
    if (ret != 0) {
        std::cerr << "Error: Failed to execute xrandr command" << std::endl;
    }

    Reso = std::to_string(screenResources->modes[bestModeIndex].width) + "x" + std::to_string(screenResources->modes[bestModeIndex].height);

    XRRFreeScreenResources(screenResources);

    return display;
}

// Function to draw the desktop environment
void DrawDE(Display* display) {
    std::cout << "LOG: Resolution for window is " << Reso << std::endl;

    int argc = 0;
    char *argv[] = { nullptr };
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setGeometry(0, 0, 1029, 496);


    QPushButton* pushButton = new QPushButton("Search", &mainWindow);
    pushButton->setGeometry(100, 10, 80, 26);

    QPushButton* pushButton_2 = new QPushButton("Apps", &mainWindow);
    pushButton_2->setGeometry(10, 10, 80, 26);

    QTextEdit* ktextedit = new QTextEdit(&mainWindow);
    ktextedit->setGeometry(880, 10, 141, 21);
    ktextedit->setHtml(QString(Get_Active_Time()));

    QDockWidget* dockWidget = new QDockWidget(&mainWindow);
    dockWidget->setGeometry(120, 390, 821, 80);
    dockWidget->setAcceptDrops(true);
    dockWidget->setFloating(false);
    mainWindow.show();

    Cursor cursor = XcursorFilenameLoadCursor(display, "/Assets/cursor");
    if (cursor == None) {
        std::cerr << "Error: Could not load custom cursor" << std::endl;
    } else {
        Window rootWindow = DefaultRootWindow(display);
        XDefineCursor(display, rootWindow, cursor);
        XFlush(display);
        std::cout << "LOG: Custom cursor set." << std::endl;
    }

    app.exec();
}

// Main function
int main() {
    Get_Active_Time();
    std::cout << "LOG: Loading DE!" << std::endl;
    Display* display = queryScreenInformation();
    if (display) {
        DrawDE(display);
    }

    return 0;
}
