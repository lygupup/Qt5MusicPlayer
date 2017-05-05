#include "aboutform.h"
#include <QPainter>
#include <QPushButton>
#include <QMouseEvent>
#include <QApplication>

aboutForm::aboutForm(QWidget *parent)
    :QDialog(parent)
{
    exitButton = new QPushButton(this);
    exitButton->setIcon(QIcon(":/Resources/close.png"));
    exitButton->setGeometry(QRect(255, 12, 30, 30));

    exitButton->setIconSize(QSize(20,20));
    exitButton->setFlat(true);
    exitButton->setFocusPolicy(Qt::NoFocus);
    exitButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

    connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));

    this->setWindowFlags(Qt::FramelessWindowHint);
    //设置窗体背景透明，配合paintEvent设置不规则背景图
    setAttribute(Qt::WA_TranslucentBackground);

    //半透明
    this->setWindowOpacity(0.9);

    //固定大小
    this->setFixedSize(300, 400);

    //设置本窗口为模式窗口（即弹出后无法点击主界面）
    this->setModal(true);

}

aboutForm:: ~ aboutForm()
{

}

void aboutForm::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawPixmap(0, 0 , QPixmap(":/Resources/about.png"));
}

void aboutForm::mousePressEvent(QMouseEvent *event)
{
    mMoveing = true;
    //记录下鼠标相对于窗口的位置
    //event->globalPos()鼠标按下时，鼠标相对于整个屏幕位置
    //pos() this->pos()鼠标按下时，窗口相对于整个屏幕位置
    mMovePosition = event->globalPos() - pos();
    return QDialog::mousePressEvent(event);
}

//重写鼠标移动事件
void aboutForm::mouseMoveEvent(QMouseEvent *event)
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
    return QDialog::mouseMoveEvent(event);
}

void aboutForm::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    mMoveing = false;
}


