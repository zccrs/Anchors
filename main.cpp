#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QDebug>
#include <QMetaClassInfo>
#include "anchors.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Anchors<MainWindow> *w = new Anchors<MainWindow>;

    (*w)->show();
    //w.setTop(wtop());

    //qDebug()<<w.errorCode()<<w.errorString();

    return a.exec();
}
