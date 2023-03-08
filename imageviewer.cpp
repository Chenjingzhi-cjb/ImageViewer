#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    loadSlots();
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::loadSlots()
{
    connect(ui->button_loadimage, &QPushButton::clicked, this, &ImageViewer::buttonLoadImageClicked);

    connect(ui->label_image, &ImageLabel::mouseSignal, this, [=](QString qstring) {
        ui->label_colorvalue->setText(qstring);
    });
}

void ImageViewer::buttonLoadImageClicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp *.tif)"));

    if (!filename.isEmpty()) {
        cv::Mat image = cv::imread(filename.toStdString(), cv::IMREAD_GRAYSCALE);
        if (!image.empty()) {
            // 获取 QLabel 的尺寸
            QSize size = ui->label_image->size();  // 正方形，width == height

            // 获取图像的原尺寸并计算缩放后的尺寸
            int image_width = image.cols;
            int image_height = image.rows;
            int scaled_width;
            int scaled_height;
            if (image_width > image_height) {
                scaled_width = size.width();
                double scale = (double) scaled_width / image_width;
                scaled_height = (int) (image_height * scale);
            } else {  // image_weight <= image_height
                scaled_height = size.width();
                double scale = (double) scaled_height / image_height;
                scaled_width = (int) (image_width * scale);
            }

            // 将图像缩放到合适的尺寸
            cv::Mat scaled_image;
            cv::resize(image, scaled_image, cv::Size(scaled_width, scaled_height));

            // 将 OpenCV 的 Mat 转换为 Qt 的 QImage
            QImage qimage = QImage(scaled_image.data, scaled_image.cols, scaled_image.rows, scaled_image.step, QImage::Format_Grayscale8);  // QImage::Format_RGB888

            // 将 QImage 转化为 QPixmap
            QPixmap qpixmap = QPixmap::fromImage(qimage);

            // 将 QPixmap 居中，并在两侧留白
            QPixmap result_qpixmap(size);
            result_qpixmap.fill(Qt::transparent);
            QPainter painter(&result_qpixmap);
            painter.drawPixmap((size.width() - qpixmap.width()) / 2, (size.height() - qpixmap.height()) / 2, qpixmap);

            // 将 result QPixmap 显示在 QLabel 上
            ui->label_image->setPixmap(result_qpixmap);
        }
    }
}

