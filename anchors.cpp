#include <QDebug>

#include "anchors.h"

class ExtendWidgetPrivate
{
    explicit ExtendWidgetPrivate(ExtendWidget *qq): q_ptr(qq){}

    QSize old_size;
    QPoint old_pos;
    QWidget *target = NULL;

    ExtendWidget *q_ptr;

    Q_DECLARE_PUBLIC(ExtendWidget)
};

ExtendWidget::ExtendWidget(QWidget *w, QObject *parent):
    QObject(parent),
    d_ptr(new ExtendWidgetPrivate(this))
{
    if(w){
        Q_D(ExtendWidget);

        d->target = w;
        w->installEventFilter(this);
    }
}

ExtendWidget::~ExtendWidget()
{
    delete d_ptr;
}

QWidget *ExtendWidget::target() const
{
    Q_D(const ExtendWidget);

    return d->target;
}

void ExtendWidget::setTarget(QWidget *target)
{
    Q_D(ExtendWidget);

    if (d->target == target)
        return;

    if(d->target)
        d->target->removeEventFilter(this);
    target->installEventFilter(this);
    d->target = target;
    emit targetChanged(target);
}

bool ExtendWidget::eventFilter(QObject *o, QEvent *e)
{
    Q_D(ExtendWidget);

    if( o == d->target ){
        if(e->type() == QEvent::Resize){
            QResizeEvent *event = static_cast<QResizeEvent*>(e);
            if(event){
                QSize size = event->size();

                if(size.width() != d->old_size.width())
                    emit widthChanged(size.width());

                if(size.height() != d->old_size.height())
                    emit heightChanged(size.height());

                if(size != d->old_size)
                    emit sizeChanged(size);

                d->old_size = size;
            }
        }else if(e->type() == QEvent::Move){
            QMoveEvent *event = static_cast<QMoveEvent*>(e);

            if(event){
                QPoint pos = event->pos();

                if(pos.x() != d->old_pos.x())
                    emit xChanged(pos.x());

                if(pos.y() != d->old_pos.y())
                    emit yChanged(pos.y());

                if(pos != d->old_pos)
                    emit positionChanged(pos);

                d->old_pos = pos;
            }
        }
    }

    return false;
}

ExtendWidget::ExtendWidget(ExtendWidgetPrivate *dd, QWidget *w, QObject *parent):
    QObject(parent),
    d_ptr(dd)
{
    w->installEventFilter(this);
}

class AnchorsBasePrivate
{
    AnchorsBasePrivate(AnchorsBase *qq): q_ptr(qq){}
    ~AnchorsBasePrivate(){
        delete top;
        delete bottom;
        delete left;
        delete right;
        delete horizontalCenter;
        delete verticalCenter;
    }

    static void setMap(const QWidget* w, AnchorsBase* b){
        if(w){
            AnchorsBase *bb = widgetMap.value(w, NULL);
            if(bb)
                bb->deleteLater();
            widgetMap[w] = b;
        }
    }
    static AnchorsBase *getMap(QWidget* w){
        if(!w)
            return NULL;

        AnchorsBase *bb = widgetMap.value(w, NULL);
        if(!bb){
            bb = new AnchorsBase(w);
            widgetMap[w] = bb;
        }

        return bb;
    }
    static void removeMap(const QWidget* w, const AnchorsBase *b){
        if(w && b && widgetMap.value(w, NULL) == b){
            widgetMap.remove(w);
        }
    }

    const AnchorsInfo* getInfoByPoint(const Qt::AnchorPoint &p) const{
        switch (p) {
        case Qt::AnchorTop:
            return top;
        case Qt::AnchorBottom:
            return bottom;
        case Qt::AnchorLeft:
            return left;
        case Qt::AnchorRight:
            return right;
        case Qt::AnchorHorizontalCenter:
            return horizontalCenter;
        case Qt::AnchorVerticalCenter:
            return verticalCenter;
        default:
            return NULL;
        }
    }

