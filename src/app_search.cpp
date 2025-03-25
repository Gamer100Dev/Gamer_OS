#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <pwd.h>
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

int main(){
    int argc = 0;
    char *argv[] = { nullptr };
    QApplication app(argc, argv);
    QMainWindow window;
    window.setGeometry(50, 50, 400, 600);
    QTextEdit* text_box = new QTextEdit("", &window);
    text_box->setGeometry(10, 10, 380, 40);
    QPushButton* button_1 = new QPushButton("Launch!", &window);
    button_1->setGeometry(150, 60, 100, 35);

    window.show();
    return app.exec();
}
