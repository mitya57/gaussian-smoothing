#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include "drawwidget.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    DrawWidget drawWidget(QSize(128, 128));

    QVBoxLayout layout;
    layout.addWidget(&drawWidget, 0, Qt::AlignCenter);

    QWidget mainWidget;
    mainWidget.setLayout(&layout);

    QMainWindow window;
    window.setCentralWidget(&mainWidget);
    window.resize(320, 240);
    window.show();

    return app.exec();
}