    bool checkInfo(const AnchorsInfo* info1, const AnchorsInfo *info2){
        const Qt::AnchorPoint &p = info2->type;

        switch (info1->type) {
        case Qt::AnchorTop://Deliberate
        case Qt::AnchorBottom://Deliberate
        case Qt::AnchorHorizontalCenter:
            return (p == Qt::AnchorTop || p == Qt::AnchorBottom || p == Qt::AnchorVerticalCenter);
        case Qt::AnchorLeft://Deliberate
        case Qt::AnchorRight://Deliberate
        case Qt::AnchorVerticalCenter:
            return (p == Qt::AnchorLeft || p == Qt::AnchorRight || p == Qt::AnchorHorizontalCenter);
        default:
            return false;
        }
    }

    int getValueByInfo(const AnchorsInfo *info){
//        switch (info1->type) {
//        case Qt::AnchorTop:
//            return info->base->target()->geometry().top();
//        case Qt::AnchorBottom:
//            return info->base->target()->geometry().bottom();
//        case Qt::AnchorHorizontalCenter:
//            return (p == Qt::AnchorTop || p == Qt::AnchorBottom || p == Qt::AnchorVerticalCenter);
//        case Qt::AnchorLeft://Deliberate
//        case Qt::AnchorRight://Deliberate
//        case Qt::AnchorVerticalCenter:
//            return (p == Qt::AnchorLeft || p == Qt::AnchorRight || p == Qt::AnchorHorizontalCenter);
//        default:
//            return false;
//        }
    }

    ExtendWidget *extendWidget = NULL;
    AnchorsInfo* top = new AnchorsInfo(q_ptr, Qt::AnchorTop);
    AnchorsInfo* bottom = new AnchorsInfo(q_ptr, Qt::AnchorBottom);
    AnchorsInfo* left = new AnchorsInfo(q_ptr, Qt::AnchorLeft);
    AnchorsInfo* right = new AnchorsInfo(q_ptr, Qt::AnchorRight);
    AnchorsInfo* horizontalCenter = new AnchorsInfo(q_ptr, Qt::AnchorHorizontalCenter);
    AnchorsInfo* verticalCenter = new AnchorsInfo(q_ptr, Qt::AnchorVerticalCenter);
    ExtendWidget* fill = new ExtendWidget(NULL, q_ptr);
    ExtendWidget* centerIn = new ExtendWidget(NULL, q_ptr);
    int margins = 0;
    int topMargin = 0;
    int bottomMargin = 0;
    int leftMargin = 0;
    int rightMargin = 0;
    int horizontalCenterOffset = 0;
    int verticalCenterOffset = 0;
    bool alignWhenCentered = false;
    QSize widget_old_size;
    QPoint widget_old_pos;
    AnchorsBase::AnchorError errorCode = AnchorsBase::NoError;
    QString errorString;
    static QMap<const QWidget*, AnchorsBase*> widgetMap;

    AnchorsBase *q_ptr;

    Q_DECLARE_PUBLIC(AnchorsBase)
};

QMap<const QWidget*, AnchorsBase*> AnchorsBasePrivate::widgetMap;

AnchorsBase::AnchorsBase(QWidget *w):
    QObject(w),
    d_ptr(new AnchorsBasePrivate(this))
{
    Q_D(AnchorsBase);

    d->extendWidget = new ExtendWidget(w, this);
    connect(d->extendWidget, SIGNAL(targetChanged(QWidget*)), SIGNAL(targetChanged(QWidget*)));

    d->setMap(w, this);
}

AnchorsBase::~AnchorsBase()
{
    Q_D(AnchorsBase);

    d->removeMap(target(), this);

    delete d;
}

QWidget *AnchorsBase::target() const
{
    Q_D(const AnchorsBase);

    return d->extendWidget->target();
}

