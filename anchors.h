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

    Q_PROPERTY(QWidget *target READ target WRITE setTarget NOTIFY targetChanged)
public:
    explicit ExtendWidget(QWidget *w, QObject *parent = 0);
    ~ExtendWidget();

    QWidget *target() const;

public slots:
    void setTarget(QWidget *target);

signals:
    void xChanged(int x);
    void yChanged(int y);
    void positionChanged(const QPoint &point);
    void widthChanged(int width);
    void heightChanged(int height);
    void sizeChanged(const QSize &size);
    void targetChanged(QWidget *target);

protected:
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;

private:
    explicit ExtendWidget(ExtendWidgetPrivate *dd, QWidget *w, QObject *parent = 0);

    ExtendWidgetPrivate *d_ptr;

    Q_DECLARE_PRIVATE(ExtendWidget)
};

class AnchorsBase;
struct AnchorInfo {
    AnchorInfo(AnchorsBase *b, const Qt::AnchorPoint &t):
        base(b),
        type(t)
    {
    }

    AnchorsBase *base;
    Qt::AnchorPoint type;
    const AnchorInfo *targetInfo = NULL;

    bool operator==(const AnchorInfo *info) const
    {
        return info == targetInfo;
    }

    bool operator==(const AnchorInfo &info) const
    {
        return &info == targetInfo;
    }

    bool operator!=(const AnchorInfo *info) const
    {
        return info != targetInfo;
    }

    bool operator!=(const AnchorInfo &info) const
    {
        return &info != targetInfo;
    }

    const AnchorInfo &operator=(const AnchorInfo *info)
    {
        targetInfo = info;

        return *this;
    }
};

class ARect: public QRect
{
public:
    ARect() {}
    ARect(const QRect &rect): QRect(rect.topLeft(), rect.bottomRight()) {}

    const QRect &operator=(const QRect &rect)
    {
        setTopLeft(rect.topLeft());
        setBottomRight(rect.bottomRight());

        return *this;
    }

    inline qreal horizontalCenter()
    {
        return left() + width() / 2.0;
    }

    inline void moveHorizontalCenter(int arg)
    {
        moveLeft(arg - width() / 2.0);
    }

    inline qreal verticalCenter()
    {
        return top() + height() / 2.0;
    }

    inline void moveVerticalCenter(int arg)
    {
        moveTop(arg - height() / 2.0);
    }

    inline void setTop(int arg, Qt::AnchorPoint point);
    inline void setVerticalCenter(qreal arg, Qt::AnchorPoint point);
    inline void setBottom(int arg, Qt::AnchorPoint point);
    inline void setLeft(int arg, Qt::AnchorPoint point);
    inline void setHorizontalCenter(qreal arg, Qt::AnchorPoint point);
    inline void setRight(int arg, Qt::AnchorPoint point);
};

