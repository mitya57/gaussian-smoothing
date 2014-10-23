#include <QtCore/QPoint>
#include <QtGui/QImage>
#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>
#include <QtWidgets/QWidget>

class DrawWidget: public QWidget {
    Q_OBJECT
private:
    QImage image;
    QPoint previousPoint;
    qreal *distArray;
    qreal *gaussArray;
    void invertSign(QPoint const &point);

    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
public:
    DrawWidget(QSize size, QWidget *parent = 0):
        QWidget(parent),
        image(size, QImage::Format_Mono),
        distArray(new qreal[size.width() * size.height()]),
        gaussArray(new qreal[size.width() * size.height()])
    {
        setFixedSize(size);
        image.fill(Qt::color1);
    }
    ~DrawWidget() {
        delete[] distArray;
        delete[] gaussArray;
    }
public slots:
    void fillArray();
    void smooth();
};
