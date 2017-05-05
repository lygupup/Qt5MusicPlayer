#ifndef LRCLABEL_H
#define LRCLABEL_H

#include "icelabel.h"
#include <QFont>
#include <QLinearGradient>

class QPaintEvent;
class QContextMenuEvent;

class lrcLabel :public iceLabel
{
    Q_OBJECT
public:
    explicit lrcLabel(QWidget *parent = 0);
    void startLycMask(qint64 intervaltime);
    void stopLycMask();
    void setSize(int size);

protected:
    void paintEvent(QPaintEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

private:
    QLinearGradient linearGradient;
    QFont lrcFont;
};

#endif // LRCLABEL_H
