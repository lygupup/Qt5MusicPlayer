#include "lrclabel.h"
#include <QMenu>
#include <QPainter>
#include <QPaintEvent>
#include <QContextMenuEvent>
#include <QDebug>

lrcLabel::lrcLabel(QWidget *parent)
    :iceLabel(parent)
{
//    setText(QString::fromLocal8Bit("冰点播放器"));

    //歌词的线性渐变填充
    linearGradient.setStart(0, 10);//填充的起点坐标
    linearGradient.setFinalStop(0, 40);//填充的终点坐标
    linearGradient.setColorAt(0, QColor(1, 150, 255));

    // 设置字体
    lrcFont.setFamily(QString::fromLocal8Bit("微软雅黑"));
    //lrcFont.setBold(true);
    lrcFont.setPointSize(25);

}

void lrcLabel::startLycMask(qint64 intervaltime)
{

}

void lrcLabel::stopLycMask()
{
   update();;
}

void lrcLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);


    QPainter painter(this);
    painter.setFont(lrcFont);

    QString tmp = text();
//    qDebug()<<text();
    tmp.toUtf8();

    // 先绘制底层文字，作为阴影，这样会使显示效果更加清晰，且更有质感
    painter.setPen(QColor(90, 105, 115, 200));
    painter.drawText(1, 1, 800, 60, Qt::AlignCenter, tmp);

    // 再在上面绘制文字
    painter.setPen(QPen(linearGradient, 0));
    painter.drawText(0, 0, 800, 60, Qt::AlignCenter, tmp);

}

void lrcLabel::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(QString::fromLocal8Bit("隐藏"), this->parent(), SLOT(hide()));
    menu.exec(event->globalPos());//globalPos()为当前鼠标的位置坐标
}

void lrcLabel::setSize(int size)
{
    lrcFont.setPointSize(size);
}
