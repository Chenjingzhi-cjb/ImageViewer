#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QMouseEvent>

class ImageLabel : public QLabel
{
    Q_OBJECT

public:
    ImageLabel(QWidget *qwidget = 0)
        : QLabel(qwidget) {}

    ~ImageLabel() = default;

protected:
    void mouseMoveEvent(QMouseEvent *event) override
    {
        QPixmap qpixmap = this->pixmap(Qt::ReturnByValue);
        if (!qpixmap.isNull()) {
            point_x = event->x();
            point_y = event->y();
            if (point_x >= 0 && point_x < qpixmap.width() && point_y >= 0 && point_y < qpixmap.height()) {
                gray_value = qpixmap.toImage().pixelColor(point_x, point_y).green();
            }

            std::string result = "(" + std::to_string(point_x) + ", " + std::to_string(point_y) + ")" + " " + std::to_string(gray_value);

            emit mouseSignal(QString::fromStdString(result));
        }
    }

signals:
    void mouseSignal(QString qstring);

private:
    int point_x = 0;
    int point_y = 0;
    int gray_value = 0;
};



#endif // IMAGELABEL_H
