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
#include "imagelabel.hpp"
#include <string>

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

private slots:
    void buttonLoadImageClicked();

//    void labelGrayValueFeedback();

private:
    Ui::ImageViewer *ui;
};
#endif // IMAGEVIEWER_H
