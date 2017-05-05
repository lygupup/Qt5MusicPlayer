#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include <QDialog>
class QPushButton;
class QMouseEvent;

class aboutForm :public QDialog
{
public:
    explicit aboutForm(QWidget *parent = 0);
    ~ aboutForm();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    void paintEvent(QPaintEvent *event);
    QPushButton *exitButton;
    bool mMoveing;
    QPoint mMovePosition;


};

#endif // ABOUTFORM_H
