#include "iceplayer.h"
#include <QDir>
#include <QUrl>
#include <QList>
#include <QMenu>
#include <QTime>
#include <QImage>
#include <QBuffer>
#include <QColor>
#include <QPixmap>
#include <QMouseEvent>
#include <QMediaPlaylist>
#include <QStringList>
#include <QFileDialog>
#include <QFileInfo>
#include <QMediaMetaData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QDebug>

IcePlayer::IcePlayer(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    initUi();
    initPlayer();
    initConnection();
    initWindows();
    initMenuActions();
//    initNetwork();
    iceDir = QApplication::applicationDirPath();
    readList();
}

IcePlayer::~IcePlayer()
{
    if(m_aboutForm)
    {
        delete m_aboutForm;
        m_aboutForm = nullptr;
    }
}

void IcePlayer::initUi()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    this->setWindowOpacity(0.9);

    volumeSlider->setVisible(false);

    //设置音乐信息字体颜色
    nameLabel->setStyleSheet("color:rgb(1, 149, 255);");
    musicianLabel->setStyleSheet("color:rgb(1, 149, 255);");
    albumLabel->setStyleSheet("color:rgb(1, 149, 255);");
    timeLabel->setStyleSheet("color:rgb(106, 182, 240);");

    nameLabel->setFont(QFont(QString::fromLocal8Bit("微软雅黑"), 12));
    QFont othersfont(QString::fromLocal8Bit("微软雅黑"), 9);
    musicianLabel->setFont(othersfont);
    albumLabel->setFont(othersfont);
    timeLabel->setFont(othersfont);

    if (playlistTable->columnCount() < 1)
        playlistTable->setColumnCount(2);
    QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
    playlistTable->setHorizontalHeaderItem(0, __qtablewidgetitem);

    //设置列宽
    playlistTable->setColumnWidth(0,300);
    playlistTable->setColumnWidth(1,45);
    //禁止编辑单元格内容
    playlistTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //选中一行
    playlistTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    //去掉水平滚动条
    playlistTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //垂直滚动条按项移动
    playlistTable->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    //去除水平表头
    playlistTable->horizontalHeader()->setFixedHeight(0);
    //去除垂直表头
    playlistTable->verticalHeader()->setFixedWidth(0);
    //设置无边框
    playlistTable->setFrameShape(QFrame::NoFrame);
    //设置不显示格子线
    playlistTable->setShowGrid(false);
    //设置右键菜单
    playlistTable->setContextMenuPolicy(Qt::CustomContextMenu);
    //设置背景颜色
    QPalette pal;
    pal.setBrush(QPalette::Base, QBrush(QColor(255, 255, 255)));
    playlistTable->setPalette(pal);
    //设置竖直滚动条样式
    //使拖动操作生效
    playlistTable->setAcceptDrops(true);
    setAcceptDrops(true);

}

void IcePlayer::initPlayer()
{
    m_player = new QMediaPlayer(this);
    m_playlist = new QMediaPlaylist(this);
    m_player->setPlaylist(m_playlist);

    playMode = PlayMode::SINGAL;
    preIndex = -1;
    currentIndex = 0;
    m_player->setVolume(50);

    m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
}

