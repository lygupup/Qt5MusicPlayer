#ifndef ICELRC_H
#define ICELRC_H

#include <QWidget>
#include "lrclabel.h"
class QPaintEvent;

class iceLrc : public QWidget
{
    Q_OBJECT
public:
    explicit iceLrc(QWidget *parent = 0);
    ~ iceLrc();

    void startLrcMask(qint64 intervaltime);

    void stopLrcMask();

    QString text() const;

    void setText(const QString t);

protected:
    //窗口的自由移动
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:

    void paintEvent(QPaintEvent *event);

    lrcLabel *lrclabel;

    bool        mMoveing;
    QPoint      mMovePosition;

};

#endif // ICELRC_H
