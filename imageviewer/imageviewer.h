#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QApplication>
#include <QAction>
#include <QUrl>
#include <QFile>
#include <QFileDialog>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QTextStream>
#include <QString>
#include <QList>

#include <windows.h>
#include <iostream>
#include <string>
#include <cmath>
#include <opencv2/opencv.hpp>

#include "image_label.hpp"
#include "image_processor.hpp"


QT_BEGIN_NAMESPACE

namespace Ui { class ImageViewer; }

QT_END_NAMESPACE


class ImageViewer : public QMainWindow {
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();

public:
    void externalLoad(QString path);

    static bool checkFileType(QString path);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

private slots:
    void on_button_loadimage_clicked();

    void on_button_rgb_clicked();

    void on_button_gray_clicked();

    void on_button_hsi_clicked();

private:
    void loadConnect();

    static std::wstring string2wstring(const std::string &str);

    // 解决中文路径的问题
    static std::vector<char> readImage(const QString &image_path);

private:
    Ui::ImageViewer *ui;

    QString m_image_path;
    ImageProcessor m_processor;
};


#endif // IMAGEVIEWER_H