void IcePlayer::initConnection()
{
    connect(m_player, SIGNAL(positionChanged(qint64)), this, SLOT(updatePosition(qint64)));
    connect(m_player, SIGNAL(durationChanged(qint64)), this, SLOT(updateDuration(qint64)));
    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)),this, SLOT(updateState(QMediaPlayer::State)));
    connect(m_player, SIGNAL(metaDataChanged()), this, SLOT(updateMetaData()));
    connect(playSlider, SIGNAL(sliderMoved(int)), this, SLOT(setPlayPosition(int)));
    connect(playSlider, SIGNAL(sliderReleased()), this, SLOT(setPosition()));
    connect(volumeSlider, SIGNAL(valueChanged(int)), m_player, SLOT(setVolume(int)));

    connect(playlistTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(cellDoubleClicked(int, int)));
    connect(playlistTable, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(tableMenuRequested(const QPoint &)));
    connect(addButton, SIGNAL(clicked()), this, SLOT(openMusic()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(playButton, SIGNAL(clicked()), this, SLOT(playButtonClicked()));
    connect(lyricButton, SIGNAL(clicked()), this, SLOT(lyricButtonClicked()));
    connect(logoButton, SIGNAL(clicked()), this, SLOT(logoButtonClicked()));
    connect(modeButton, SIGNAL(clicked()), this, SLOT(modeButtonClicked()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(nextButtonClicked()));
    connect(lastButton, SIGNAL(clicked()), this, SLOT(lastButtonClicked()));
    connect(volButton, SIGNAL(clicked()), this, SLOT(volButtonClicked()));
    connect(mminButton, SIGNAL(clicked()), this, SLOT(minButtonClicked()));
    connect(minButton, SIGNAL(clicked()), this, SLOT(showMinimized()));

}

void IcePlayer::initWindows()
{
    m_aboutForm = new aboutForm();
    m_lrc = new iceLrc();
}

void IcePlayer::initMenuActions()
{
    modeSingal = new QAction(QString::fromLocal8Bit("单曲播放"), this);
    modeListCircle = new QAction(QString::fromLocal8Bit("列表循环"), this);
    modeSingalCircle = new QAction(QString::fromLocal8Bit("单曲循环"), this);
    modeRandom = new QAction(QString::fromLocal8Bit("随机播放"), this);

    connect(modeSingal, SIGNAL(triggered()), this, SLOT(setPlayMode()));
    connect(modeListCircle, SIGNAL(triggered()), this, SLOT(setPlayMode()));
    connect(modeSingalCircle, SIGNAL(triggered()), this, SLOT(setPlayMode()));
    connect(modeRandom, SIGNAL(triggered()), this, SLOT(setPlayMode()));

    modeActionGroup = new QActionGroup(this);
    modeActionGroup->addAction(modeSingal);
    modeActionGroup->addAction(modeListCircle);
    modeActionGroup->addAction(modeSingalCircle);
    modeActionGroup->addAction(modeRandom);

    modeSingal->setCheckable(true);
    modeListCircle->setCheckable(true);
    modeSingalCircle->setCheckable(true);
    modeRandom->setCheckable(true);
    modeSingal->setChecked(true);

    playModeMenu = new QMenu(modeButton);
    playModeMenu->setStyleSheet(
        "QMenu{padding:5px;background:white;border:1px solid gray;}"
        "QMenu::item{padding:0px 40px 0px 30px;height:25px;}"
        "QMenu::item:selected:enabled{background:#0096ff;color:white;}"
        "QMenu::item:selected:!enabled{background:transparent;}"
        "QMenu::separator{height:1px;background:lightgray;margin:5px 0px 5px 0px;}");
    playModeMenu->addActions(modeActionGroup->actions());


    contextMenuLess = new QMenu(playlistTable);
    contextMenuMore = new QMenu(playlistTable);
    addMusic = new QAction(QString::fromLocal8Bit("添加歌曲"),this);
    addFileDiv = new QAction(QString::fromLocal8Bit("添加目录"),this);
    removeCurr = new QAction(QString::fromLocal8Bit("移除本曲"),this);
    removeAll = new QAction(QString::fromLocal8Bit("移除所有"),this);

    connect(addMusic, SIGNAL(triggered()), this, SLOT(openMusic()));
    connect(addFileDiv, SIGNAL(triggered()), this, SLOT(openDir()));
    connect(removeCurr, SIGNAL(triggered()), this, SLOT(removeCurrentMusic()));
    connect(removeAll, SIGNAL(triggered()), this, SLOT(clearList()));

    contextMenuLess->addAction(addMusic);
    contextMenuLess->addAction(addFileDiv);
    contextMenuLess->addSeparator();
    contextMenuLess->addAction(removeAll);

    contextMenuMore->addAction(addMusic);
    contextMenuMore->addAction(addFileDiv);
    contextMenuMore->addSeparator();
    contextMenuMore->addAction(removeCurr);
    contextMenuMore->addAction(removeAll);

    contextMenuLess->setStyleSheet(
        "QMenu{padding:5px;background:white;border:1px solid gray;}"
        "QMenu::item{padding:0px 40px 0px 30px;height:25px;}"
        "QMenu::item:selected:enabled{background:#0096ff;color:white;}"
        "QMenu::item:selected:!enabled{background:transparent;}"
        "QMenu::separator{height:1px;background:lightgray;margin:5px 0px 5px 0px;}");

    contextMenuMore->setStyleSheet(
        "QMenu{padding:5px;background:white;border:1px solid gray;}"
        "QMenu::item{padding:0px 40px 0px 30px;height:25px;}"
        "QMenu::item:selected:enabled{background:#0096ff;color:white;}"
        "QMenu::item:selected:!enabled{background:transparent;}"
        "QMenu::separator{height:1px;background:lightgray;margin:5px 0px 5px 0px;}");
}


void IcePlayer::updatePosition(qint64 position)
{
    if(!playSlider->isSliderDown())
    {
        playSlider->setValue(position);
    }

    qint64 duration = m_player->duration();
    QTime totalTime(0,(duration / 60000)% 60, (duration / 1000)%60);

    QTime currentTime(0,(position / 60000)% 60, (position / 1000)%60);
    timeLabel->setText(currentTime.toString(tr("mm::ss")));

}

void IcePlayer::updateDuration(qint64 duration)
{

    playSlider->setEnabled(true);
    playSlider->setRange(0, duration);
    playSlider->setEnabled(duration > 0);
    playSlider->setPageStep(duration / 10);
}

void IcePlayer::updateState(QMediaPlayer::State)
{
    if(m_player->state() == QMediaPlayer::StoppedState)
    {
        playButton->setIcon(QIcon(":/Resources/play.png"));
        playSlider->setEnabled(false);
    }
}

void IcePlayer::updateMetaData()
{
    currentIndex = m_playlist->currentIndex();
//    qDebug()<<playList;
//    qDebug()<<currentIndex;
//    这里如果是单次播放的话，返回的currentindex值为-1，需要判断才能使用
    if(currentIndex >= 0 && currentIndex < playList.size() )
    {
        playingFile = playList.at(currentIndex);
    }

    songName.clear();
    songArtist.clear();

    picLabel->setPixmap(QPixmap(":/Resources/default_album.jpg"));


    songName = m_player->metaData(QMediaMetaData::Title).toString();
    qDebug()<<songName;
    // 当前歌曲名合法的时候，我们就让它显示，不然设置为music
    if(songName != "")
    {
        m_lrc->setText(songName);
    }
    else
    {
        m_lrc->setText(QString::fromLocal8Bit("Music.."));
    }

    if (songName.size() > 2)
        nameLabel->ice_setText(songName, TEXT_SPEED);
    else{
        nameLabel->stop();
        nameLabel->setText(songName);
    }

    nameLabel->setText(songName);
    songArtist = m_player->metaData(QMediaMetaData::AlbumArtist).toString();
    musicianLabel->setText(songArtist);
    albumLabel->setText(m_player->metaData(QMediaMetaData::AlbumTitle).toString());
    //    qDebug()<<m_player->availableMetaData();

    if(nameLabel->text() == "")
        nameLabel->setText(QString::fromLocal8Bit("未知歌曲"));
    if(musicianLabel->text() == "")
        musicianLabel->setText(QString::fromLocal8Bit("未知音乐家"));
    if(albumLabel->text() == "")
        albumLabel->setText(QString::fromLocal8Bit("未知专辑"));

    if(playlistTable->rowCount() == 0){
        timeLabel->setText(tr("00:00"));
        nameLabel->setText(tr("Name " ));
        musicianLabel->setText(tr("Musician" ));
        albumLabel->setText(tr("Album" ));
        picLabel->setPixmap(QPixmap(":/Resources/default_album.jpg"));
    }
}

void IcePlayer::setPlayPosition(int position)
{
        playPosition = position;
}

int IcePlayer::getPlayPosition()
{
    return playPosition;
}

void IcePlayer::setPosition()
{
    int pos = getPlayPosition();
    m_player->setPosition(pos);

    // 细腻的控制进度条和play按钮对应的状态
    if(m_player->state() == QMediaPlayer::PausedState)
    {
        playButton->setIcon(QIcon(":/Resources/play.png"));
    }
    else
    {
        playButton->setIcon(QIcon(":/Resources/pause.png"));
    }
}

void IcePlayer::playButtonClicked()
{
    if(m_player->state() == QMediaPlayer::StoppedState)
    {
        m_player->play();
        playButton->setIcon(QIcon(":/Resources/pause.png"));
    }
    else if(m_player->state() == QMediaPlayer::PausedState)
    {
        m_player->play();
        playButton->setIcon(QIcon(":/Resources/pause.png"));
    }
    else
    {
        m_player->pause();
        playButton->setIcon(QIcon(":/Resources/play.png"));
    }
}

void IcePlayer::lyricButtonClicked()
{
    if(m_lrc->isHidden()) m_lrc->show();
    else m_lrc->hide();
}

void IcePlayer::logoButtonClicked()
{
    m_aboutForm->show();
}

void IcePlayer::modeButtonClicked()
{
    playModeMenu->exec(QCursor::pos());
}

void IcePlayer::nextButtonClicked()
{
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);

    if(m_playlist->currentIndex() != m_playlist->nextIndex())
    {
        m_playlist->setCurrentIndex(m_playlist->nextIndex());
    }
    else if (m_playlist->currentIndex() != m_playlist->previousIndex())
    {
        m_playlist->setCurrentIndex(m_playlist->previousIndex());
    }

    m_player->play();
    playlistTable->item(m_playlist->currentIndex(), 1)->setText(QString::fromLocal8Bit(""));
    //上下曲之后需要重新读取当前的播放模式
    setPlayMode();
}

void IcePlayer::lastButtonClicked()
{
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);

    if(m_playlist->currentIndex() != m_playlist->previousIndex())
        m_playlist->setCurrentIndex(m_playlist->previousIndex());
    else
        m_playlist->setCurrentIndex(m_playlist->nextIndex());

    if (!m_player->isAudioAvailable()){
        playlistTable->item(m_playlist->currentIndex(), 1)->setText(QString::fromLocal8Bit("失效"));
    }

    m_player->play();
    playlistTable->item(m_playlist->currentIndex(), 1)->setText(QString::fromLocal8Bit(""));
    //上下曲之后需要重新读取当前的播放模式
    setPlayMode();
}

