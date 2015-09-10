#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "anchors.h"
#include "dragwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Anchors<MainWindow> w = new MainWindow;
    w->resize(500, 500);

    Anchors<DragWidget> ww = new DragWidget(w);
    ww->setAutoFillBackground(true);
    ww->resize(100, 100);
    ww->setStyleSheet("QWidget{background-color:green;}");
    ww->show();

    Anchors<DragWidget> widget = new DragWidget(w);
    widget->setStyleSheet("QWidget{background-color:red;}");
    widget->resize(100, 100);

    //ww.setBottom(w.bottom());
   // ww.setBottomMargin(10);

    //ww.setVerticalCenter(widget.bottom());
    //ww.setTop(widget.bottom());
    ww.setRight(widget.right());
    //ww.setBottom(widget.bottom());
    //ww.setTopMargin(-1);
    //ww.setVerticalCenterOffset(10);
    ww.setMargins(10);
    ww.setTopMargin(5);
    //ww->raise();
    qDebug()<<ww.errorString();
    //ww.setRightMargin(-1);

    //ww.moveLeft(widget->rect().right());

    //widget.setVerticalCenter(100, Qt::AnchorTop);

    w->show();

    return a.exec();
}
