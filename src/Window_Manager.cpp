#include <iostream>
#include <string>
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QPushButton>
#include <QString>
#include <QTimer>
#include <QTextEdit>
#include <QOpenGLWidget>
#include <chrono>
#include <QDockWidget>
#include <QScreen>
#include <QTime>
#include <QDebug>
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <X11/Xcursor/Xcursor.h>
#include <GL/gl.h>
#include <iomanip>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <pwd.h>
// Global variable for resolution
std::string Reso;
std::string modules_apps = "/Gamer_OS/kernel/modules";
std::string launchd = modules_apps + "/launchd.ko";
int calculateDockPoints(int& dock_x1, int& dock_x2, int& dock_y1, int& dock_y2) {
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        std::cerr << "Error: Could not get primary screen" << std::endl;
        return 1;
    }

    int screenWidth = screen->size().width();
    int screenHeight = screen->size().height();

    dock_x1 = (screenWidth - (dock_y1 + dock_y2)) / 2;
    dock_y1 = screenHeight - (dock_y1 + dock_x1);
    dock_x2 = dock_x1 + (dock_y1 + dock_y2);
    dock_y2 = screenHeight;

    std::cout << "DEBUG: DockX1 " << dock_x1 << " DockY1 " << dock_y1 << " DockX2 " << dock_x2 << " DockY2 " << dock_y2 << std::endl;
    return(dock_x1 , dock_x2, dock_y1, dock_y2);
}

void customSleep(int seconds) {
    QEventLoop loop;
    QTimer::singleShot(seconds * 1000, &loop, &QEventLoop::quit);
    loop.exec();
}

void handle_events(Display *display) {
    XEvent event;
    while (true) {
        XNextEvent(display, &event);
        std::cout << "Event received: " << event.type << std::endl;
        if (event.type == KeyPress) {
            std::cout << "Key press event detected!" << std::endl;
            // Further processing for key press event
        }
    }
}

QString Get_Active_Time(){
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);
    std::cout << "The current time is " <<  std::ctime(&current_time_t) << std::endl;
    return std::ctime(&current_time_t);
}

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

void calculateScaledPosition(int baseWidth, int baseHeight, int& x, int& y, int& w, int& h) {
    int screenWidth = QApplication::primaryScreen()->size().width();
    int screenHeight = QApplication::primaryScreen()->size().height();

    double scaleX = static_cast<double>(screenWidth) / baseWidth;
    double scaleY = static_cast<double>(screenHeight) / baseHeight;
    double scale = std::min(scaleX, scaleY);
    x = static_cast<int>(x * scale);
    y = static_cast<int>(y * scale);
    double scaleFactor = 1.00;
    w = static_cast<int>(w * scale * scaleFactor);
    h = static_cast<int>(h * scale * scaleFactor);
}

int getMiddleBottomCenter(int& WidgetX, int& WidgetY) {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = screenGeometry.width() / 2;
    int y = screenGeometry.height();
    WidgetX = x;
    WidgetY = y;
    std::cout << "DEBUG: DockX " << WidgetX << " DOCKY " << WidgetY << std::endl;
    return WidgetX,WidgetY;
}
int Caclulate_Widget_X_Y_Bar(int& x1, int& x2, int& y1, int&y2){

}
std::string getCurrentUser() {
    const char* user = getenv("USER");
    if (user == nullptr) {
        struct passwd* pw = getpwuid(getuid());
        if (pw) {
            user = pw->pw_name;
        }
    }
    return user ? user : "unknown";
}

bool PowerOff() {
    std::cout << "LOG: System is powering off! Requested by the user of " << getCurrentUser() << " at "  << Get_Active_Time().toStdString() << std::endl;
    int command = system("sudo poweroff");
    if (command == 0) {
        std::cout << "LOG: Fired exit! Awaiting kernel response!" << std::endl;
        return true;
    } else {
        std::cout << "LOG: Something is preventing the kernel to power off, please check the applications in the background!" << std::endl;
        return false;
    }
}
// << Get_Active_Time()
bool SignOut() {
    std::cout << "LOG: Requested to sign out! Requested by the user of " << getCurrentUser() << " at "  << Get_Active_Time().toStdString() << std::endl;
    int command = system("sudo pkill X startAQ");
    if (command == 0) {
        std::cout << "LOG: Fired exit! Awaiting kernel response!" << std::endl;
        return true;
    } else {
        std::cout << "LOG: Something went wrong with the sign out process!" << std::endl;
        return false;
    }
}
// FreeBSD Userspace management