void IcePlayer::volButtonClicked()
{
    if(volumeSlider->isHidden())
    {
        volumeSlider->show();
    }
    else
    {
        volumeSlider->hide();
    }
}

void IcePlayer::minButtonClicked()
{

}

void IcePlayer::cellDoubleClicked(int row, int)
{
    QFile file(playList.at(row));
    if(!file.open(QIODevice::ReadOnly))
    {
        playlistTable->item(row, 1)->setText(QString::fromLocal8Bit("失效"));
        return;
    }
    file.close();
    m_playlist->setCurrentIndex(row);
    m_player->play();
    playButton->setIcon(QIcon(":/Resources/pause.png"));
    playlistTable->item(row, 1)->setText(QString::fromLocal8Bit(""));
}

void IcePlayer::tableMenuRequested(const QPoint &pos)
{
    if(playlistTable->itemAt(pos))
    {
        currentIndex = playlistTable->rowAt(pos.y());
        contextMenuMore->exec(QCursor::pos());
    }
    else contextMenuLess->exec(QCursor::pos());
}

void IcePlayer::setPlayMode()
{
    if(modeSingal->isChecked())
    {
        playMode = PlayMode::SINGAL;
        m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    }
    else if(modeListCircle->isChecked())
    {
        playMode = PlayMode::LISTCIRCLE;
        m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }
    else if(modeSingalCircle->isChecked())
    {
        playMode = PlayMode::SINGALCIRCLE;
        m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }
    else if(modeRandom->isChecked())
    {
        playMode = PlayMode::RANDOM;
        m_playlist->setPlaybackMode(QMediaPlaylist::Random);
    }
}

