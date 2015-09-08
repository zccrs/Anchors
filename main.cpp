#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "anchors.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Anchors<MainWindow> *w = new Anchors<MainWindow>;
    (*w)->resize(300, 300);
    (*w)->show();

    Anchors<QWidget> ww = new QWidget(w->widget());
    ww->setAutoFillBackground(true);
    ww->resize(100, 100);
    ww->setStyleSheet("QWidget{background-color:red;}");
    ww->show();

    ww.setHorizontalCenter(w->horizontalCenter());
    ww.setHorizontalCenterOffset(10);
    ww.setMargins(10);
    ww.setTopMargin(50);
    ww.setFill(w->widget());
    qDebug() << ww->geometry();
    ww.setBottom(w->bottom());
    qDebug() << ww.errorString();

    return a.exec();
}
