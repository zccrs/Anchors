#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QFrame>

class DragWidget : public QFrame
{
    Q_OBJECT
public:
    explicit DragWidget(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);

private:
    QPoint press_pos;
};

#endif // DRAGWIDGET_H
