#ifndef ICEPLAYER_H
#define ICEPLAYER_H

#include <QMap>
#include <QWidget>
#include <QMediaPlayer>
#include "icelrc.h"
#include "aboutform.h"
#include "ui_IcePlayer.h"

class QMouseEvent;
class QMediaPlaylist;

//文字滚动速度
#define TEXT_SPEED 500

//枚举播放模式
enum PlayMode{SINGAL, LISTCIRCLE, SINGALCIRCLE, RANDOM};


class IcePlayer : public QWidget, Ui::IcePlayer
{
    Q_OBJECT

public:
    IcePlayer(QWidget *parent = 0);
    ~IcePlayer();

protected:
    //窗口的自由移动
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    //拖拽添加
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);


private slots:
    // 处理mediaplayer发来的信号
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void updateState(QMediaPlayer::State);
    void updateMetaData();

    // 处理playslider发来的信号
    void setPlayPosition(int position);
    int getPlayPosition();
    void setPosition();

    // 处理listwidget发来的信号
    void cellDoubleClicked(int row, int);
    void tableMenuRequested(const QPoint &pos);

    // 处理按钮的点击信号
    void openMusic();
    void playButtonClicked();
    void lyricButtonClicked();
    void logoButtonClicked();
    void modeButtonClicked();
    void nextButtonClicked();
    void lastButtonClicked();
    void volButtonClicked();
    void minButtonClicked();

    // 设置播放模式
    void setPlayMode();

    // 设置table有件菜单弹出事件
    void openDir();
    void removeCurrentMusic();
    void clearList();


private:
    void initUi();
    void initPlayer();
    void initConnection();
    void initWindows();
    void initMenuActions();
    void initNetwork();

    void addPlayList(QStringList list);
    void setColorTable();
    void readList();
    void writeList();



private:
    bool        mMoveing;
    QPoint      mMovePosition;

    QMediaPlayer *m_player;
    QMediaPlaylist *m_playlist;

    QStringList playList;
    QString playingFile;
    QString iceDir;

    int preIndex;
    int currentIndex;
    int playMode;

    QString songName;
    QString songArtist;

    QMenu *contextMenuLess;
    QMenu *contextMenuMore;
    QMenu *playModeMenu;

    QActionGroup *modeActionGroup;

    //enum ICE_Play_Mode{SINGAL, LISTCIRCLE, SINGALCIRCLE, RANDOM};
    QAction *modeSingal;
    QAction *modeListCircle;
    QAction *modeSingalCircle;
    QAction *modeRandom;
    QAction *addMusic;
    QAction *addFileDiv;
    QAction *removeCurr;
    QAction *removeAll;

    int playPosition;
    int receiveState;

    aboutForm *m_aboutForm;
    iceLrc *m_lrc;

};
#endif // ICEPLAYER_H
