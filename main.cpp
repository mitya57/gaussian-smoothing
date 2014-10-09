#include <QtWidgets/QApplication>
#include "drawwidget.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    DrawWidget widget;
    widget.show();
    return app.exec();
}
