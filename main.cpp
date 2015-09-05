#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QDebug>
#include <QMetaClassInfo>
#include "anchors.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Anchors<MainWindow> w;
    w->show();

    QRect rect(100, 100, 100, 100);

    rect.marginsAdded(QMargins(10, 20, 30, 40));

    qDebug()<<rect;

    return a.exec();
}
