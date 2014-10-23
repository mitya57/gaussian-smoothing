#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include "drawwidget.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    QMainWindow window;
    QWidget mainWidget;
    QVBoxLayout layout;
    DrawWidget drawWidget(QSize(128, 128));

    layout.addWidget(&drawWidget, 0, Qt::AlignCenter);

    mainWidget.setLayout(&layout);

    QAction fillArrayAction("Fill array", &window);
    QAction smoothAction("Smooth", &window);

    QObject::connect(&fillArrayAction, &QAction::triggered, [&]() {
        drawWidget.setEnabled(false);
        fillArrayAction.setEnabled(false);
        drawWidget.fillArray();
        smoothAction.setEnabled(true);
        drawWidget.setEnabled(true);
    });

    smoothAction.setEnabled(false);
    QObject::connect(&smoothAction, &QAction::triggered, [&]() {
        smoothAction.setEnabled(false);
        drawWidget.smooth();
        drawWidget.setEnabled(false);
    });

    QToolBar toolBar;
    toolBar.addAction(&fillArrayAction);
    toolBar.addAction(&smoothAction);

    window.addToolBar(&toolBar);
    window.setCentralWidget(&mainWidget);
    window.resize(320, 240);
    window.show();

    return app.exec();
}
