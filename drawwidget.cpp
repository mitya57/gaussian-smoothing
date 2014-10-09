#include <QtGui/QPainter>
#include "drawwidget.h"

void DrawWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter(this).drawPath(painterPath);
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event) {
    painterPath.lineTo(event->localPos());
    repaint();
}

void DrawWidget::mousePressEvent(QMouseEvent *event) {
    painterPath.moveTo(event->localPos());
}