const AnchorsBase *AnchorsBase::anchors() const
{
    return this;
}

const AnchorsInfo *AnchorsBase::top() const
{
    Q_D(const AnchorsBase);

    return d->top;
}

const AnchorsInfo *AnchorsBase::bottom() const
{
    Q_D(const AnchorsBase);

    return d->bottom;
}

const AnchorsInfo *AnchorsBase::left() const
{
    Q_D(const AnchorsBase);

    return d->left;
}

const AnchorsInfo *AnchorsBase::right() const
{
    Q_D(const AnchorsBase);

    return d->right;
}

const AnchorsInfo *AnchorsBase::horizontalCenter() const
{
    Q_D(const AnchorsBase);

    return d->horizontalCenter;
}

const AnchorsInfo *AnchorsBase::verticalCenter() const
{
    Q_D(const AnchorsBase);

    return d->verticalCenter;
}

QWidget *AnchorsBase::fill() const
{
    Q_D(const AnchorsBase);

    if(d->fill)
        return d->fill->target();

    return NULL;
}

QWidget *AnchorsBase::centerIn() const
{
    Q_D(const AnchorsBase);

    if(d->centerIn)
        return d->centerIn->target();

    return NULL;
}

int AnchorsBase::margins() const
{
    Q_D(const AnchorsBase);

    return d->margins;
}

int AnchorsBase::topMargin() const
{
    Q_D(const AnchorsBase);

    return d->topMargin;
}

int AnchorsBase::bottomMargin() const
{
    Q_D(const AnchorsBase);

    return d->bottomMargin;
}

int AnchorsBase::leftMargin() const
{
    Q_D(const AnchorsBase);

    return d->leftMargin;
}

int AnchorsBase::rightMargin() const
{
    Q_D(const AnchorsBase);

    return d->rightMargin;
}

int AnchorsBase::horizontalCenterOffset() const
{
    Q_D(const AnchorsBase);

    return d->horizontalCenterOffset;
}

int AnchorsBase::verticalCenterOffset() const
{
    Q_D(const AnchorsBase);

    return d->verticalCenterOffset;
}

int AnchorsBase::alignWhenCentered() const
{
    Q_D(const AnchorsBase);

    return d->alignWhenCentered;
}

AnchorsBase::AnchorError AnchorsBase::errorCode() const
{
    Q_D(const AnchorsBase);

    return d->errorCode;
}

QString AnchorsBase::errorString() const
{
    Q_D(const AnchorsBase);

    return d->errorString;
}

bool AnchorsBase::setAnchor(QWidget *w, const Qt::AnchorPoint &p, QWidget *target, const Qt::AnchorPoint &point)
{
    AnchorsBase *base = AnchorsBasePrivate::getMap(w);

    return base->setAnchor(p, target, point);
}

void AnchorsBase::setTarget(QWidget *target)
{
    Q_D(AnchorsBase);

    d->extendWidget->setTarget(target);
}

bool AnchorsBase::setAnchor(const Qt::AnchorPoint &p, QWidget *target, const Qt::AnchorPoint &point)
{
    AnchorsBase *base = AnchorsBasePrivate::getMap(target);
    const AnchorsInfo* info = base->d_func()->getInfoByPoint(point);

    switch (p) {
    case Qt::AnchorTop:
        return setTop(info);
    case Qt::AnchorBottom:
        return setBottom(info);
    case Qt::AnchorLeft:
        return setLeft(info);
    case Qt::AnchorRight:
        return setRight(info);
    case Qt::AnchorHorizontalCenter:
        return setHorizontalCenter(info);
    case Qt::AnchorVerticalCenter:
        return setVerticalCenter(info);
    default:
        return false;
    }
}

