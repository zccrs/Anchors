#include <QMouseEvent>
#include <QPainter>

#include "dragwidget.h"

DragWidget::DragWidget(QWidget *parent) :
    QFrame(parent)
{

}

void DragWidget::mousePressEvent(QMouseEvent *e)
{
    press_pos = e->pos();

    QFrame::mousePressEvent(e);
}

void DragWidget::mouseMoveEvent(QMouseEvent *e)
{
    move(pos() + e->pos() - press_pos);

    QFrame::mouseMoveEvent(e);
}

void DragWidget::paintEvent(QPaintEvent *e)
{
    QFrame::paintEvent(e);

    QPainter pa(this);
    pa.drawText(rect(), QString("%1,%2\n%3,%4").arg(x()).arg(y()).arg(width()).arg(height()));
    pa.end();
}
