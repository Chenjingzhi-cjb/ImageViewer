#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QApplication>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QPainter>
#include "imagelabel.hpp"
#include <windows.h>
#include <iostream>
#include <string>
#include <cmath>

QT_BEGIN_NAMESPACE
namespace Ui { class ImageViewer; }
QT_END_NAMESPACE

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();

private:
    void loadSlots();

    std::wstring string2wstring(const std::string &str);

    std::vector<char> readImage(const QString &image_path);

    void showImage(cv::Mat &image, ImageType image_type);

private slots:
    void buttonLoadImageClicked();

    void on_button_rgb_clicked();

    void on_button_gray_clicked();

    void on_button_hsi_clicked();

private:
    Ui::ImageViewer *ui;

    QString m_image_path;
};
#endif // IMAGEVIEWER_H