bool AnchorsBase::setTop(const AnchorsInfo* top)
{
    Q_D(AnchorsBase);

    if(*d->top == top)
        return true;

    ExtendWidget *tmp_w1 = NULL;
    ExtendWidget *tmp_w2 = NULL;

    if(d->top->targetInfo){
        tmp_w1 = d->top->targetInfo->base->d_func()->extendWidget;
    }

    if(top){
        if (top->base == this){
            d->errorCode = TargetInvalid;
            d->errorString = tr("Cannot anchor widget to self.");

            return false;
        }else if(target()->parentWidget() != top->base->target()){
            bool isBrother = false;

            foreach (const QWidget *w, target()->parentWidget()->findChildren<QWidget*>()) {
                if(w == top->base->target()){
                    isBrother = true;
                    break;
                }
            }

            if(!isBrother){
                d->errorCode = TargetInvalid;
                d->errorString = tr("Cannot anchor to an widget that isn't a parent or sibling.");
            }
        }

        if(!d->checkInfo(d->top, top)){
            d->errorCode = PointInvalid;
            d->errorString = tr("Cannot anchor a vertical/horizontal edge to a horizontal/vertical edge.");
        }

        tmp_w2 = top->base->d_func()->extendWidget;
        if(tmp_w1 != tmp_w2){
            if(tmp_w1)
                disconnect(tmp_w1, SIGNAL(yChanged(int)), this, SLOT(updateTop()));
            connect(tmp_w2, SIGNAL(yChanged(int)), this, SLOT(updateTop()));
        }

        *d->top = top;
        updateTop();
    }else{
        disconnect(tmp_w1, SIGNAL(yChanged(int)), this, SLOT(updateTop()));
        *d->top = top;
        return true;
    }

    emit topChanged(d->top);

    return true;
}

bool AnchorsBase::setBottom(const AnchorsInfo* bottom)
{
    Q_D(AnchorsBase);

    if(*d->bottom == bottom)
        return true;

    ExtendWidget *tmp_w1 = NULL;
    ExtendWidget *tmp_w2 = NULL;

    if(d->bottom->targetInfo){
        tmp_w1 = d->bottom->targetInfo->base->d_func()->extendWidget;
    }

    if(bottom){
        if (bottom->base == this){
            d->errorCode = TargetInvalid;
            d->errorString = tr("Cannot anchor widget to self.");

            return false;
        }

        tmp_w2 = bottom->base->d_func()->extendWidget;
        if(tmp_w1 != tmp_w2){
            if(tmp_w1){
                disconnect(tmp_w1, SIGNAL(yChanged(int)), this, SLOT(updateBottom()));
                disconnect(tmp_w1, SIGNAL(heightChanged(int)), this, SLOT(updateBottom()));
            }
            connect(tmp_w2, SIGNAL(yChanged(int)), SLOT(updateBottom()));
            connect(tmp_w2, SIGNAL(heightChanged(int)), SLOT(updateBottom()));
        }
    }else{
        disconnect(tmp_w1, SIGNAL(yChanged(int)), this, SLOT(updateBottom()));
        disconnect(tmp_w1, SIGNAL(heightChanged(int)), this, SLOT(updateBottom()));
        *d->bottom = bottom;
        return true;
    }

    *d->bottom = bottom;
    updateTop();

    emit bottomChanged(d->bottom);

    return true;
}

bool AnchorsBase::setLeft(const AnchorsInfo* left)
{
    Q_D(AnchorsBase);

    if (left->base == this){
        d->errorCode = TargetInvalid;
        d->errorString = tr("Cannot anchor widget to self.");

        return false;
    }
    if(*d->left == left)
        return true;

    ExtendWidget *tmp_w;
    if(d->left->targetInfo){
        tmp_w = d->left->targetInfo->base->d_func()->extendWidget;
        disconnect(tmp_w, SIGNAL(xChanged(int)), this, SLOT(updateLeft()));
    }

    *d->left = left;
    if(left){
        updateLeft();

        tmp_w = left->base->d_func()->extendWidget;
        connect(tmp_w, SIGNAL(xChanged(int)), SLOT(updateLeft()));
    }

    emit leftChanged(d->left);

    return true;
}

