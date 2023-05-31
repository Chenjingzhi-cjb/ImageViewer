#include "imageviewer.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ImageViewer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    ImageViewer w;
    if (argc > 1) {  // 捕获输入参数，实现图片拖入可执行文件(exe)运行
        QString path = QString::fromUtf8(argv[1]);

        if (w.checkFileType(path)) {
            w.externalLoad(path);
        }
    }
    w.show();

    return a.exec();
}
