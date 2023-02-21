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
            cv::resize(image, image, cv::Size(ui->label_image->width(), ui->label_image->height()));
            QImage qimage = QImage((const unsigned char*)(image.data), image.cols, image.rows, QImage::Format_Indexed8);

            ui->label_image->setPixmap(QPixmap::fromImage(qimage));
        }
    }
}

