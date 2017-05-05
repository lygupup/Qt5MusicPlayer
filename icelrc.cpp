#include "icelrc.h"
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>

iceLrc::iceLrc(QWidget *parent)
    :QWidget(parent)
{
    lrclabel = nullptr;

    //无边界的窗口,总在最上
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    //去除窗体原背景
    setAttribute(Qt::WA_TranslucentBackground);
    //固定大小
    this->setFixedSize(800, 80);

    lrclabel = new lrcLabel(this);
    lrclabel->setGeometry(QRect(0, 6, 800, 60));
}

iceLrc::~iceLrc()
{
    if(lrclabel != nullptr)
        delete lrclabel;
}

void iceLrc::mousePressEvent(QMouseEvent *event)
{
    mMoveing = true;
    //记录下鼠标相对于窗口的位置
    //event->globalPos()鼠标按下时，鼠标相对于整个屏幕位置
    //pos() this->pos()鼠标按下时，窗口相对于整个屏幕位置
    mMovePosition = event->globalPos() - pos();
    return QWidget::mousePressEvent(event);
}

void iceLrc::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawPixmap(0, 0, QPixmap(":/Resources/lrc_background.png"));

}

void iceLrc::startLrcMask(qint64 intervaltime)
{
    lrclabel->startLycMask(intervaltime);
}

void iceLrc::stopLrcMask()
{
    lrclabel->stopLycMask();
}

QString iceLrc::text() const
{
    return lrclabel->text();
}

void iceLrc::setText(const QString t)
{
    //if (t.size() > 35){
    //	lrcLabel->ICE_Set_Size(25);
    //	lrcLabel->setText(t);
    //}
    //else{
    //	lrcLabel->ICE_Set_Size(30);
    //	lrcLabel->setText(t);
    //}

    lrclabel->setText(t);
    lrclabel->stopLycMask();
}

//重写鼠标移动事件
void iceLrc::mouseMoveEvent(QMouseEvent *event)
{
    //(event->buttons() && Qt::LeftButton)按下是左键
    //鼠标移动事件需要移动窗口，窗口移动到哪里呢？就是要获取鼠标移动中，窗口在整个屏幕的坐标，然后move到这个坐标，怎么获取坐标？
    //通过事件event->globalPos()知道鼠标坐标，鼠标坐标减去鼠标相对于窗口位置，就是窗口在整个屏幕的坐标
    if (mMoveing && (event->buttons() && Qt::LeftButton)
        && (event->globalPos()-mMovePosition).manhattanLength() > QApplication::startDragDistance())
    {
        move(event->globalPos()-mMovePosition);
        mMovePosition = event->globalPos() - pos();
    }
    return QWidget::mouseMoveEvent(event);
}

void iceLrc::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    mMoveing = false;
}
