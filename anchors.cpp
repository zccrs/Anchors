#include <QDebug>

#include "anchors.h"

const AnchorsBase *AnchorsBase::anchors() const
{
    return this;
}

const AnchorsBase::AnchorsInfo *AnchorsBase::top() const
{
    return data->top;
}

const AnchorsBase::AnchorsInfo *AnchorsBase::bottom() const
{
    return data->bottom;
}

const AnchorsBase::AnchorsInfo *AnchorsBase::left() const
{
    return data->left;
}

const AnchorsBase::AnchorsInfo *AnchorsBase::right() const
{
    return data->right;
}

const AnchorsBase::AnchorsInfo *AnchorsBase::horizontalCenter() const
{
    return data->horizontalCenter;
}

const AnchorsBase::AnchorsInfo *AnchorsBase::verticalCenter() const
{
    return data->verticalCenter;
}

const QWidget *AnchorsBase::fill() const
{
    return data->fill;
}

const QWidget *AnchorsBase::centerIn() const
{
    return data->centerIn;
}

int AnchorsBase::margins() const
{
    return data->margins;
}

int AnchorsBase::topMargin() const
{
    return data->topMargin;
}

int AnchorsBase::bottomMargin() const
{
    return data->bottomMargin;
}

int AnchorsBase::leftMargin() const
{
    return data->leftMargin;
}

int AnchorsBase::rightMargin() const
{
    return data->rightMargin;
}

int AnchorsBase::horizontalCenterOffset() const
{
    return data->horizontalCenterOffset;
}

int AnchorsBase::verticalCenterOffset() const
{
    return data->verticalCenterOffset;
}

int AnchorsBase::alignWhenCentered() const
{
    return data->alignWhenCentered;
}

void AnchorsBase::setTop(const AnchorsBase::AnchorsInfo* top)
{
    if (top == NULL || *data->top == top)
        return;

    *data->top = top;
    emit topChanged(data->top);
}

void AnchorsBase::setBottom(const AnchorsBase::AnchorsInfo* bottom)
{
    if (bottom == NULL || *data->bottom == bottom)
        return;

    *data->bottom = bottom;
    emit bottomChanged(data->bottom);
}

void AnchorsBase::setLeft(const AnchorsBase::AnchorsInfo* left)
{
    if (left == NULL || *data->left == left)
        return;

    *data->left = left;
    emit leftChanged(data->left);
}

void AnchorsBase::setRight(const AnchorsBase::AnchorsInfo* right)
{
    if (right == NULL || *data->right == right)
        return;

    *data->right = right;
    emit rightChanged(data->right);
}

void AnchorsBase::setHorizontalCenter(const AnchorsBase::AnchorsInfo* horizontalCenter)
{
    if (horizontalCenter == NULL || *data->horizontalCenter == horizontalCenter)
        return;

    *data->horizontalCenter = horizontalCenter;
    emit horizontalCenterChanged(data->horizontalCenter);
}

void AnchorsBase::setVerticalCenter(const AnchorsBase::AnchorsInfo* verticalCenter)
{
    if (verticalCenter == NULL || *data->verticalCenter == verticalCenter)
        return;

    *data->verticalCenter = verticalCenter;
    emit verticalCenterChanged(data->verticalCenter);
}

void AnchorsBase::setFill(const QWidget* fill)
{
    if (data->fill == fill)
        return;

    data->fill = fill;
    emit fillChanged(fill);
}

void AnchorsBase::setCenterIn(const QWidget* centerIn)
{
    if (data->centerIn == centerIn)
        return;

    data->centerIn = centerIn;
    emit centerInChanged(centerIn);
}

void AnchorsBase::setMargins(int margins)
{
    if (data->margins == margins)
        return;

    data->margins = margins;
    emit marginsChanged(margins);
}

void AnchorsBase::setTopMargin(int topMargin)
{
    if (data->topMargin == topMargin)
        return;

    data->topMargin = topMargin;
    emit topMarginChanged(topMargin);
}

void AnchorsBase::setBottomMargin(int bottomMargin)
{
    if (data->bottomMargin == bottomMargin)
        return;

    data->bottomMargin = bottomMargin;
    emit bottomMarginChanged(bottomMargin);
}

void AnchorsBase::setLeftMargin(int leftMargin)
{
    if (data->leftMargin == leftMargin)
        return;

    data->leftMargin = leftMargin;
    emit leftMarginChanged(leftMargin);
}

void AnchorsBase::setRightMargin(int rightMargin)
{
    if (data->rightMargin == rightMargin)
        return;

    data->rightMargin = rightMargin;
    emit rightMarginChanged(rightMargin);
}

void AnchorsBase::setHorizontalCenterOffset(int horizontalCenterOffset)
{
    if (data->horizontalCenterOffset == horizontalCenterOffset)
        return;

    data->horizontalCenterOffset = horizontalCenterOffset;
    emit horizontalCenterOffsetChanged(horizontalCenterOffset);
}

void AnchorsBase::setVerticalCenterOffset(int verticalCenterOffset)
{
    if (data->verticalCenterOffset == verticalCenterOffset)
        return;

    data->verticalCenterOffset = verticalCenterOffset;
    emit verticalCenterOffsetChanged(verticalCenterOffset);
}

void AnchorsBase::setAlignWhenCentered(bool alignWhenCentered)
{
    if (data->alignWhenCentered == alignWhenCentered)
        return;

    data->alignWhenCentered = alignWhenCentered;
    emit alignWhenCenteredChanged(alignWhenCentered);
}

void AnchorsBase::onXChanged(int x)
{
    qDebug()<<"xchanged:"<<x<<sender();
}

void AnchorsBase::onYChanged(int y)
{
    qDebug()<<"ychanged:"<<y<<sender();
}

void AnchorsBase::onWidthChanged(int width)
{
    qDebug()<<"widthchanged:"<<width<<sender();
}

void AnchorsBase::onHeightChanged(int height)
{
    qDebug()<<"heightchanged:"<<height<<sender();
}

bool AnchorsBase::eventFilter(QObject *o, QEvent *e)
{
    if( o == data->widget ){
        if(e->type() == QEvent::Resize){
            QResizeEvent *event = static_cast<QResizeEvent*>(e);
            if(event){
                QSize size = event->size();

                if(size.width() != data->widget_old_size.width())
                    onWidthChanged(size.width());

                if(size.height() != data->widget_old_size.height())
                    onHeightChanged(size.height());

                data->widget_old_size = size;
            }
        }else if(e->type() == QEvent::Move){
            QMoveEvent *event = static_cast<QMoveEvent*>(e);

            if(event){
                QPoint pos = event->pos();

                if(pos.x() != data->widget_old_pos.x())
                    onXChanged(pos.x());

                if(pos.y() != data->widget_old_pos.y())
                    onYChanged(pos.y());

                data->widget_old_pos = pos;
            }
        }
    }

    return false;
}
