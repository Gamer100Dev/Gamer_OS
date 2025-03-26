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
#include <QMouseEvent>
using namespace std;
void print(std::string& string){
    std::cout << "LOG: " << string << std::endl;
}
int Launch_App(std::string& app_name){
    bool App_Launched_so = system(app_name.c_str());
    if (App_Launched_so == 0){
        print("Ran Successfully! 0 ");
    } else {
        print("Error while running app! 1");
    }
}
class DraggableWindow : public QMainWindow {
    Q_OBJECT

public:
    DraggableWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        this->setGeometry(50, 50, 400, 600);
        QTextEdit* text_box = new QTextEdit("", this);
        text_box->setGeometry(10, 10, 380, 40);
        QPushButton* button_1 = new QPushButton("Launch!", this);
        button_1->setGeometry(150, 60, 100, 35);

        QObject::connect(button_1, &QPushButton::clicked, [&, text_box]() {
            std::string app_name = text_box->toPlainText().toStdString();
            Launch_App(app_name);
        });

        m_isDragging = false;
        m_dragStartPos = QPoint(0, 0);
    }

protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            m_isDragging = true;
            m_dragStartPos = event->globalPos() - this->frameGeometry().topLeft();
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        if (m_isDragging) {
            this->move(event->globalPos() - m_dragStartPos);
        }
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            m_isDragging = false;
        }
    }

private:
    bool m_isDragging;
    QPoint m_dragStartPos;
};
int main(){
    int argc = 0;
    char *argv[] = { nullptr };
    QApplication app(argc, argv);
    DraggableWindow window;
    window.show();
    return app.exec();
}