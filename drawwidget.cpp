#include <cmath>
#include <QtCore/QQueue>
#include <QtCore/QSet>
#include <QtCore/QTextStream>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include "drawwidget.h"
#include "distarray.h"

static const double sigma = 1;
static const int radius = 5;
static const int diameter = 2 * radius + 1;

double gaussianFilter(double x, double y) {
    return exp(-(x * x + y * y) / (2 * sigma * sigma)) /
           (sigma * sqrt(2 * M_PI));
}

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
        invertSign(event->pos());
    } else if (event->button() == Qt::MiddleButton) {
        smooth();
    } else {
        previousPoint = event->pos();
    }
}

void DrawWidget::invertSign(QPoint const &point) {
    BitArray bitArray(image.bits());
    PixelArray array = {&bitArray, image.size()};

    QQueue<unsigned> processQueue;
    QSet<unsigned> processedSet;

    processQueue.enqueue(array.num(point));

    while (!processQueue.isEmpty()) {
        unsigned num = processQueue.dequeue();
        if (processedSet.contains(num)) {
            continue;
        }
        QPoint ind = array.index2d(num);
        for (unsigned j = 1; j <= 7; j += 2) {
            QPoint neighbour;
            if (!getNeighbour(ind, size(), j, neighbour)) {
                continue;
            }
            unsigned neighbourNum = array.num(neighbour);
            if (!processedSet.contains(neighbourNum) && bitArray.value(neighbourNum)) {
                processQueue.enqueue(neighbourNum);
            }
        }
        image.setPixel(ind, !bitArray.value(num));
        distArray[num] = -distArray[num];
        processedSet.insert(num);
    }
    repaint();
}

void DrawWidget::fillArray() {
    /* Extract the bits */
    BitArray bitArray(image.bits());
    PixelArray array = {&bitArray, image.size()};

    /* And then create a distArray from it */
    memset(gaussArray, 0, width() * height() * sizeof(qreal));

    QTextStream(stdout) << "Filling distance array..." << endl;
    fillDistanceArray(array, distArray);
    QTextStream(stdout) << "Done!" << endl;
}

void DrawWidget::smooth() {
    /* Apply Gaussian filter */
    QTextStream(stdout) << "Applying Gaussian filter..." << endl;
    for (int i = 0; i < width(); ++i) {
        for (int j = 0; j < height(); ++j) {
            for (int k = 0; k < diameter * diameter; ++k) {
                int diff_i = (k / diameter) - radius;
                int diff_j = (k % diameter) - radius;
                int new_i = i + diff_i;
                int new_j = j + diff_j;
                if (new_i > 0 && new_i < height() && new_j > 0 && new_j < width()) {
                    gaussArray[i * width() + j] +=
                        distArray[new_i * width() + new_j] * gaussianFilter(diff_i, diff_j);
                }
            }
        }
    }
    QTextStream(stdout) << "Done!" << endl;

    repaint();
}
