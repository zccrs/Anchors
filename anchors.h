#ifndef ANCHORS_H
#define ANCHORS_H

#include <QObject>
#include <QPointer>
#include <QResizeEvent>
#include <QMoveEvent>

class AnchorWidgetObject: public QObject
{
    Q_OBJECT

signals:
    void xChanged(int x) const;
    void yChanged(int y) const;
    void widthChanged(int width) const;
    void heightChanged(int height) const;
};

template<class T>
class AnchorWidget : public T
{
public:
    inline const AnchorWidgetObject *object() const
    {return m_object;}

protected:
     void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;
     void moveEvent(QMoveEvent * event) Q_DECL_OVERRIDE;

private:
     const AnchorWidgetObject *m_object = new AnchorWidgetObject;
};

class AnchorsBase : public QObject
{
    Q_OBJECT

    Q_PROPERTY(const AnchorsBase* anchors READ anchors)
    Q_PROPERTY(const AnchorsInfo* top READ top WRITE setTop NOTIFY topChanged)
    Q_PROPERTY(const AnchorsInfo* bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
    Q_PROPERTY(const AnchorsInfo* left READ left WRITE setLeft NOTIFY leftChanged)
    Q_PROPERTY(const AnchorsInfo* right READ right WRITE setRight NOTIFY rightChanged)
    Q_PROPERTY(const AnchorsInfo* horizontalCenter READ horizontalCenter WRITE setHorizontalCenter NOTIFY horizontalCenterChanged)
    Q_PROPERTY(const AnchorsInfo* verticalCenter READ verticalCenter WRITE setVerticalCenter NOTIFY verticalCenterChanged)
    Q_PROPERTY(const QWidget* fill READ fill WRITE setFill NOTIFY fillChanged)
    Q_PROPERTY(const QWidget* centerIn READ centerIn WRITE setCenterIn NOTIFY centerInChanged)
    Q_PROPERTY(int margins READ margins WRITE setMargins NOTIFY marginsChanged)
    Q_PROPERTY(int topMargin READ topMargin WRITE setTopMargin NOTIFY topMarginChanged)
    Q_PROPERTY(int bottomMargin READ bottomMargin WRITE setBottomMargin NOTIFY bottomMarginChanged)
    Q_PROPERTY(int leftMargin READ leftMargin WRITE setLeftMargin NOTIFY leftMarginChanged)
    Q_PROPERTY(int rightMargin READ rightMargin WRITE setRightMargin NOTIFY rightMarginChanged)
    Q_PROPERTY(int horizontalCenterOffset READ horizontalCenterOffset WRITE setHorizontalCenterOffset NOTIFY horizontalCenterOffsetChanged)
    Q_PROPERTY(int verticalCenterOffset READ verticalCenterOffset WRITE setVerticalCenterOffset NOTIFY verticalCenterOffsetChanged)
    Q_PROPERTY(bool alignWhenCentered READ alignWhenCentered WRITE setAlignWhenCentered NOTIFY alignWhenCenteredChanged)

public:
    struct AnchorsInfo
    {
        AnchorsInfo(QWidget *w, Qt::AnchorPoint tp):
            me(w),
            target(NULL),
            type(tp),
            targetType(tp)
        {
        }

        QWidget *me;
        QWidget *target;
        Qt::AnchorPoint type;
        Qt::AnchorPoint targetType;

        bool operator==(const AnchorsInfo *info) const{
            return info->target == target && info->type == type && info->targetType == targetType;
        }

        bool operator==(const AnchorsInfo &info) const{
            return info.target == target && info.type == type && info.targetType == targetType;
        }

        bool operator!=(const AnchorsInfo *info) const{
            return info->target != target || info->type != type || info->targetType != targetType;
        }

        bool operator!=(const AnchorsInfo &info) const{
            return info.target != target || info.type != type || info.targetType != targetType;
        }

        const AnchorsInfo& operator=(const AnchorsInfo *info){
            target = info->target;
            type = info->type;
            targetType = info->targetType;

            return *this;
        }

        const AnchorsInfo& operator=(const AnchorsInfo &info){
            target = info.target;
            type = info.type;
            targetType = info.targetType;

            return *this;
        }
    };

    struct AnchorsData{
        AnchorsData(QWidget *w):
            widget(w){}

        QWidget *widget;
        AnchorsInfo* top = new AnchorsInfo(widget, Qt::AnchorTop);
        AnchorsInfo* bottom = new AnchorsInfo(widget, Qt::AnchorBottom);
        AnchorsInfo* left = new AnchorsInfo(widget, Qt::AnchorLeft);
        AnchorsInfo* right = new AnchorsInfo(widget, Qt::AnchorRight);
        AnchorsInfo* horizontalCenter = new AnchorsInfo(widget, Qt::AnchorHorizontalCenter);
        AnchorsInfo* verticalCenter = new AnchorsInfo(widget, Qt::AnchorVerticalCenter);
        const QWidget* fill = NULL;
        const QWidget* centerIn = NULL;
        int margins = 0;
        int topMargin = 0;
        int bottomMargin = 0;
        int leftMargin = 0;
        int rightMargin = 0;
        int horizontalCenterOffset = 0;
        int verticalCenterOffset = 0;
        bool alignWhenCentered = false;
    };

    const AnchorsBase *anchors() const;
    const AnchorsInfo *top() const;
    const AnchorsInfo *bottom() const;
    const AnchorsInfo *left() const;
    const AnchorsInfo *right() const;
    const AnchorsInfo *horizontalCenter() const;
    const AnchorsInfo *verticalCenter() const;
    const QWidget *fill() const;
    const QWidget *centerIn() const;
    int margins() const;
    int topMargin() const;
    int bottomMargin() const;
    int leftMargin() const;
    int rightMargin() const;
    int horizontalCenterOffset() const;
    int verticalCenterOffset() const;
    int alignWhenCentered() const;

public slots:
    void setTop(const AnchorsInfo* top);
    void setBottom(const AnchorsInfo* bottom);
    void setLeft(const AnchorsInfo* left);
    void setRight(const AnchorsInfo* right);
    void setHorizontalCenter(const AnchorsInfo* horizontalCenter);
    void setVerticalCenter(const AnchorsInfo* verticalCenter);
    void setFill(const QWidget* fill);
    void setCenterIn(const QWidget* centerIn);
    void setMargins(int margins);
    void setTopMargin(int topMargin);
    void setBottomMargin(int bottomMargin);
    void setLeftMargin(int leftMargin);
    void setRightMargin(int rightMargin);
    void setHorizontalCenterOffset(int horizontalCenterOffset);
    void setVerticalCenterOffset(int verticalCenterOffset);
    void setAlignWhenCentered(bool alignWhenCentered);

signals:
    void topChanged(const AnchorsInfo* top);
    void bottomChanged(const AnchorsInfo* bottom);
    void leftChanged(const AnchorsInfo* left);
    void rightChanged(const AnchorsInfo* right);
    void horizontalCenterChanged(const AnchorsInfo* horizontalCenter);
    void verticalCenterChanged(const AnchorsInfo* verticalCenter);
    void fillChanged(const QWidget* fill);
    void centerInChanged(const QWidget* centerIn);
    void marginsChanged(int margins);
    void topMarginChanged(int topMargin);
    void bottomMarginChanged(int bottomMargin);
    void leftMarginChanged(int leftMargin);
    void rightMarginChanged(int rightMargin);
    void horizontalCenterOffsetChanged(int horizontalCenterOffset);
    void verticalCenterOffsetChanged(int verticalCenterOffset);
    void alignWhenCenteredChanged(bool alignWhenCentered);

protected:
    inline AnchorsBase(QWidget *w): data(new AnchorsData(w)){}

    AnchorsData *data;
};

template<class T>
class Anchors : public AnchorsBase
{
public:
    inline Anchors(): m_widget(new AnchorWidget<T>), AnchorsBase(m_widget){}
    inline Anchors(AnchorWidget<T> *w): AnchorsBase(w), m_widget(w){}
    inline Anchors(const Anchors &me): AnchorsBase(me.m_widget), m_widget(me.m_widget){}

