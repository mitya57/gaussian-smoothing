#include <cmath>
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
    qreal *gaussArray = new qreal[width * height];
    memset(gaussArray, 0, width * height * sizeof(qreal));

    QTextStream(stdout) << "Filling distance array..." << endl;
    fillDistanceArray(array, distArray);

    /* Apply Gaussian filter */
    QTextStream(stdout) << "Applying Gaussian filter..." << endl;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            for (int k = 0; k < diameter * diameter; ++k) {
                int diff_i = (k / diameter) - radius;
                int diff_j = (k % diameter) - radius;
                int new_i = i + diff_i;
                int new_j = j + diff_j;
                if (new_i > 0 && new_i < height && new_j > 0 && new_j < height) {
                    gaussArray[i * width + j] +=
                        distArray[new_i * width + new_j] * gaussianFilter(diff_i, diff_j);
                }
            }
        }
    }
    QTextStream(stdout) << "Done!" << endl;

    image = QImage(size(), QImage::Format_RGB32);

    // FIXME: access pixel data directly, for speed
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            int index = j * width + i;
            image.setPixel(i, j, QColor(Qt::white).darker(100 * gaussArray[index]).rgb());
        }
    }

    delete[] distArray;
    delete[] gaussArray;
    repaint();
}
