#include <QtCore/QTextStream>
#include <QtGui/QColor>
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
    int width = image.width();
    int height = image.height();

    BitArray bitArray(image.bits());

    PixelArray array = {
        &bitArray,
        QPointF(0., 0.),
        QSizeF(1., 1.),
        image.size()
    };

    /* And then create a distArray from it */
    qreal *distArray = new qreal[width * height];

    QTextStream(stdout) << "Filling distance array..." << endl;
    fillDistanceArray(array, distArray);
    QTextStream(stdout) << "Done!" << endl;

    image = QImage(size(), QImage::Format_RGB32);

    // FIXME: access pixel data directly, for speed
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            int index = j * width + i;
            if (distArray[index] < -.5) {
                image.setPixel(i, j, 0xeeeeff);
            } else {
                image.setPixel(i, j, QColor(Qt::white).darker(100 * distArray[index]).rgb());
            }
        }
    }

    delete[] distArray;
    repaint();
}
