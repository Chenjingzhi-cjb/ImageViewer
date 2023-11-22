#include "imageviewer.h"
#include "ui_imageviewer.h"


ImageViewer::ImageViewer(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::ImageViewer) {
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/ui/resource/icon2.ico"));

    setAcceptDrops(true);  // 开启窗口拖拽

    loadConnect();
}

ImageViewer::~ImageViewer() {
    delete ui;
}

void ImageViewer::externalLoad(QString path) {
    m_image_path = std::move(path);

    on_button_rgb_clicked();
}

bool ImageViewer::checkFileType(QString path) {
    QList<QString> file_types = {"png", "jpg", "bmp", "tif"};
    QString path_file_type = path.right(3);  // 目前均为 3

    for (const auto &file_type : file_types) {
        if (!path_file_type.compare(file_type)) {
            return true;
        }
    }

    return false;
}

void ImageViewer::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        if (checkFileType(event->mimeData()->urls().first().fileName())) {
            event->acceptProposedAction();
        } else {
            event->ignore();
        }
    } else {
        event->ignore();
    }
}

void ImageViewer::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        m_image_path = urlList.first().toLocalFile();

        on_button_rgb_clicked();
    }
}

void ImageViewer::on_button_loadimage_clicked() {
    m_image_path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp *.tif)"));

    on_button_rgb_clicked();
}

void ImageViewer::on_button_rgb_clicked() {
    if (!m_image_path.isEmpty()) {
        cv::Mat image = cv::imdecode(readImage(m_image_path), cv::IMREAD_COLOR);

        ui->label_image->showImage(image, ImageType::RGB);
    }
}

void ImageViewer::on_button_gray_clicked() {
    if (!m_image_path.isEmpty()) {
        cv::Mat image = cv::imdecode(readImage(m_image_path), cv::IMREAD_GRAYSCALE);

        ui->label_image->showImage(image, ImageType::GRAY);
    }
}

void ImageViewer::on_button_hsi_clicked() {
    if (!m_image_path.isEmpty()) {
        cv::Mat image = cv::imdecode(readImage(m_image_path), cv::IMREAD_COLOR);

        cv::Mat image_hsi;
        m_processor.bgrToHsi(image, image_hsi);

        ui->label_image->showImage(image_hsi, ImageType::HSI);
    }
}

void ImageViewer::loadConnect() {
    connect(ui->label_image, &ImageLabel::signalMouseMove, this, [=](QString qstring) {
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
