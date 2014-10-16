#ifndef __DISTARRAY_H
#define __DISTARRAY_H

#include <QtCore/QPoint>
#include <QtCore/QPointF>
#include <QtCore/QSize>
#include <QtCore/QSizeF>

struct PixelArray {
    unsigned char const *pixels;
    QPointF start;
    QSizeF pixelSize;
    QSize size;

    inline bool verifyIndex(QPoint const &index) const {
        return (index.x() > 0 && index.x() < size.width() &&
                index.y() > 0 && index.y() < size.height());
    }
    inline unsigned num(QPoint const &index) const {
        return index.y() * size.width() + index.x();
    }
    inline QPoint index2d(unsigned index) const {
        return QPoint(
            index % size.width(),
            index / size.width()
        );
    }
    inline unsigned char operator [](QPoint const &index) const {
        return pixels[num(index)];
    }
    inline QPointF getPixelCenter(QPoint const &index) const {
        return QPointF(
           start.x() + pixelSize.width() * (.5 + index.x()),
           start.y() + pixelSize.height() * (.5 + index.y())
        );
    }
};

void fillDistanceArray(PixelArray const &array,
                       qreal            *distArray,
                       bool             reverse = false);

#endif /* __DISTARRAY_H */
