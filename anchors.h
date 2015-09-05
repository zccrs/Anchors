#ifndef ANCHORS_H
#define ANCHORS_H

#include <QObject>
#include <QPointer>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QWidget>
#include <QDebug>

class ExtendWidgetPrivate;
class ExtendWidget: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QWidget* target READ target WRITE setTarget NOTIFY targetChanged)
public:
    explicit ExtendWidget(QWidget *w, QObject *parent = 0);
    ~ExtendWidget();

    QWidget* target() const;

public slots:
    void setTarget(QWidget* target);

signals:
    void xChanged(int x);
    void yChanged(int y);
    void positionChanged(const QPoint& point);
    void widthChanged(int width);
    void heightChanged(int height);
    void sizeChanged(const QSize &size);
    void targetChanged(QWidget* target);

protected:
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;

private:
    explicit ExtendWidget(ExtendWidgetPrivate *dd, QWidget *w, QObject *parent = 0);

    ExtendWidgetPrivate *d_ptr;

    Q_DECLARE_PRIVATE(ExtendWidget)
};

class AnchorsBase;
struct AnchorsInfo
{
    AnchorsInfo(AnchorsBase *b, const Qt::AnchorPoint &t):
        base(b),
        type(t)
    {
    }

    AnchorsBase *base;
    Qt::AnchorPoint type;
    const AnchorsInfo *targetInfo = NULL;

    bool operator==(const AnchorsInfo *info) const{
        return info == targetInfo;
    }

    bool operator==(const AnchorsInfo &info) const{
        return &info == targetInfo;
    }

    bool operator!=(const AnchorsInfo *info) const{
        return info != targetInfo;
    }

    bool operator!=(const AnchorsInfo &info) const{
        return &info != targetInfo;
    }

    const AnchorsInfo& operator=(const AnchorsInfo *info){
        targetInfo = info;

        return *this;
    }
};

class AnchorsRect: public QRect
{
public:
    AnchorsRect(const QRect &rect): QRect(rect.left(), rect.top(), rect.right(), rect.bottom()){}
    const QRect& operator=(const QRect &rect){
        setTopLeft(rect.topLeft());
        setBottomRight(rect.bottomRight());

        return *this;
    }

    int horizontalCenter(){
        return left() + width()/2;
    }

    void moveHorizontalCenter(int arg){
        moveLeft(arg - width()/2);
    }

    int verticalCenter(){
        return top() + height()/2;
    }

    void moveVerticalCenter(int arg){
        moveTop(arg - height()/2);
    }

    void setMargins(int arg){
        if(topMargin == 0)
            setTop(top() + arg);
        if(bottomMargin == 0)
            setBottom(bottom() - arg);
        if(leftMargin == 0)
            setLeft(left() + arg);
        if(rightMargin == 0)
            setRight(right() - arg);
    }

    void setTopMargin(int arg){

    }

private:
    int margins = 0;
    int topMargin = 0;
    int bottomMargin = 0;
    int leftMargin = 0;
    int rightMargin = 0;
};

