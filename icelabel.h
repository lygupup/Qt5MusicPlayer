#ifndef ICELABEL_H
#define ICELABEL_H

#include <QLabel>
#include <QTimer>

class iceLabel :public QLabel
{
    Q_OBJECT
public:
    explicit iceLabel(QWidget *parent = 0);
    ~ iceLabel();
    void ice_setText(QString &content , const int &ms );
    void stop();



private slots:
    void timeOut();

private:
    void init(int ms = 0);

    QString m_content;
    QTimer *m_timer;

    int mMs;
    int mPos;
};

#endif // ICELABEL_H
