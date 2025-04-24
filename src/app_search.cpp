#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <pwd.h>
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QString>

using namespace std;

void print(std::string string) {
    std::cout << "LOG: " << string << std::endl;
}

int Launch_App(std::string& app_name) {
    bool App_Launched_so = system(app_name.c_str());
    if (App_Launched_so == 1) {
        print("Error while running app! 1");
        print("Ran Successfully! 0 ");
    } else {
        print("Ran Successfully! 0 ");
    }
    return 0;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    

    QMainWindow mainWindow;
    mainWindow.setGeometry(50, 50, 400, 200);

  
    QTextEdit* text_box = new QTextEdit("", &mainWindow);
    text_box->setGeometry(10, 10, 380, 70);

   
    QPushButton* button_1 = new QPushButton("Launch!", &mainWindow);
    button_1->setGeometry(150, 100, 99, 50);

    QObject::connect(button_1, &QPushButton::clicked, [&, text_box]() {
        std::string app_name = text_box->toPlainText().toStdString();
        Launch_App(app_name);
    });

  
    mainWindow.show();
    
    return app.exec();
}