class AnchorsBasePrivate;
class AnchorsBase : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QWidget *target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(const AnchorsBase *anchors READ anchors)
    Q_PROPERTY(const AnchorInfo *top READ top WRITE setTop NOTIFY topChanged)
    Q_PROPERTY(const AnchorInfo *bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
    Q_PROPERTY(const AnchorInfo *left READ left WRITE setLeft NOTIFY leftChanged)
    Q_PROPERTY(const AnchorInfo *right READ right WRITE setRight NOTIFY rightChanged)
    Q_PROPERTY(const AnchorInfo *horizontalCenter READ horizontalCenter WRITE setHorizontalCenter NOTIFY horizontalCenterChanged)
    Q_PROPERTY(const AnchorInfo *verticalCenter READ verticalCenter WRITE setVerticalCenter NOTIFY verticalCenterChanged)
    Q_PROPERTY(QWidget *fill READ fill WRITE setFill NOTIFY fillChanged)
    Q_PROPERTY(QWidget *centerIn READ centerIn WRITE setCenterIn NOTIFY centerInChanged)
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

    enum AnchorError {
        NoError,
        Conflict,
        TargetInvalid,
        PointInvalid,
        LoopBind
    };

    QWidget *target() const;
    const AnchorsBase *anchors() const;
    const AnchorInfo *top() const;
    const AnchorInfo *bottom() const;
    const AnchorInfo *left() const;
    const AnchorInfo *right() const;
    const AnchorInfo *horizontalCenter() const;
    const AnchorInfo *verticalCenter() const;
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
    bool isBinding(const AnchorInfo *info) const;

    static bool setAnchor(QWidget *w, const Qt::AnchorPoint &p, QWidget *target, const Qt::AnchorPoint &point);

public slots:
    void setTarget(QWidget *target);
    bool setAnchor(const Qt::AnchorPoint &p, QWidget *target, const Qt::AnchorPoint &point);
    bool setTop(const AnchorInfo *top);
    bool setBottom(const AnchorInfo *bottom);
    bool setLeft(const AnchorInfo *left);
    bool setRight(const AnchorInfo *right);
    bool setHorizontalCenter(const AnchorInfo *horizontalCenter);
    bool setVerticalCenter(const AnchorInfo *verticalCenter);
    bool setFill(QWidget *fill);
    bool setCenterIn(QWidget *centerIn);
    bool setFill(AnchorsBase *fill);
    bool setCenterIn(AnchorsBase *centerIn);
    void setMargins(int margins);
    void setTopMargin(int topMargin);
    void setBottomMargin(int bottomMargin);
    void setLeftMargin(int leftMargin);
    void setRightMargin(int rightMargin);
    void setHorizontalCenterOffset(int horizontalCenterOffset);
    void setVerticalCenterOffset(int verticalCenterOffset);
    void setAlignWhenCentered(bool alignWhenCentered);

    void setTop(int arg, Qt::AnchorPoint point);
    void setBottom(int arg, Qt::AnchorPoint point);
    void setLeft(int arg, Qt::AnchorPoint point);
    void setRight(int arg, Qt::AnchorPoint point);
    void setHorizontalCenter(int arg, Qt::AnchorPoint point);
    void setVerticalCenter(int arg, Qt::AnchorPoint point);

    void moveTop(int arg);
    void moveBottom(int arg);
    void moveLeft(int arg);
    void moveRight(int arg);
    void moveHorizontalCenter(int arg);
    void moveVerticalCenter(int arg);
    void moveCenter(const QPoint &arg);

private slots:
    void updateVertical();
    void updateHorizontal();
    void updateFill();
    void updateCenterIn();

signals:
    void topChanged(const AnchorInfo *top);
    void bottomChanged(const AnchorInfo *bottom);
    void leftChanged(const AnchorInfo *left);
    void rightChanged(const AnchorInfo *right);
    void horizontalCenterChanged(const AnchorInfo *horizontalCenter);
    void verticalCenterChanged(const AnchorInfo *verticalCenter);
    void fillChanged(QWidget *fill);
    void centerInChanged(QWidget *centerIn);
    void marginsChanged(int margins);
    void topMarginChanged(int topMargin);
    void bottomMarginChanged(int bottomMargin);
    void leftMarginChanged(int leftMargin);
    void rightMarginChanged(int rightMargin);
    void horizontalCenterOffsetChanged(int horizontalCenterOffset);
    void verticalCenterOffsetChanged(int verticalCenterOffset);
    void alignWhenCenteredChanged(bool alignWhenCentered);

    void targetChanged(QWidget *target);

protected:
    explicit AnchorsBase(AnchorsBasePrivate *dd, QWidget *w);
    void setWidget(QWidget *w);

    AnchorsBasePrivate *d_ptr;

    Q_DECLARE_PRIVATE(AnchorsBase)

private:
    void init(QWidget *w);
};

template<class T>
class Anchors : public AnchorsBase
{
public:
    inline Anchors(): AnchorsBase(new T)
    {
        m_widget = qobject_cast<T *>(target());
    }
    inline Anchors(T *w): AnchorsBase(w), m_widget(w) {}
    inline Anchors(const Anchors &me): AnchorsBase(me.m_widget), m_widget(me.m_widget) {}

    inline T &operator=(const Anchors &me)
    {
        m_widget = me.m_widget, setTarget(m_widget);
        return *m_widget;
    }
    inline T &operator=(T *w)
    {
        m_widget = w;
        setTarget(w);
        return *m_widget;
    }
    inline T *widget() const
    {
        return m_widget;
    }
    inline T *operator ->() const
    {
        return m_widget;
    }
    inline T &operator *() const
    {
        return *m_widget;
    }
    inline operator T *() const
    {
        return m_widget;
    }
    inline operator T &() const
    {
        return *m_widget;
    }

private:
    T *m_widget;
};

#endif // ANCHORS_H
