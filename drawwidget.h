#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainterPath>
#include <QtWidgets/QWidget>

class DrawWidget: public QWidget {
    Q_OBJECT
private:
    QPainterPath painterPath;
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void smooth();
};
