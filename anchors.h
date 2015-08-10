#ifndef ANCHORS_H
#define ANCHORS_H

#include <QObject>
#include <QWidget>

class AnchorsBasePrivate;
class AnchorsBase
{
public:
    AnchorsBase(QWidget *w);
    virtual ~AnchorsBase();

    struct AnchorsInfo{
        QWidget *me;
        QWidget *target;
        Qt::AnchorPoint type;
        Qt::AnchorPoint targetType;

        AnchorsInfo(QWidget *w, Qt::AnchorPoint tp):
            me(w),
            target(NULL),
            type(tp),
            targetType(tp)
        {
        }
    };

    const AnchorsInfo *top();
    const AnchorsInfo * bottom();
    const AnchorsInfo * left();
    const AnchorsInfo * right();
    const AnchorsInfo * horizontalCenter();
    const AnchorsInfo * verticalCenter();
    const QWidget * fill();
    const QWidget * centerIn();
    int margins();
    int topMargin();
    int bottomMargin();
    int leftMargin();
    int rightMargin();
    int horizontalCenterOffset();
    int verticalCenterOffset();
    int alignWhenCentered();

    AnchorsBasePrivate *anchors() const;
private:
    QWidget *m_widget;
    AnchorsBasePrivate *d_ptr;
    Q_DECLARE_PRIVATE(AnchorsBase)

    virtual void _anchorsbase_(){}
};

class AnchorsBasePrivate: public QObject
{
    Q_OBJECT
protected:
    AnchorsBasePrivate(AnchorsBase *parent);
    ~AnchorsBasePrivate();
public Q_SLOTS:
    void top(const AnchorsBase::AnchorsInfo *info);
    void bottom(const AnchorsBase::AnchorsInfo *info);
    void left(const AnchorsBase::AnchorsInfo *info);
    void right(const AnchorsBase::AnchorsInfo *info);
    void horizontalCenter(const AnchorsBase::AnchorsInfo *info);
    void verticalCenter(const AnchorsBase::AnchorsInfo *info);
    void fill(const QWidget *w);
    void centerIn(const QWidget *w);
    void margins(int m);
    void topMargin(int m);
    void bottomMargin(int m);
    void leftMargin(int m);
    void rightMargin(int m);
    void horizontalCenterOffset(int o);
    void verticalCenterOffset(int o);
    void alignWhenCentered(bool c);
private:
    AnchorsBase::AnchorsInfo* m_top;
    AnchorsBase::AnchorsInfo* m_bottom;
    AnchorsBase::AnchorsInfo* m_left;
    AnchorsBase::AnchorsInfo* m_right;
    AnchorsBase::AnchorsInfo* m_horizontalCenter;
    AnchorsBase::AnchorsInfo* m_verticalCenter;
    const QWidget* m_fill;
    const QWidget* m_centerIn;
    int m_margins;
    int m_topMargin;
    int m_bottomMargin;
    int m_leftMargin;
    int m_rightMargin;
    int m_horizontalCenterOffset;
    int m_verticalCenterOffset;
    bool m_alignWhenCentered;

    AnchorsBase *q_ptr;
    Q_DECLARE_PUBLIC(AnchorsBase)
};

template <class T>
class Anchors: public AnchorsBase, public T
{
public:
    explicit Anchors(QWidget *parent = 0):
        AnchorsBase(this),
        T(parent)
    {
    }

    static Anchors<T> *cast(AnchorsBase* tt)
    {
        return reinterpret_cast<Anchors<T>*>(tt);
    }
};

#endif // ANCHORS_H
