#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QDebug>
#include "anchors.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Anchors<MainWindow> w;
    w.show();

    return a.exec();
}
