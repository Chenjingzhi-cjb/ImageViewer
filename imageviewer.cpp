#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::ImageViewer) {
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/ui/Resource/icon2.ico"));

    loadSlots();
}

ImageViewer::~ImageViewer() {
    delete ui;
}

void ImageViewer::loadSlots() {
    connect(ui->button_loadimage, &QPushButton::clicked, this, &ImageViewer::buttonLoadImageClicked);

    connect(ui->label_image, &ImageLabel::mouseSignal, this, [=](QString qstring) {
        ui->label_colorvalue->setText(qstring);
    });
}

std::wstring ImageViewer::string2wstring(const std::string &str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wstr(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, const_cast<wchar_t *>(wstr.data()), len);
    wstr.resize(wcslen(wstr.c_str()));
    return wstr;
}

// 解决中文路径的问题
std::vector<char> ImageViewer::readImage(const QString &image_path) {
    FILE* f = _wfopen(string2wstring(image_path.toStdString()).c_str(), L"rb");
    fseek(f, 0, SEEK_END);
    size_t buffer_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    std::vector<char> buffer(buffer_size);
    fread(&buffer[0], sizeof(char), buffer_size, f);
    fclose(f);

    return std::move(buffer);
}

void ImageViewer::showImage(cv::Mat &image, ImageType image_type) {
    if (!image.empty()) {
        ui->label_image->image_type = image_type;

        // 获取 QLabel 的尺寸
        QSize qlabel_size = ui->label_image->size();  // 正方形，width == height

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

        // 将缩放后的图像尺寸传入 ImageLabel
        ui->label_image->setImageSize(scaled_image.size());

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

        // 将 result QPixmap 显示在 QLabel 上
        ui->label_image->setPixmap(result_qpixmap);
    }
}

void ImageViewer::buttonLoadImageClicked() {
    m_image_path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp *.tif)"));

    on_button_rgb_clicked();
}

void ImageViewer::on_button_rgb_clicked() {
    if (!m_image_path.isEmpty()) {
        cv::Mat image = cv::imdecode(readImage(m_image_path), cv::IMREAD_COLOR);

        showImage(image, ImageType::RGB);
    }
}

void ImageViewer::on_button_gray_clicked() {
    if (!m_image_path.isEmpty()) {
        cv::Mat image = cv::imdecode(readImage(m_image_path), cv::IMREAD_GRAYSCALE);

        showImage(image, ImageType::GRAY);
    }
}

void ImageViewer::on_button_hsi_clicked() {
    if (!m_image_path.isEmpty()) {
        cv::Mat image = cv::imdecode(readImage(m_image_path), cv::IMREAD_COLOR);

        cv::Mat image_hsi(image.size(), image.type());

        for (int r = 0; r < image.rows; ++r) {
            for (int c = 0; c < image.cols; ++c) {
                cv::Vec3b m = image.at<cv::Vec3b>(r, c);
                double B = (double) m[0] / 255;
                double G = (double) m[1] / 255;
                double R = (double) m[2] / 255;

                double H, S, I;

                double den = sqrt((R - G) * (R - G) + (R - B) * (G - B));
                if (den == 0) {
                    H = 0;
                } else {  // den != 0
                    double theta = acos((R - G + R - B) / (2 * den));
                    H = (B <= G) ? (theta / (2 * M_PI)) : (1 - theta / (2 * M_PI));
                }

                double sum = B + G + R;
                if (sum == 0) {
                    S = 0;
                } else {  // sum != 0
                    S = 1 - 3 * std::min(std::min(B, G), R) / sum;
                }

                I = sum / 3.0;

                image_hsi.at<cv::Vec3b>(r, c) = cv::Vec3b((uchar) (H * 255), (uchar) (S * 255), (uchar) (I * 255));
            }
        }

        showImage(image_hsi, ImageType::HSI);
    }
}