    inline AnchorWidget<T> &operator=(const Anchors &me)
    { m_widget = me.m_widget, data->widget = m_widget; return *m_widget; }
    inline AnchorWidget<T> &operator=(AnchorWidget<T>* w)
    { m_widget = w; data->widget = w; return *m_widget; }
    inline AnchorWidget<T>* widget() const
    {return m_widget;}
    inline AnchorWidget<T>* operator->() const
    { return widget(); }
    inline AnchorWidget<T>& operator*() const
    { return *widget(); }
    inline operator AnchorWidget<T>*() const
    { return widget(); }

private:
    AnchorWidget<T> *m_widget;
};


template<class T>
void AnchorWidget<T>::resizeEvent(QResizeEvent *event)
{
    QSize size = event->size();

    if(size.width() != T::width())
        emit m_object->widthChanged(size.width());

    if(size.height() != T::height())
        emit m_object->heightChanged(size.height());

    T::resizeEvent(event);
}

template<class T>
void AnchorWidget<T>::moveEvent(QMoveEvent *event)
{
    QPoint pos = event->pos();

    if(pos.x() != T::x())
        emit m_object->xChanged(pos.x());

    if(pos.y() != T::y())
        emit m_object->yChanged(pos.y());

    T::moveEvent(event);
}

#endif // ANCHORS_H