class AnchorsBasePrivate;
class AnchorsBase : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QWidget* target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(const AnchorsBase* anchors READ anchors)
    Q_PROPERTY(const AnchorsInfo* top READ top WRITE setTop NOTIFY topChanged)
    Q_PROPERTY(const AnchorsInfo* bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
    Q_PROPERTY(const AnchorsInfo* left READ left WRITE setLeft NOTIFY leftChanged)
    Q_PROPERTY(const AnchorsInfo* right READ right WRITE setRight NOTIFY rightChanged)
    Q_PROPERTY(const AnchorsInfo* horizontalCenter READ horizontalCenter WRITE setHorizontalCenter NOTIFY horizontalCenterChanged)
    Q_PROPERTY(const AnchorsInfo* verticalCenter READ verticalCenter WRITE setVerticalCenter NOTIFY verticalCenterChanged)
    Q_PROPERTY(QWidget* fill READ fill WRITE setFill NOTIFY fillChanged)
    Q_PROPERTY(QWidget* centerIn READ centerIn WRITE setCenterIn NOTIFY centerInChanged)
    Q_PROPERTY(int margins READ margins WRITE setMargins NOTIFY marginsChanged)
    Q_PROPERTY(int topMargin READ topMargin WRITE setTopMargin NOTIFY topMarginChanged)
    Q_PROPERTY(int bottomMargin READ bottomMargin WRITE setBottomMargin NOTIFY bottomMarginChanged)
    Q_PROPERTY(int leftMargin READ leftMargin WRITE setLeftMargin NOTIFY leftMarginChanged)
    Q_PROPERTY(int rightMargin READ rightMargin WRITE setRightMargin NOTIFY rightMarginChanged)
    Q_PROPERTY(int horizontalCenterOffset READ horizontalCenterOffset WRITE setHorizontalCenterOffset NOTIFY horizontalCenterOffsetChanged)
    Q_PROPERTY(int verticalCenterOffset READ verticalCenterOffset WRITE setVerticalCenterOffset NOTIFY verticalCenterOffsetChanged)
    Q_PROPERTY(bool alignWhenCentered READ alignWhenCentered WRITE setAlignWhenCentered NOTIFY alignWhenCenteredChanged)

public:
    explicit AnchorsBase(QWidget *w);
    ~AnchorsBase();

    enum AnchorError{
        NoError,
        Conflict,
        TargetInvalid,
        PointInvalid,
        LoopBind
    };

    QWidget* target() const;
    const AnchorsBase *anchors() const;
    const AnchorsInfo *top() const;
    const AnchorsInfo *bottom() const;
    const AnchorsInfo *left() const;
    const AnchorsInfo *right() const;
    const AnchorsInfo *horizontalCenter() const;
    const AnchorsInfo *verticalCenter() const;
    QWidget *fill() const;
    QWidget *centerIn() const;
    int margins() const;
    int topMargin() const;
    int bottomMargin() const;
    int leftMargin() const;
    int rightMargin() const;
    int horizontalCenterOffset() const;
    int verticalCenterOffset() const;
    int alignWhenCentered() const;
    AnchorError errorCode() const;
    QString errorString() const;

    static bool setAnchor(QWidget *w, const Qt::AnchorPoint &p, QWidget *target, const Qt::AnchorPoint &point);

public slots:
    void setTarget(QWidget* target);
    bool setAnchor(const Qt::AnchorPoint &p, QWidget *target, const Qt::AnchorPoint &point);
    bool setTop(const AnchorsInfo* top);
    bool setBottom(const AnchorsInfo* bottom);
    bool setLeft(const AnchorsInfo* left);
    bool setRight(const AnchorsInfo* right);
    bool setHorizontalCenter(const AnchorsInfo* horizontalCenter);
    bool setVerticalCenter(const AnchorsInfo* verticalCenter);
    bool setFill(QWidget* fill);
    bool setCenterIn(QWidget* centerIn);
    void setMargins(int margins);
    void setTopMargin(int topMargin);
    void setBottomMargin(int bottomMargin);
    void setLeftMargin(int leftMargin);
    void setRightMargin(int rightMargin);
    void setHorizontalCenterOffset(int horizontalCenterOffset);
    void setVerticalCenterOffset(int verticalCenterOffset);
    void setAlignWhenCentered(bool alignWhenCentered);

    void setTop(int arg);
    void setBottom(int arg);
    void setLeft(int arg);
    void setRight(int arg);

    void moveTop(int arg);
    void moveBottom(int arg);
    void moveLeft(int arg);
    void moveRight(int arg);
    void moveHorizontalCenter(int arg);
    void moveVerticalCenter(int arg);
    void moveCenter(const QPoint &arg);

private slots:
    void updateTop();
    void updateBottom();
    void updateLeft();
    void updateRight();
    void updateHorizontalCenter();
    void updateVerticalCenter();
    void updateFill();
    void updateCenterIn();

signals:
    void topChanged(const AnchorsInfo* top);
    void bottomChanged(const AnchorsInfo* bottom);
    void leftChanged(const AnchorsInfo* left);
    void rightChanged(const AnchorsInfo* right);
    void horizontalCenterChanged(const AnchorsInfo* horizontalCenter);
    void verticalCenterChanged(const AnchorsInfo* verticalCenter);
    void fillChanged(QWidget* fill);
    void centerInChanged(QWidget* centerIn);
    void marginsChanged(int margins);
    void topMarginChanged(int topMargin);
    void bottomMarginChanged(int bottomMargin);
    void leftMarginChanged(int leftMargin);
    void rightMarginChanged(int rightMargin);
    void horizontalCenterOffsetChanged(int horizontalCenterOffset);
    void verticalCenterOffsetChanged(int verticalCenterOffset);
    void alignWhenCenteredChanged(bool alignWhenCentered);

    void targetChanged(QWidget* target);

protected:
    explicit AnchorsBase(AnchorsBasePrivate *dd, QWidget *w);
    void setWidget(QWidget *w);

    AnchorsBasePrivate *d_ptr;

    Q_DECLARE_PRIVATE(AnchorsBase)
};

template<class T>
class Anchors : public AnchorsBase
{
public:
    inline Anchors(): AnchorsBase(new T){m_widget = qobject_cast<T*>(target());}
    inline Anchors(T *w): AnchorsBase(w), m_widget(w){}
    inline Anchors(const Anchors &me): AnchorsBase(me.m_widget), m_widget(me.m_widget){}

    inline T &operator=(const Anchors &me)
    { m_widget = me.m_widget, setTarget(m_widget); return *m_widget; }
    inline T &operator=(T* w)
    { m_widget = w; setTarget(w); return *m_widget; }
    inline T* widget() const
    {return m_widget;}
    inline T* operator->() const
    { return widget(); }
    inline T& operator*() const
    { return *widget(); }
    inline operator T*() const
    { return widget(); }

private:
    T *m_widget;
};

#endif // ANCHORS_H
