#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QMouseEvent>


enum ImageType {
    RGB,
    GRAY,
    HSI
};


class ImageLabel : public QLabel
{
    Q_OBJECT

public:
    ImageLabel(QWidget *qwidget = 0)
        : QLabel(qwidget),
          image_type(ImageType::RGB) {
        this->setMouseTracking(true);
    }

    ~ImageLabel() = default;

private:
    void mouseMoveEvent(QMouseEvent *event) override
    {
        m_qpixmap = this->pixmap(Qt::ReturnByValue);
        if (!m_qpixmap.isNull()) {
            int x = event->x();
            int y = event->y();

            QColor color;
            if (x >= 0 && x < m_qpixmap.width() && y >= 0 && y < m_qpixmap.height()) {
                color = m_qpixmap.toImage().pixelColor(x, y);
            }

            std::string result = " (" + std::to_string(x) + ", " + std::to_string(y) + ") ";
            result += "->";

            if (image_type == ImageType::RGB || image_type == ImageType::GRAY) {
                result += " (" + std::to_string(color.red()) + ", " + std::to_string(color.green()) + ", " + std::to_string(color.blue()) + ") ";
            } else if (image_type == ImageType::HSI) {
                result += " (" + std::to_string(color.blue()) + ", " + std::to_string(color.green()) + ", " + std::to_string(color.red()) + ") ";
            }

            emit mouseSignal(QString::fromStdString(result));
        }
    }

signals:
    void mouseSignal(QString qstring);

private:
    QPixmap m_qpixmap;

public:
    ImageType image_type;
};


#endif // IMAGELABEL_H
