#include "anchors.h"
#include <QDebug>

AnchorsBasePrivate::AnchorsBasePrivate(Anchors<QWidget> *parent):
    QObject(),
    m_fill(0),
    m_centerIn(0),
    m_margins(0),
    m_topMargin(0),
    m_bottomMargin(0),
    m_leftMargin(0),
    m_rightMargin(0),
    m_horizontalCenterOffset(0),
    m_verticalCenterOffset(0),
    m_alignWhenCentered(false),
    q_ptr(parent)
{
    m_top = new AnchorsBase::AnchorsInfo(q_ptr, Qt::AnchorTop);
    m_bottom = new AnchorsBase::AnchorsInfo(q_ptr, Qt::AnchorBottom);
    m_left = new AnchorsBase::AnchorsInfo(q_ptr, Qt::AnchorLeft);
    m_right = new AnchorsBase::AnchorsInfo(q_ptr, Qt::AnchorRight);
    m_horizontalCenter = new AnchorsBase::AnchorsInfo(q_ptr, Qt::AnchorHorizontalCenter);
    m_verticalCenter = new AnchorsBase::AnchorsInfo(q_ptr, Qt::AnchorVerticalCenter);
}

AnchorsBasePrivate::~AnchorsBasePrivate()
{
    delete m_top;
    delete m_bottom;
    delete m_left;
    delete m_right;
    delete m_horizontalCenter;
    delete m_verticalCenter;
}

void AnchorsBasePrivate::top(const AnchorsBase::AnchorsInfo *info)
{
    if(info==NULL || info->me == NULL)
        return;

    m_top->target = info->me;
    m_top->targetType = info->type;
}

void AnchorsBasePrivate::bottom(const AnchorsBase::AnchorsInfo *info)
{
    if(info==NULL || info->me == NULL)
        return;

    m_top->target = info->me;
    m_top->targetType = info->type;
}

void AnchorsBasePrivate::left(const AnchorsBase::AnchorsInfo *info)
{
    if(info==NULL || info->me == NULL)
        return;

    m_top->target = info->me;
    m_top->targetType = info->type;
}

void AnchorsBasePrivate::right(const AnchorsBase::AnchorsInfo *info)
{
    if(info==NULL || info->me == NULL)
        return;

    m_top->target = info->me;
    m_top->targetType = info->type;
}

void AnchorsBasePrivate::horizontalCenter(const AnchorsBase::AnchorsInfo *info)
{
    if(info==NULL || info->me == NULL)
        return;

    m_top->target = info->me;
    m_top->targetType = info->type;
}

void AnchorsBasePrivate::verticalCenter(const AnchorsBase::AnchorsInfo *info)
{
    if(info==NULL || info->me == NULL)
        return;

    m_top->target = info->me;
    m_top->targetType = info->type;
}

void AnchorsBasePrivate::fill(const QWidget *w)
{
    if(w == NULL)
        return;

    m_fill = w;
}

void AnchorsBasePrivate::centerIn(const QWidget *w)
{
    if(w == NULL)
        return;

    m_centerIn = w;
}

void AnchorsBasePrivate::margins(int m)
{
    m_margins = m;
}

void AnchorsBasePrivate::topMargin(int m)
{
    m_topMargin = m;
}

void AnchorsBasePrivate::bottomMargin(int m)
{
    m_bottomMargin = m;
}

void AnchorsBasePrivate::leftMargin(int m)
{
    m_leftMargin = m;
}

void AnchorsBasePrivate::rightMargin(int m)
{
    m_rightMargin = m;
}

void AnchorsBasePrivate::horizontalCenterOffset(int o)
{
    m_horizontalCenterOffset = o;
}

void AnchorsBasePrivate::verticalCenterOffset(int o)
{
    m_verticalCenterOffset = o;
}

void AnchorsBasePrivate::alignWhenCentered(bool c)
{
    m_alignWhenCentered = c;
}

AnchorsBase::AnchorsBase():
    d_ptr(new AnchorsBasePrivate(Anchors<QWidget>::cast(this)))
{
}

AnchorsBase::~AnchorsBase()
{
    Q_D(AnchorsBase);

    d->deleteLater();
}

const AnchorsBase::AnchorsInfo *AnchorsBase::top()
{
    Q_D(AnchorsBase);

    return d->m_top;
}

const AnchorsBase::AnchorsInfo *AnchorsBase::bottom()
{
    Q_D(AnchorsBase);

    return d->m_bottom;
}

const AnchorsBase::AnchorsInfo *AnchorsBase::left()
{
    Q_D(AnchorsBase);

    return d->m_left;
}

const AnchorsBase::AnchorsInfo *AnchorsBase::right()
{
    Q_D(AnchorsBase);

    return d->m_right;
}

const AnchorsBase::AnchorsInfo *AnchorsBase::horizontalCenter()
{
    Q_D(AnchorsBase);

    return d->m_horizontalCenter;
}

const AnchorsBase::AnchorsInfo *AnchorsBase::verticalCenter()
{
    Q_D(AnchorsBase);

    return d->m_verticalCenter;
}

const QWidget *AnchorsBase::fill()
{
    Q_D(AnchorsBase);

    return d->m_fill;
}

const QWidget *AnchorsBase::centerIn()
{
    Q_D(AnchorsBase);

    return d->m_centerIn;
}

int AnchorsBase::margins()
{
    Q_D(AnchorsBase);

    return d->m_margins;
}

int AnchorsBase::topMargin()
{
    Q_D(AnchorsBase);

    return d->m_topMargin;
}

int AnchorsBase::bottomMargin()
{
    Q_D(AnchorsBase);

    return d->m_bottomMargin;
}

int AnchorsBase::leftMargin()
{
    Q_D(AnchorsBase);

    return d->m_leftMargin;
}

int AnchorsBase::rightMargin()
{
    Q_D(AnchorsBase);

    return d->m_rightMargin;
}

int AnchorsBase::horizontalCenterOffset()
{
    Q_D(AnchorsBase);

    return d->m_horizontalCenterOffset;
}

int AnchorsBase::verticalCenterOffset()
{
    Q_D(AnchorsBase);

    return d->m_verticalCenterOffset;
}

int AnchorsBase::alignWhenCentered()
{
    Q_D(AnchorsBase);

    return d->m_alignWhenCentered;
}

AnchorsBasePrivate *AnchorsBase::anchors() const
{
    return d_ptr;
}
