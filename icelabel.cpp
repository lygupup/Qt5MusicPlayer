#include "icelabel.h"

iceLabel::iceLabel(QWidget *parent)
    :QLabel(parent)
{
    init();
}

iceLabel::~iceLabel()
{
    stop();
    delete m_timer;
    m_timer = nullptr;
}

void iceLabel::init(int ms)
{
    m_timer = nullptr;
    mMs = ms;
    m_content = "";
    mPos = 0;
}

void iceLabel::ice_setText(QString &content, const int &ms)
{

    bool b = false;
    m_content = content;
    mMs = ms;
    if(mMs != 0)
    {
        // 这里如果有音乐播放暂停后，定时器会处于休眠状态
        if(m_timer == nullptr || m_timer->isActive() == false)
        {
            m_timer = new QTimer(this);
            if(m_timer)
            {
                b = connect(m_timer, SIGNAL(timeout()), this, SLOT(timeOut()));
                m_timer->start(mMs);
            }
        }
    }
}

void iceLabel::timeOut()
{
    QString str = "";
    if(mPos > m_content.count())
    {
        mPos = 0;
    }

    str = m_content.mid(mPos);

    setText(str);
    mPos++;
}

void iceLabel::stop()
{
    if(m_timer)
    {
        disconnect(m_timer, SIGNAL(timeout()), this, SLOT(timeOut()));
        if(m_timer->isActive())
        {
            m_timer->stop();
        }
    }
}