bool LogIn(QString &username, QString &password){
    std::string std_usrname = username.toStdString();
    std::string std_password = password.toStdString();

    int command = system(std::string("login " + std_usrname + " " + std_password).c_str());

    if (command == 0){
        return 0;
    } else {
        return 1;
    }
}
void DrawDE(Display* display) {
    std::cout << "LOG: Resolution for window is " << Reso << std::endl;
    int DockX; 
    int DockY;
    int argc = 0;
    char *argv[] = { nullptr };
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    getMiddleBottomCenter(DockX, DockY);
    int width, height;
    int baseWidth = 1029;   
    int baseHeight = 496;  
    parseResolution(Reso, width, height);
    mainWindow.setGeometry(0, 0, width, height);

    int button1_x = 100;
    int button1_y = 10;
    int button1_w = 60;
    int button1_h = 16;

    int button2_x = 10;
    int button2_y = 10;
    int button2_w = 60;
    int button2_h = 16;

    int textEdit_x = 880;
    int textEdit_y = 10;
    int textEdit_w = 141;
    int textEdit_h = 15;

    int dock_x1;
    int dock_x2;
    int dock_y1;
    int dock_y2;
    // Call of widgets points
    calculateDockPoints(dock_x1, dock_x2, dock_y1, dock_y2);


    calculateScaledPosition(baseWidth, baseHeight, button1_x, button1_y, button1_w, button1_h);
    calculateScaledPosition(baseWidth, baseHeight, button2_x, button2_y, button2_w, button2_h);
    calculateScaledPosition(baseWidth, baseHeight, textEdit_x, textEdit_y, textEdit_w, textEdit_h);


    QPushButton* pushButton = new QPushButton("Search", &mainWindow);
    pushButton->setGeometry(button1_x, button1_y, button1_w, button1_h);
    QObject::connect(pushButton, &QPushButton::clicked, []() {
        std::cout << "Search button clicked!" << std::endl;
    });

    QPushButton* options_button = new QPushButton("Poweroff" , &mainWindow);
    options_button->setGeometry(360, button1_y, button1_w, button1_h);
    QObject::connect(options_button, &QPushButton::clicked, [](){
        PowerOff();
    });

    QPushButton* pushButton_2 = new QPushButton("Apps", &mainWindow);
    pushButton_2->setGeometry(button2_x, button2_y, button2_w, button2_h);
    QObject::connect(pushButton_2, &QPushButton::clicked, []() {
        std::cout << "Apps button clicked!" << std::endl;
    });

    QTextEdit* ktextedit = new QTextEdit(&mainWindow);
    ktextedit->setGeometry(textEdit_x, textEdit_y, textEdit_w, textEdit_h);
    ktextedit->setPlainText(Get_Active_Time());
    ktextedit->setReadOnly(true);
    QTimer* timer = new QTimer(&mainWindow);
    QObject::connect(timer, &QTimer::timeout, [&]() {
        ktextedit->setPlainText(Get_Active_Time());
      //  vScrollBar->setValue(vScrollBar->maximum());  // Optional: Auto-scroll to the bottom
    });
    timer->start(1000);  // 1000 ms = 1 second

    QScrollBar* vScrollBar = ktextedit->verticalScrollBar();
    if (vScrollBar) {
        vScrollBar->setVisible(false);
    }


    mainWindow.show();
   
  //  Cursor cursor = XcursorFilenameLoadCursor(display, "/Assets/cursor");
 //   if (cursor == None) {
 //       std::cerr << "Error: Could not load custom cursor" << std::endl;
  //  } else {
  //      Window rootWindow = DefaultRootWindow(display);
   //     XDefineCursor(display, rootWindow, cursor);
   //     XFlush(display);
   //     std::cout << "LOG: Custom cursor set." << std::endl;
   // }
   // customSleep(5);
    
    app.exec();
}

int main() {
    Get_Active_Time();
    std::cout << "LOG: Loading DE!" << std::endl;
    Display* display = queryScreenInformation();
    if (display) {
        DrawDE(display);
    }
    return 0;
}
