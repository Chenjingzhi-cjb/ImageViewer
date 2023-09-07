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


class ImageLabel : public QLabel {
    Q_OBJECT

public:
    ImageLabel(QWidget *qwidget = 0)
            : QLabel(qwidget),
              m_image_size(cv::Size(0, 0)),
              image_type(ImageType::RGB) {
        this->setMouseTracking(true);
    }

    ~ImageLabel() = default;

public:
    void setImageSize(cv::Size image_size) {
        m_image_size = image_size;
    }

private:
    void mouseMoveEvent(QMouseEvent *event) override {
        m_qpixmap = this->pixmap(Qt::ReturnByValue);
        if (!m_qpixmap.isNull()) {
            int label_x = event->x();
            int label_y = event->y();
            int image_x = event->x() - (m_qpixmap.width() - m_image_size.width) / 2;
            int image_y = event->y() - (m_qpixmap.height() - m_image_size.height) / 2;

            QColor color;
            if (label_x >= 0 && label_x < m_qpixmap.width() && label_y >= 0 && label_y < m_qpixmap.height()) {
                color = m_qpixmap.toImage().pixelColor(label_x, label_y);
            }

            std::string result = "XY ";
            result += "(" + std::to_string(image_x) + ", " + std::to_string(image_y) + ") ";
            result += "-> ";

            if (image_type == ImageType::RGB) {
                result += "RGB ";
                result += "(" + std::to_string(color.red()) + ", " + std::to_string(color.green()) + ", " + std::to_string(color.blue()) + ") ";
                result += "#" + uint8ToHex16Str(color.red()) + uint8ToHex16Str(color.green()) + uint8ToHex16Str(color.blue());
            } else if (image_type == ImageType::GRAY) {
                result += "Gray ";
                result += std::to_string(color.green());
            } else if (image_type == ImageType::HSI) {
                result += "HSI ";
                result += "(" + std::to_string(color.blue()) + ", " + std::to_string(color.green()) + ", " + std::to_string(color.red()) + ") ";
                result += "#" + uint8ToHex16Str(color.blue()) + uint8ToHex16Str(color.green()) + uint8ToHex16Str(color.red());
            }

            emit mouseSignal(QString::fromStdString(result));
        }
    }

    // 单字节无符号十进制数 转 双字节十六进制数字符串
    std::string uint8ToHex16Str(int num) {
        if ((num < 0) || (num > 255)) {
            return "XX";
        }

        std::string result = "";

        int quo;
        if ((quo = num / 16) == 0) {
            result += '0';
        } else {  // quo != 0
            if (quo < 10) {
                result += ('0' + quo);
            } else {  // quo >= 10
                quo -= 10;
                result += ('A' + quo);
            }
            num %= 16;
        }

        if (num < 10) {
            result += ('0' + num);
        } else {  // num >= 10
            num -= 10;
            result += ('A' + num);
        }

        return result;
    }

signals:
    void mouseSignal(QString qstring);

private:
    QPixmap m_qpixmap;

    cv::Size m_image_size;

public:
    ImageType image_type;
};


#endif // IMAGELABEL_H
