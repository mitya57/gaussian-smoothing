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

    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
public:
    DrawWidget(QSize size, QWidget *parent = 0):
        QWidget(parent),
        image(size, QImage::Format_Mono)
    {
        setFixedSize(size);
        image.fill(Qt::color1);
    }
public slots:
    void smooth();
};
