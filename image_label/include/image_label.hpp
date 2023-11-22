#ifndef IMAGE_LABEL_HPP
#define IMAGE_LABEL_HPP

#include <QMainWindow>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <opencv2/opencv.hpp>


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
              m_image_type(ImageType::RGB) {
        this->setMouseTracking(true);
    }

    ~ImageLabel() = default;

public:
    void showImage(cv::Mat &image, ImageType image_type) {
        if (!image.empty()) {
            m_image_type = image_type;

            // 获取 QLabel 的尺寸
            QSize qlabel_size = this->size();  // 正方形，width == height

            // 获取图像的原尺寸并计算缩放后的尺寸
            int image_width = image.cols;
            int image_height = image.rows;
            int scaled_width;
            int scaled_height;
            if (image_width > image_height) {
                scaled_width = qlabel_size.width();
                double scale = (double) scaled_width / image_width;
                scaled_height = (int) (image_height * scale);
            } else {  // image_weight <= image_height
                scaled_height = qlabel_size.width();
                double scale = (double) scaled_height / image_height;
                scaled_width = (int) (image_width * scale);
            }

            // 将图像缩放到合适的尺寸
            cv::Mat scaled_image;
            cv::resize(image, scaled_image, cv::Size(scaled_width, scaled_height));

            // 记录缩放后的图像尺寸
            m_image_size = scaled_image.size();

            // 将 OpenCV 的 Mat 转换为 Qt 的 QImage
            QImage qimage;
            if (image_type == ImageType::RGB || image_type == ImageType::HSI) {
                qimage = QImage(scaled_image.data, scaled_image.cols, scaled_image.rows, scaled_image.step, QImage::Format_BGR888);
            } else if (image_type == ImageType::GRAY) {
                qimage = QImage(scaled_image.data, scaled_image.cols, scaled_image.rows, scaled_image.step, QImage::Format_Grayscale8);
            } else {  // Error
                return;
            }

            // 将 QImage 转化为 QPixmap
            QPixmap qpixmap = QPixmap::fromImage(qimage);

            // 将 QPixmap 居中，并在两侧留白
            QPixmap result_qpixmap(qlabel_size);
            result_qpixmap.fill(Qt::transparent);
            QPainter painter(&result_qpixmap);
            painter.drawPixmap((qlabel_size.width() - qpixmap.width()) / 2, (qlabel_size.height() - qpixmap.height()) / 2, qpixmap);

            // 显示 result QPixmap
            this->setPixmap(result_qpixmap);
        }
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

            if (m_image_type == ImageType::RGB) {
                result += "RGB ";
                result += "(" + std::to_string(color.red()) + ", " + std::to_string(color.green()) + ", " + std::to_string(color.blue()) + ") ";
                result += "#" + uint8ToHex16Str(color.red()) + uint8ToHex16Str(color.green()) + uint8ToHex16Str(color.blue());
            } else if (m_image_type == ImageType::GRAY) {
                result += "Gray ";
                result += std::to_string(color.green());
            } else if (m_image_type == ImageType::HSI) {
                result += "HSI ";
                result += "(" + std::to_string(color.blue()) + ", " + std::to_string(color.green()) + ", " + std::to_string(color.red()) + ") ";
                result += "#" + uint8ToHex16Str(color.blue()) + uint8ToHex16Str(color.green()) + uint8ToHex16Str(color.red());
            }

            emit signalMouseMove(QString::fromStdString(result));
        }
    }

    // 单字节无符号十进制数 转 双字节十六进制数字符串
    static std::string uint8ToHex16Str(int num) {
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
    void signalMouseMove(QString qstring);

private:
    QPixmap m_qpixmap;

    cv::Size m_image_size;
    ImageType m_image_type;
};


#endif // IMAGE_LABEL_HPP