void IcePlayer::openMusic()
{ 
    QStringList fileList = QFileDialog::getOpenFileNames(this,QString::fromLocal8Bit("添加音乐"),QString(),QString("MP3 (*.mp3)"));
    addPlayList(fileList);
}

void IcePlayer::openDir()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("选择目录"));
    if(dirPath.size() == 0)
        return;
    QDir dir(dirPath);
    QStringList fileList = dir.entryList(QStringList()<<"*.mp3", QDir::Files);
    for(int i = 0; i< fileList.size(); ++i)
        fileList[i] = dir.absoluteFilePath(fileList.at(i));
    addPlayList(fileList);
}

void IcePlayer::removeCurrentMusic()
{
    playList.removeAt(currentIndex);
    playlistTable->removeRow(currentIndex);
    m_playlist->removeMedia(currentIndex);

    setColorTable();
}

void IcePlayer::clearList()
{
    m_player->stop();
    playList.clear();
    m_playlist->clear();
    while(playlistTable->rowCount())
    playlistTable->removeRow(0);

    timeLabel->setText(tr("00:00"));
    nameLabel->setText(tr("Name " ));
    musicianLabel->setText(tr("Musician" ));
    albumLabel->setText(tr("Album" ));
}

void IcePlayer::addPlayList(QStringList list)
{
    foreach (QString fileName, list) {
       if(playList.contains(fileName))
        continue;
       playList.append(fileName);
       m_playlist->addMedia(QUrl::fromLocalFile(fileName));
       playlistTable->insertRow(playlistTable->rowCount());

       for(int i = 0; i< playlistTable->columnCount(); ++i)
       {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setFont(QFont(QString::fromLocal8Bit("微软雅黑"), 10));

            playlistTable->setItem(playlistTable->rowCount() - 1,i,item);
       }
       playlistTable->item(playlistTable->rowCount() - 1,0)->setText(QFileInfo(fileName).baseName());
    }
    setColorTable();
    writeList();
}

