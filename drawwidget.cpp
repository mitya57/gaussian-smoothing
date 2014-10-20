#include <QtCore/QTextStream>
#include <QtGui/QPainter>
#include "drawwidget.h"
#include "distarray.h"

void DrawWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter(this).drawImage(rect(), image);
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event) {
    QPainter painter;
    painter.begin(&image);
    painter.drawLine(previousPoint, event->pos());
    painter.end();
    repaint();
    previousPoint = event->pos();
}

void DrawWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        smooth();
    } else {
        previousPoint = event->pos();
    }
}

void DrawWidget::smooth() {
    /* Extract the bits */
    unsigned width = image.width();
    unsigned height = image.height();

    BitArray bitArray(image.bits());

    PixelArray array = {
        &bitArray,
        QPointF(0., 0.),
        QSizeF(1., 1.),
        image.size()
    };

    /* And then create a distArray from it */
    qreal *distArray = new qreal[width * height];
    fillDistanceArray(array, distArray);

    delete[] distArray;
}
