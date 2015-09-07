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
    if(w)
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

    static void setWidgetAnchorsBase(const QWidget* w, AnchorsBase* b){
        if(w){
            AnchorsBase *bb = widgetMap.value(w, NULL);
            if(bb)
                bb->deleteLater();
            widgetMap[w] = b;
        }
    }
    static AnchorsBase *getWidgetAnchorsBase(QWidget* w){
        if(!w)
            return NULL;

        AnchorsBase *bb = widgetMap.value(w, NULL);
        if(!bb){
            bb = new AnchorsBase(w);
            widgetMap[w] = bb;
        }

        return bb;
    }
    static void removeWidgetAnchorsBase(const QWidget* w, const AnchorsBase *b){
        if(w && b && widgetMap.value(w, NULL) == b){
            widgetMap.remove(w);
        }
    }

    const AnchorInfo* getInfoByPoint(const Qt::AnchorPoint &p) const{
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

    bool checkInfo(const AnchorInfo* info1, const AnchorInfo *info2) const{
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

    bool isBindable(const AnchorInfo *info) const{
        Q_Q(const AnchorsBase);

        if(fill->target() || centerIn->target())
            return false;

        bool tmp1 = ((int)q->isBinding(top)+(int)q->isBinding(verticalCenter)+(int)q->isBinding(bottom)) < 2;
        bool tmp2 = ((int)q->isBinding(left)+(int)q->isBinding(horizontalCenter)+(int)q->isBinding(right)) < 2;

        switch (info->type) {
        case Qt::AnchorTop://Deliberate
        case Qt::AnchorBottom://Deliberate
        case Qt::AnchorHorizontalCenter:
            return tmp1;
        case Qt::AnchorLeft://Deliberate
        case Qt::AnchorRight://Deliberate
        case Qt::AnchorVerticalCenter:
            return tmp2;
        default:
            return false;
        }
    }

    int getValueByInfo(const AnchorInfo *info){
        ARect rect = info->base->target()->geometry();

        switch (info->type) {
        case Qt::AnchorTop:
            return rect.top();
        case Qt::AnchorBottom:
            return rect.bottom();
        case Qt::AnchorHorizontalCenter:
            return rect.horizontalCenter();
        case Qt::AnchorLeft:
            return rect.left();
        case Qt::AnchorRight:
            return rect.right();
        case Qt::AnchorVerticalCenter:
            return rect.right();
        default:
            return 0;
        }
    }

    const ARect getWidgetRect(const QWidget *w) const{
        if(!w)
            return ARect();

        if(extendWidget && extendWidget->target()->parentWidget() == w)
            return w->rect();

        return w->geometry();
    }

    AnchorsBase *q_ptr;

    ExtendWidget *extendWidget = NULL;
    AnchorInfo* top = new AnchorInfo(q_ptr, Qt::AnchorTop);
    AnchorInfo* bottom = new AnchorInfo(q_ptr, Qt::AnchorBottom);
    AnchorInfo* left = new AnchorInfo(q_ptr, Qt::AnchorLeft);
    AnchorInfo* right = new AnchorInfo(q_ptr, Qt::AnchorRight);
    AnchorInfo* horizontalCenter = new AnchorInfo(q_ptr, Qt::AnchorHorizontalCenter);
    AnchorInfo* verticalCenter = new AnchorInfo(q_ptr, Qt::AnchorVerticalCenter);
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

    Q_DECLARE_PUBLIC(AnchorsBase)
};

QMap<const QWidget*, AnchorsBase*> AnchorsBasePrivate::widgetMap;

AnchorsBase::AnchorsBase(QWidget *w):
    QObject(w),
    d_ptr(new AnchorsBasePrivate(this))
{
    init(w);
}

AnchorsBase::~AnchorsBase()
{
    Q_D(AnchorsBase);

    d->removeWidgetAnchorsBase(target(), this);

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

const AnchorInfo *AnchorsBase::top() const
{
    Q_D(const AnchorsBase);

    return d->top;
}

const AnchorInfo *AnchorsBase::bottom() const
{
    Q_D(const AnchorsBase);

    return d->bottom;
}

const AnchorInfo *AnchorsBase::left() const
{
    Q_D(const AnchorsBase);

    return d->left;
}

const AnchorInfo *AnchorsBase::right() const
{
    Q_D(const AnchorsBase);

    return d->right;
}

const AnchorInfo *AnchorsBase::horizontalCenter() const
{
    Q_D(const AnchorsBase);

    return d->horizontalCenter;
}

const AnchorInfo *AnchorsBase::verticalCenter() const
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

bool AnchorsBase::isBinding(const AnchorInfo *info) const
{
    return info->targetInfo;
}

bool AnchorsBase::setAnchor(QWidget *w, const Qt::AnchorPoint &p, QWidget *target, const Qt::AnchorPoint &point)
{
    AnchorsBase *base = AnchorsBasePrivate::getWidgetAnchorsBase(w);

    return base->setAnchor(p, target, point);
}

void AnchorsBase::setTarget(QWidget *target)
{
    Q_D(AnchorsBase);

    d->extendWidget->setTarget(target);
}

bool AnchorsBase::setAnchor(const Qt::AnchorPoint &p, QWidget *target, const Qt::AnchorPoint &point)
{
    AnchorsBase *base = AnchorsBasePrivate::getWidgetAnchorsBase(target);
    const AnchorInfo* info = base->d_func()->getInfoByPoint(point);

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

#define ANCHOR_BIND_INFO(point, Point, signalsname...)\
    Q_D(AnchorsBase);\
    if(*d->point == point)\
        return true;\
    ExtendWidget *tmp_w1 = NULL;\
    ExtendWidget *tmp_w2 = NULL;\
    if(d->point->targetInfo){\
        tmp_w1 = d->point->targetInfo->base->d_func()->extendWidget;\
    }\
    QStringList signalList = QString(#signalsname).split("),");\
    if(point){\
        if(!d->isBindable(d->point)){\
            d->errorCode = Conflict;\
            d->errorString = tr("Conflict: CenterIn or Fill is anchored.");\
            return false;\
        }\
        if (point->base == this){\
            d->errorCode = TargetInvalid;\
            d->errorString = tr("Cannot anchor widget to self.");\
            return false;\
        }else if(target()->parentWidget() != point->base->target()){\
            bool isBrother = false;\
            foreach (const QWidget *w, target()->parentWidget()->findChildren<QWidget*>()) {\
                if(w == point->base->target()){\
                    isBrother = true;\
                    break;\
                }\
            }\
            if(!isBrother){\
                d->errorCode = TargetInvalid;\
                d->errorString = tr("Cannot anchor to an widget that isn't a parent or sibling.");\
            }\
        }\
        if(!d->checkInfo(d->point, point)){\
            d->errorCode = PointInvalid;\
            d->errorString = tr("Cannot anchor a vertical/horizontal edge to a horizontal/vertical edge.");\
        }\
        int old_pos = d->getValueByInfo(point);\
        AnchorInfo old_info = *d->point;\
        *d->point = point;\
        update##Point();\
        if(old_pos != d->getValueByInfo(point)){\
            *d->point = old_info;\
            update##Point();\
            d->errorCode = PointInvalid;\
            d->errorString = tr("loop bind.");\
            return false;\
        }\
        tmp_w2 = point->base->d_func()->extendWidget;\
        if(tmp_w1 != tmp_w2){\
            foreach(const QString &str, signalList){\
                QByteArray arr = str.toLatin1();\
                if(tmp_w1)\
                    disconnect(tmp_w1, QByteArray("2"+arr+")").data(), this, SLOT(update##Point()));\
                connect(tmp_w2, QByteArray("2"+arr+")").data(), this, SLOT(update##Point()));\
            }\
        }\
    }else{\
    foreach(const QString &str, signalList){\
        QByteArray arr = str.toLatin1();\
            disconnect(tmp_w1, QByteArray("2"+arr+")").data(), this, SLOT(update##Point()));\
        }\
        *d->point = point;\
        return true;\
    }\
    emit point##Changed(d->point);\
    return true;\

#define ANCHOR_BIND_WIDGET(point, Point)\
    if(d->point->target() == point)\
        return true;\
    if(point){\
        if (point == target()){\
            d->errorCode = TargetInvalid;\
            d->errorString = tr("Cannot anchor widget to self.");\
            return false;\
        }else if(target()->parentWidget() != point){\
            bool isBrother = false;\
            foreach (const QWidget *w, target()->parentWidget()->findChildren<QWidget*>()) {\
                if(w == point){\
                    isBrother = true;\
                    break;\
                }\
            }\
            if(!isBrother){\
                d->errorCode = TargetInvalid;\
                d->errorString = tr("Cannot anchor to an widget that isn't a parent or sibling.");\
            }\
        }\
        QRect old_rect = point->geometry();\
        QWidget *old_widget = d->point->target();\
        d->point->setTarget(point);\
        update##Point();\
        if(old_rect != point->geometry()){\
            d->point->setTarget(old_widget);\
            update##Point();\
            d->errorCode = PointInvalid;\
            d->errorString = tr("loop bind.");\
            return false;\
        }\
        AnchorInfo *info = NULL;\
        setTop(info);setLeft(info);setRight(info);setBottom(info);setHorizontalCenter(info);setVerticalCenter(info);\
        if(d->point == d->fill)\
            setCenterIn(NULL);\
    }\
    d->point->setTarget(point);\
    emit point##Changed(point);\
    return true;\

bool AnchorsBase::setTop(const AnchorInfo* top)
{
    ANCHOR_BIND_INFO(top,Top,yChanged(int))
}

bool AnchorsBase::setBottom(const AnchorInfo* bottom)
{
    ANCHOR_BIND_INFO(bottom,Bottom,yChanged(int),heightChanged(int))
}

bool AnchorsBase::setLeft(const AnchorInfo* left)
{
    ANCHOR_BIND_INFO(left,Left,xChanged(int))
}

bool AnchorsBase::setRight(const AnchorInfo* right)
{
    ANCHOR_BIND_INFO(right,Right,xChanged(int),widthChanged(int))
}

bool AnchorsBase::setHorizontalCenter(const AnchorInfo* horizontalCenter)
{
    ANCHOR_BIND_INFO(horizontalCenter,HorizontalCenter,xChanged(int),widthChanged(int))
}

bool AnchorsBase::setVerticalCenter(const AnchorInfo* verticalCenter)
{
    ANCHOR_BIND_INFO(verticalCenter,VerticalCenter,yChanged(int),heightChanged(int))
}

bool AnchorsBase::setFill(QWidget* fill)
{
    Q_D(AnchorsBase);

    ANCHOR_BIND_WIDGET(fill,Fill)
}

bool AnchorsBase::setCenterIn(QWidget* centerIn)
{
    Q_D(AnchorsBase);

    if(d->fill->target()){
        d->errorCode = Conflict;
        d->errorString = tr("Conflict: Fill is anchored.");
        return false;
    }

    ANCHOR_BIND_WIDGET(centerIn,CenterIn)
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
    ARect rect = target()->geometry();
    rect.moveHorizontalCenter(arg);

    target()->setGeometry(rect);
}

void AnchorsBase::moveVerticalCenter(int arg)
{
    ARect rect = target()->geometry();
    rect.moveVerticalCenter(arg);

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

    int offset = d->topMargin == 0 ? d->margins : d->topMargin;

    ARect rect = d->getWidgetRect(d->top->targetInfo->base->target());

    switch (d->top->targetInfo->type) {
    case Qt::AnchorVerticalCenter:
        offset += rect.verticalCenter();
        break;
    case Qt::AnchorBottom:
        offset += rect.bottom();
        break;
    default:
        break;
    }

    if(isBinding(d->verticalCenter)){

    }else if(isBinding(d->bottom)){
        setTop(offset);
    }else{
        moveTop(offset);
    }
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
    init(w);
}

void AnchorsBase::init(QWidget *w)
{
    Q_D(AnchorsBase);

    d->extendWidget = new ExtendWidget(w, this);
    connect(d->extendWidget, SIGNAL(targetChanged(QWidget*)), SIGNAL(targetChanged(QWidget*)));
    connect(d->fill, SIGNAL(positionChanged(QPoint)), SLOT(updateFill()));
    connect(d->fill, SIGNAL(sizeChanged(QSize)), SLOT(updateFill()));
    connect(d->centerIn, SIGNAL(positionChanged(QPoint)), SLOT(updateCenterIn()));

    d->setWidgetAnchorsBase(w, this);
}