void IcePlayer::setColorTable()
{
    for(int i = 0; i< playlistTable->rowCount(); ++i)
    {
        for(int j = 0; j < playlistTable->colorCount(); ++j)
        {
            QTableWidgetItem *item = playlistTable->item(i, j);
            if(item)
            {
                item->setBackground(QColor(255, 255, 255));
            }
        }
        if(i % 2 == 0)
        {
            for(int j = 0; j < playlistTable->columnCount(); ++j)
            {
                QTableWidgetItem *item = playlistTable->item(i, j);
                if(item)
                {
                    item->setBackgroundColor(QColor(213, 238, 242));
                }
            }
        }
    }
}
void IcePlayer::readList()
{
    QFile text(iceDir + "/IceList.ilist");
    QStringList inList;

    if(text.open(QIODevice::ReadOnly))
    {
        QDataStream in(&text);
        in.setVersion(QDataStream::Qt_5_7);
        in>>inList;
        addPlayList(inList);
        text.close();
    }
}

void IcePlayer::writeList()
{
    QFile text(iceDir + "/IceList.ilist");
    QStringList outList = playList;

    if(text.open(QIODevice::WriteOnly))
    {
        QDataStream out(&text);
        out.setVersion(QDataStream::Qt_5_7);
        out<<outList;
        text.close();
    }
}

void IcePlayer::mousePressEvent(QMouseEvent *event)
{
    mMoveing = true;
    //记录下鼠标相对于窗口的位置
    //event->globalPos()鼠标按下时，鼠标相对于整个屏幕位置
    //pos() this->pos()鼠标按下时，窗口相对于整个屏幕位置
    mMovePosition = event->globalPos() - pos();
    return QWidget::mousePressEvent(event);
}

//重写鼠标移动事件
void IcePlayer::mouseMoveEvent(QMouseEvent *event)
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

void IcePlayer::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    mMoveing = false;
}

void IcePlayer::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void IcePlayer::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;
    QStringList dropfileNames;
    foreach (QUrl url, urls) {
       if(url.toString().right(4) == QString(".mp3"))
           dropfileNames.append(url.toLocalFile());
    }
    if(!dropfileNames.isEmpty())
        addPlayList(dropfileNames);
}
