#include <QtCore/QTextStream>
#include <QtGui/QPainter>
#include "drawwidget.h"
#include "distarray.h"

void DrawWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter(this).drawPath(painterPath);
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event) {
    painterPath.lineTo(event->localPos());
    repaint();
}

void DrawWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
      smooth();
    } else {
      painterPath.moveTo(event->localPos());
    }
}

void DrawWidget::smooth() {
    /* First, save the painter path to a QImage */
    QImage image(size(), QImage::Format_Mono);
    QPainter painter;
    painter.begin(&image);
    painter.drawPath(painterPath);
    painter.end();

    /* Then, extract the bits */
    unsigned width = image.width();
    unsigned height = image.height();

    BitArray bitArray(image.bits());

#ifndef NO_DEBUG
    QTextStream cout(stdout);
    for (unsigned i = 0; i < height; ++i) {
        for (unsigned j = 0; j < width; ++j) {
            cout << bitArray.value(i * width + j);
        }
        cout << endl;
    }
#endif

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
