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
    window.setGeometry(0,0,600,300);
    QTextEdit* text_box = new QTextEdit("",&window);
    textbox.setGeometry(10,10,100,100);
    QTextButton* button_1 = new QTextButton("Launch!",&window);
    button_1.setGeometry(-3,0,50,50);
    
    window.show();
    return app.exec();
}