bool AnchorsBase::setRight(const AnchorsInfo* right)
{
    Q_D(AnchorsBase);

    if (right->base == this){
        d->errorCode = TargetInvalid;
        d->errorString = tr("Cannot anchor widget to self.");

        return false;
    }
    if(*d->right == right)
        return true;

    ExtendWidget *tmp_w;
    if(d->right->targetInfo){
        tmp_w = d->right->targetInfo->base->d_func()->extendWidget;
        disconnect(tmp_w, SIGNAL(xChanged(int)), this, SLOT(updateRight()));
        disconnect(tmp_w, SIGNAL(widthChanged(int)), this, SLOT(updateRight()));
    }

    *d->right = right;
    if(right){
        updateRight();

        tmp_w = right->base->d_func()->extendWidget;
        connect(tmp_w, SIGNAL(xChanged(int)), SLOT(updateRight()));
        connect(tmp_w, SIGNAL(widthChanged(int)), SLOT(updateRight()));
    }

    emit rightChanged(d->right);

    return true;
}

bool AnchorsBase::setHorizontalCenter(const AnchorsInfo* horizontalCenter)
{
    Q_D(AnchorsBase);

    if (horizontalCenter->base == this){
        d->errorCode = TargetInvalid;
        d->errorString = tr("Cannot anchor widget to self.");

        return false;
    }
    if(*d->horizontalCenter == horizontalCenter)
        return true;

    ExtendWidget *tmp_w;
    if(d->horizontalCenter->targetInfo){
        tmp_w = d->horizontalCenter->targetInfo->base->d_func()->extendWidget;
        disconnect(tmp_w, SIGNAL(xChanged(int)), this, SLOT(updateHorizontalCenter()));
        disconnect(tmp_w, SIGNAL(widthChanged(int)), this, SLOT(updateHorizontalCenter()));
    }

    *d->horizontalCenter = horizontalCenter;
    if(horizontalCenter){
        updateHorizontalCenter();

        tmp_w = horizontalCenter->base->d_func()->extendWidget;
        connect(tmp_w, SIGNAL(xChanged(int)), SLOT(updateHorizontalCenter()));
        connect(tmp_w, SIGNAL(widthChanged(int)), SLOT(updateHorizontalCenter()));
    }

    emit horizontalCenterChanged(d->horizontalCenter);

    return true;
}

bool AnchorsBase::setVerticalCenter(const AnchorsInfo* verticalCenter)
{
    Q_D(AnchorsBase);

    if (verticalCenter->base == this){
        d->errorCode = TargetInvalid;
        d->errorString = tr("Cannot anchor widget to self.");

        return false;
    }
    if(*d->verticalCenter == verticalCenter)
        return true;

    ExtendWidget *tmp_w;
    if(d->verticalCenter->targetInfo){
        tmp_w = d->verticalCenter->targetInfo->base->d_func()->extendWidget;
        disconnect(tmp_w, SIGNAL(yChanged(int)), this, SLOT(updateVerticalCenter()));
        disconnect(tmp_w, SIGNAL(heightChanged(int)), this, SLOT(updateVerticalCenter()));
    }

    *d->verticalCenter = verticalCenter;
    if(verticalCenter){
        updateVerticalCenter();

        tmp_w = verticalCenter->base->d_func()->extendWidget;
        connect(tmp_w, SIGNAL(yChanged(int)), SLOT(updateVerticalCenter()));
        connect(tmp_w, SIGNAL(heightChanged(int)), SLOT(updateVerticalCenter()));
    }

    emit verticalCenterChanged(d->verticalCenter);

    return true;
}

