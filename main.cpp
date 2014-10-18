#include <QtWidgets/QApplication>
#include "drawwidget.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    DrawWidget widget(QSize(128, 128));
    widget.show();
    return app.exec();
}