bool AnchorsBase::setFill(QWidget* fill)
{
    Q_D(AnchorsBase);

    if (fill == target()){
        d->errorCode = TargetInvalid;
        d->errorString = tr("Cannot anchor widget to self.");

        return false;
    }

    if(d->fill){
        if(d->fill->target() == fill)
            return true;
        else
            d->fill->deleteLater();
    }

    ExtendWidget *tmp_w;
    if(d->fill){
        tmp_w = d->fill;
        disconnect(tmp_w, SIGNAL(positionChanged(int)), this, SLOT(updateFill()));
        disconnect(tmp_w, SIGNAL(sizeChanged(int)), this, SLOT(updateFill()));
    }

    d->fill->setTarget(fill);
    if(fill){
        updateFill();

        tmp_w = d->fill;
        connect(tmp_w, SIGNAL(positionChanged(int)), SLOT(updateFill()));
        connect(tmp_w, SIGNAL(sizeChanged(int)), SLOT(updateFill()));
    }

    emit fillChanged(fill);

    return true;
}

bool AnchorsBase::setCenterIn(QWidget* centerIn)
{
    Q_D(AnchorsBase);

    if (centerIn == target()){
        d->errorCode = TargetInvalid;
        d->errorString = tr("Cannot anchor widget to self.");

        return false;
    }

    if(d->centerIn){
        if(d->centerIn->target() == centerIn)
            return true;
        else
            d->centerIn->deleteLater();
    }

    ExtendWidget *tmp_w;
    if(d->centerIn){
        tmp_w = d->centerIn;
        disconnect(tmp_w, SIGNAL(positionChanged(int)), this, SLOT(updateCenterIn()));
    }

    d->centerIn->setTarget(centerIn);
    if(centerIn){
        updateCenterIn();

        tmp_w = d->centerIn;
        connect(tmp_w, SIGNAL(positionChanged(int)), SLOT(updateCenterIn()));
    }

    emit centerInChanged(centerIn);

    return true;
}

void AnchorsBase::setMargins(int margins)
{
    Q_D(AnchorsBase);

    if (d->margins == margins)
        return;

    d->margins = margins;

    if(margins !=0 ){
        if(d->topMargin == 0)
            updateTop();
        if(d->bottomMargin == 0)
            updateBottom();
        if(d->leftMargin == 0)
            updateLeft();
        if(d->rightMargin == 0)
            updateRight();
        updateFill();
    }

    emit marginsChanged(margins);
}

void AnchorsBase::setTopMargin(int topMargin)
{
    Q_D(AnchorsBase);

    if (d->topMargin == topMargin)
        return;

    d->topMargin = topMargin;

    updateTop();
    updateFill();

    emit topMarginChanged(topMargin);
}

void AnchorsBase::setBottomMargin(int bottomMargin)
{
    Q_D(AnchorsBase);

    if (d->bottomMargin == bottomMargin)
        return;

    d->bottomMargin = bottomMargin;

    updateBottom();
    updateFill();

    emit bottomMarginChanged(bottomMargin);
}

void AnchorsBase::setLeftMargin(int leftMargin)
{
    Q_D(AnchorsBase);

    if (d->leftMargin == leftMargin)
        return;

    d->leftMargin = leftMargin;

    updateLeft();
    updateFill();

    emit leftMarginChanged(leftMargin);
}

void AnchorsBase::setRightMargin(int rightMargin)
{
    Q_D(AnchorsBase);

    if (d->rightMargin == rightMargin)
        return;

    d->rightMargin = rightMargin;

    updateRight();
    updateFill();

    emit rightMarginChanged(rightMargin);
}

void AnchorsBase::setHorizontalCenterOffset(int horizontalCenterOffset)
{
    Q_D(AnchorsBase);

    if (d->horizontalCenterOffset == horizontalCenterOffset)
        return;

    d->horizontalCenterOffset = horizontalCenterOffset;

    updateHorizontalCenter();

    emit horizontalCenterOffsetChanged(horizontalCenterOffset);
}

void AnchorsBase::setVerticalCenterOffset(int verticalCenterOffset)
{
    Q_D(AnchorsBase);

    if (d->verticalCenterOffset == verticalCenterOffset)
        return;

    d->verticalCenterOffset = verticalCenterOffset;

    updateVerticalCenter();

    emit verticalCenterOffsetChanged(verticalCenterOffset);
}

void AnchorsBase::setAlignWhenCentered(bool alignWhenCentered)
{
    Q_D(AnchorsBase);

    if (d->alignWhenCentered == alignWhenCentered)
        return;

    d->alignWhenCentered = alignWhenCentered;
    emit alignWhenCenteredChanged(alignWhenCentered);
}

void AnchorsBase::setTop(int arg)
{
    QRect rect = target()->geometry();
    rect.setTop(arg);

    target()->setGeometry(rect);
}

void AnchorsBase::setBottom(int arg)
{
    QRect rect = target()->geometry();
    rect.setBottom(arg);

    target()->setGeometry(rect);
}

void AnchorsBase::setLeft(int arg)
{
    QRect rect = target()->geometry();
    rect.setLeft(arg);

    target()->setGeometry(rect);
}

void AnchorsBase::setRight(int arg)
{
    QRect rect = target()->geometry();
    rect.setRight(arg);

    target()->setGeometry(rect);
}

void AnchorsBase::moveTop(int arg)
{
    QRect rect = target()->geometry();
    rect.moveTop(arg);

    target()->setGeometry(rect);
}

void AnchorsBase::moveBottom(int arg)
{
    QRect rect = target()->geometry();
    rect.moveBottom(arg);

    target()->setGeometry(rect);
}

void AnchorsBase::moveLeft(int arg)
{
    QRect rect = target()->geometry();
    rect.moveLeft(arg);

    target()->setGeometry(rect);
}

void AnchorsBase::moveRight(int arg)
{
    QRect rect = target()->geometry();
    rect.moveRight(arg);

    target()->setGeometry(rect);
}

void AnchorsBase::moveHorizontalCenter(int arg)
{
    QRect rect = target()->geometry();
    rect.moveLeft(arg - rect.width()/2);

    target()->setGeometry(rect);
}

void AnchorsBase::moveVerticalCenter(int arg)
{
    QRect rect = target()->geometry();
    rect.moveTop(arg - rect.height()/2);

    target()->setGeometry(rect);
}

void AnchorsBase::moveCenter(const QPoint &arg)
{
    QRect rect = target()->geometry();
    rect.moveCenter(arg);

    target()->setGeometry(rect);
}

void AnchorsBase::updateTop()
{
    Q_D(AnchorsBase);

    QWidget *w = d->top->targetInfo->base->target();
    int offset = d->topMargin == 0 ? d->margins : d->topMargin;

    if(target()->parentWidget() != w){
        offset += w->y();
    }

    switch (d->top->targetInfo->type) {
    case Qt::AnchorVerticalCenter:
        offset += w->height() / 2;
        break;
    case Qt::AnchorBottom:
        offset += w->height();
        break;
    default:
        break;
    }

    moveTop(offset);
}

void AnchorsBase::updateBottom()
{

}

void AnchorsBase::updateLeft()
{

}

void AnchorsBase::updateRight()
{

}

void AnchorsBase::updateHorizontalCenter()
{

}

void AnchorsBase::updateVerticalCenter()
{

}

void AnchorsBase::updateFill()
{

}

void AnchorsBase::updateCenterIn()
{

}

AnchorsBase::AnchorsBase(AnchorsBasePrivate *dd, QWidget *w):
    d_ptr(dd)
{
    Q_D(AnchorsBase);

    d->extendWidget = new ExtendWidget(w, this);
    connect(d->extendWidget, SIGNAL(targetChanged(QWidget*)), SIGNAL(targetChanged(QWidget*)));

    d->setMap(w, this);
}
