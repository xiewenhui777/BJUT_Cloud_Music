#include "mainWidget.h"
#include "ui_mainWidget.h"
#include <QPainter>
#include "myqss.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QScrollBar>
#include "Music.h"
#include <QMouseEvent>
#include <QtSql>
#include <QMessageBox>
#include <QInputDialog>
#include "MusicListDialog.h"
#include "chuanshu.h"
#include <QProcess>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);
//    qDebug()<<"ss";

    this->userID="";
    //UI初始化（.ui文件中无法完成的设置，这里补上）
    init_UI();

    //初始化生成播放器模块及相关组件
    init_play();
    
    //右键菜单初始化
    init_actions();
    
    //数据库初始化
    init_sqlite();
    
    //歌单初始化
    init_musicList();
    
    //配置初始化
    init_settings();
    
    //系统托盘初始化（系统托盘即指播放的一些设置 如播放/暂停按键等设计）
    init_systemTrayIcon();
    tcpSocket=new QTcpSocket();
    tcpSocket->abort();
    //连接服务器
    tcpSocket->connectToHost("10.24.10.40", 8520);

    //等待连接成功
    if(!tcpSocket->waitForConnected(30000))
    {
        qDebug() << "Connection failed!";
        return;
    }
    qDebug() << "Connect successfully!";
    QObject::connect(tcpSocket, &QTcpSocket::readyRead, this, &MainWidget::socket_Read_Data);
}

MainWidget::~MainWidget()
{
    delete ui;
}
void MainWidget::paintEvent(QPaintEvent *event)
{
    //需要添加以下代码，才能正常在主窗口Widget中显示背景图片（https://blog.csdn.net/xiejie0226/article/details/81165379）
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void MainWidget::init_UI()
{
    //窗口设置圆角后，会出现留白，需要添加以下代码
    setAttribute(Qt::WA_TranslucentBackground, true);
    //去除标题栏
    setWindowFlags(Qt::FramelessWindowHint);
    
    //UI初始化（.ui文件中无法完成的设置，这里补上）
    ui->volumeSlider->hide();
    ui->playListWidget->verticalScrollBar()->setStyleSheet(ListWidgetStyle());  //设置滚动条
    ui->localMusicWidget->verticalScrollBar()->setStyleSheet(ListWidgetStyle());
    ui->favorMusicWidget->verticalScrollBar()->setStyleSheet(ListWidgetStyle());
    ui->searchWidget->verticalScrollBar()->setStyleSheet(ListWidgetStyle());

    //进行界面初始化时 歌单应该是隐藏起来的 等到点击我的歌单按钮后才显示出来
    ui->nameListWidget->verticalScrollBar()->setStyleSheet(ListWidgetStyle());
    ui->nameListWidget->hide();         //进行隐藏

    ui->musicListWidget->verticalScrollBar()->setStyleSheet(ListWidgetStyle());
    ui->playListWidget->setIcon(QIcon(":/image/image/image/music.png"));
    ui->localMusicWidget->setIcon(QIcon(":/image/image/image/music-file.png"));
    ui->favorMusicWidget->setIcon(QIcon(":/image/image/image/like.png"));
    ui->musicListWidget->setIcon(QIcon(":/image/image/image/MusicListItem.png"));
    ui->searchWidget->setIcon(QIcon(":/image/image/image/search.png"));

    //点赞按钮的实现
    ui->dianzan->setStyleSheet("QPushButton{image: url(:/image/image/image/dianzan.png);border:none;color:rgb(255, 255, 255);}");
}

void MainWidget::init_play()
{
    //播放器初始化
    player= new QMediaPlayer(this);
    playlist=new QMediaPlaylist;    
    playlist->setPlaybackMode(QMediaPlaylist::Loop);        //设置列表循环
    player->setPlaylist(playlist);      //设置播放列表
    //连接槽与信号
    connect(ui->positionSlider, &QAbstractSlider::valueChanged, this, &MainWidget::setPosition);//  进度条设置
    connect(player, &QMediaPlayer::positionChanged, this, &MainWidget::updatePosition);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWidget::updateDuration); //音频时长变化
    connect(player, &QMediaPlayer::metaDataAvailableChanged, this, &MainWidget::updateInfo);    //每次player对应的数据信息变化  展示的歌曲信息自动更新
    connect(player, &QMediaPlayer::stateChanged, this, &MainWidget::updatePlayBtn);
}

void MainWidget::init_actions()         //一系列的动作
{
    //“当前播放”列表右键菜单初始化
    ui->playListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *action_playlist_delete=new QAction(QIcon(":/image/image/image/remove.png"),u8"移除");
    connect(action_playlist_delete,&QAction::triggered,this,&MainWidget::playlist_removeMusic);
    QAction *action_playlist_showfile=new QAction(QIcon(":/image/image/image/music-dir.png"),u8"打开所在文件夹");
    connect(action_playlist_showfile,&QAction::triggered,ui->playListWidget,&MusicListWidget::showInExplorer);
    QAction *action_playlist_detail=new QAction(QIcon(":/image/image/image/detail.png"),u8"歌曲详情");
    connect(action_playlist_detail,&QAction::triggered,ui->playListWidget,&MusicListWidget::detail);
    QAction *action_play_to_favor=new QAction(QIcon(":/image/image/image/To-like.png"),u8"添加到我喜欢");
    connect(action_play_to_favor,&QAction::triggered,this,&MainWidget::play_to_favor);
    menu_playlist=new QMenu(this);
    menu_playlist->addAction(action_playlist_delete);
    menu_playlist->addAction(action_playlist_showfile);
    menu_playlist->addAction(action_playlist_detail);
    menu_playlist->addAction(action_play_to_favor);
    
    //“本地音乐”列表右键菜单初始化
    ui->localMusicWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *action_locallist_delete=new QAction(QIcon(":/image/image/image/remove.png"),u8"移除");
    connect(action_locallist_delete,&QAction::triggered,ui->localMusicWidget,&MusicListWidget::removeMusic);
    QAction *action_locallist_showfile=new QAction(QIcon(":/image/image/image/music-dir.png"),u8"打开所在文件夹");
    connect(action_locallist_showfile,&QAction::triggered,ui->localMusicWidget,&MusicListWidget::showInExplorer);
    QAction *action_locallist_detail=new QAction(QIcon(":/image/image/image/detail.png"),u8"歌曲详情");
    connect(action_locallist_detail,&QAction::triggered,ui->localMusicWidget,&MusicListWidget::detail);
    QAction *action_local_to_favor=new QAction(QIcon(":/image/image/image/To-like.png"),u8"添加到我喜欢");
    connect(action_local_to_favor,&QAction::triggered,this,&MainWidget::local_to_favor);
    QAction *action_local_to_playlist=new QAction(QIcon(":/image/image/image/To-playlist.png"),u8"添加到当前播放列表");
    connect(action_local_to_playlist,&QAction::triggered,this,&MainWidget::local_to_playlist);
    QAction *action_local_to_upload=new QAction(QIcon(":/image/image/image/upload.png"),u8"上传歌曲");
    connect(action_local_to_upload,&QAction::triggered,this,&MainWidget::local_to_upload);
    menu_locallist=new QMenu(this);
    menu_locallist->addAction(action_locallist_delete);
    menu_locallist->addAction(action_locallist_showfile);
    menu_locallist->addAction(action_locallist_detail);
    menu_locallist->addAction(action_local_to_favor);
    menu_locallist->addAction(action_local_to_playlist);
    menu_locallist->addAction(action_local_to_upload);
    
    //“我喜欢”列表右键菜单初始化
    ui->favorMusicWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *action_favorlist_delete = new QAction(QIcon(":/image/image/image/remove.png"),u8"移除");
	connect(action_favorlist_delete, &QAction::triggered, ui->favorMusicWidget, &MusicListWidget::removeMusic);
	QAction *action_favorlist_showfile = new QAction(QIcon(":/image/image/image/music-dir.png"),u8"打开所在文件夹");
	connect(action_favorlist_showfile, &QAction::triggered, ui->favorMusicWidget, &MusicListWidget::showInExplorer);
	QAction *action_favorlist_detail = new QAction(QIcon(":/image/image/image/detail.png"),u8"歌曲详情");
	connect(action_favorlist_detail, &QAction::triggered, ui->favorMusicWidget, &MusicListWidget::detail);
    QAction *action_favor_to_playlist=new QAction(QIcon(":/image/image/image/To-playlist.png"),u8"添加到当前播放列表");
    connect(action_favor_to_playlist,&QAction::triggered,this,&MainWidget::favor_to_playlist);
	menu_favorlist = new QMenu(this);
	menu_favorlist->addAction(action_favorlist_delete);
	menu_favorlist->addAction(action_favorlist_showfile);
	menu_favorlist->addAction(action_favorlist_detail);
    menu_favorlist->addAction(action_favor_to_playlist);
    
    //“我的歌单”列表右键菜单初始化
    ui->nameListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *action_namelist_delete=new QAction(QIcon(":/image/image/image/remove.png"),u8"删除");
    connect(action_namelist_delete,&QAction::triggered,this,&MainWidget::namelist_delete);
    menu_namelist=new QMenu(this);
    menu_namelist->addAction(action_namelist_delete);
    
    //歌单展示列表右键菜单初始化（即展示歌单musicListWidget中所包含的歌曲）
    ui->musicListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *action_musiclist_delete = new QAction(QIcon(":/image/image/image/remove.png"), u8"移除");
    connect(action_musiclist_delete, &QAction::triggered, this, &MainWidget::musiclist_removeMusic);
    QAction *action_musiclist_showfile = new QAction(QIcon(":/image/image/image/music-dir.png"), u8"打开所在文件夹");
    connect(action_musiclist_showfile, &QAction::triggered, ui->musicListWidget, &MusicListWidget::showInExplorer);
    QAction *action_musiclist_detail = new QAction(QIcon(":/image/image/image/detail.png"), u8"歌曲详情");
    connect(action_musiclist_detail, &QAction::triggered, ui->musicListWidget, &MusicListWidget::detail);
    QAction *action_musiclist_to_favor = new QAction(QIcon(":/image/image/image/To-like.png"), u8"添加到我喜欢");
    connect(action_musiclist_to_favor, &QAction::triggered, this, &MainWidget::musiclist_to_favor);
    QAction *action_musiclist_to_playlist = new QAction(QIcon(":/image/image/image/To-playlist.png"), u8"添加到当前播放列表");
    connect(action_musiclist_to_playlist, &QAction::triggered, this, &MainWidget::musiclist_to_playlist);
    menu_musiclist = new QMenu(this);
    menu_musiclist->addAction(action_musiclist_delete);
    menu_musiclist->addAction(action_musiclist_showfile);
    menu_musiclist->addAction(action_musiclist_detail);
    menu_musiclist->addAction(action_musiclist_to_favor);
    menu_musiclist->addAction(action_musiclist_to_playlist);

    //搜索结果界面初始化
    ui->searchWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *action_search_result=new QAction(QIcon(":/image/image/image/download1.png"),u8"下载");
    connect(action_search_result,&QAction::triggered,this,&MainWidget::download_music);
    menu_searchlist=new QMenu(this);
    menu_searchlist->addAction(action_search_result);

    //“换肤”的菜单项
    QAction *action_backgroud_to_default = new QAction(QIcon(":/image/image/image/default.png"),u8"更换到默认背景");
	connect(action_backgroud_to_default,&QAction::triggered,this,&MainWidget::background_to_default);
    QAction *action_backgroud_setting=new QAction(QIcon(":/image/image/image/setting.png"),u8"自定义背景");
    connect(action_backgroud_setting,&QAction::triggered,this,&MainWidget::background_setting);
    menu_changeSkin=new QMenu(this);//添加菜单选项卡
    menu_changeSkin->addAction(action_backgroud_to_default);
    menu_changeSkin->addAction(action_backgroud_setting);

    //歌曲搜索按钮的实现
    connect(ui->MusicSearch,SIGNAL(clicked()), this,SLOT(on_musicsraech_clicked()));
    connect(ui->comment,SIGNAL(clicked()), this,SLOT(on_commment_clicked()));
//    connect(ui->login,SIGNAL(clicked()), this,SLOT(on_login_clicked1()));
    connect(ui->login,SIGNAL(clicked()), this,SLOT(testlogin()));
    qDebug()<<"login test";
//    connect(ui->btnPersonal,SIGNAL(clicked()), this,SLOT(on_btnPersonal_clicked()));

}

void MainWidget::init_sqlite()
{
    QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection"))    //判断数据库是否已存在默认的
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else            //若不存在数据库则新建一个
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("Music.db");
        database.setUserName("BJUT");                //这部分是数据库设计
        database.setPassword("123456");
        if (!database.open())
        {
            QMessageBox::critical(this,"无法打开数据库文件：Music.db",database.lastError().databaseText());
            exit(-1);
        }
    }
    //检查两个表是否存在，不存在则创建不存在的表
    QSqlQuery query;
    query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg("MusicInfo"));
    if(query.next()){
        if(query.value(0).toInt()==0){
            QSqlQuery sql_query;
            //QString create_sql = "create table MusicInfo (id int primary key, name varchar(30), url varchar(200), author varchar(50), title varchar(50), duration bigint, albumTitle varchar(50), audioBitRate int)";
            QString create_sql = "create table MusicInfo (name varchar(30), url varchar(200), author varchar(50), title varchar(50), duration bigint, albumTitle varchar(50), audioBitRate int)";
            sql_query.prepare(create_sql);
            sql_query.exec();
        }
    }
    QSqlQuery query2;
    query2.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg("MusicLists"));
    if(query2.next()){
        if(query2.value(0).toInt()==0){
            QSqlQuery sql_query;
            QString create_sql = "create table MusicLists (name varchar(30))";
            sql_query.prepare(create_sql);
            sql_query.exec();
        }
    }
}

void MainWidget::init_musicList()
{
    //本地音乐 初始化（实际上也看做是一种歌单类型）
    ui->localMusicWidget->musicList.setName("LocalMusic");
    ui->localMusicWidget->musicList.read_fromSQL();     //读取资源
    ui->localMusicWidget->refresh();        //列表更新
    //我喜欢 初始化
    ui->favorMusicWidget->musicList.setName("FavorMusic");
    ui->favorMusicWidget->musicList.read_fromSQL();
    ui->favorMusicWidget->refresh();
    
    //从数据库中恢复歌单
    QSqlQuery sql_query;
    QString select_sql = "select name from MusicLists";
    sql_query.prepare(select_sql);
    if(sql_query.exec())
    {
        while(sql_query.next())
        {    
            QString musicListName=sql_query.value(0).toString();    //获取一条歌单名
            MusicList tempList;
            tempList.setName(musicListName);
            tempList.read_fromSQL();                //从数据库读取信息
            musiclist.push_back(tempList);      //将musiclist类型的歌单变量压入储存的vector数组中
        }  
    }
    namelist_refresh();
}

void MainWidget::namelist_refresh()
{
    //先清空
    QSqlQuery sql_query;
    QString delete_sql = "delete from MusicLists";
    sql_query.prepare(delete_sql);
    sql_query.exec();
    for(size_t i=0;i<musiclist.size();i++){
        QSqlQuery sql_query2;
        QString insert_sql = "insert into MusicLists values (?)";
        sql_query2.prepare(insert_sql);
        sql_query2.addBindValue(musiclist[i].getName());
        sql_query2.exec();    
    }
    //展示列表刷新
    ui->nameListWidget->clear();
    for(size_t i=0;i<musiclist.size();i++){
        QListWidgetItem *item = new QListWidgetItem;
        item->setIcon(QIcon(":/image/image/image/music_list.png"));
        item->setText(musiclist[i].getName());
        ui->nameListWidget->addItem(item);  
    }
}

void MainWidget::init_settings()        //一些初始的系统设置
{
    QSettings mysettings("./BJUT_Cloud_Music.ini",QSettings::IniFormat);
    mysettings.setIniCodec("UTF8");
    QString fileName = mysettings.value("background/image-url").toString();
    QImage testImage(fileName);
    if(!fileName.isEmpty()&&!testImage.isNull())
    {
        setStyleSheet(QString("QWidget#Widget{"
                              "border-radius:10px;"
                              "border-image: url(%1);}").arg(fileName));
    }else{
        fileName=":/image/image/background/default.jpg";
        QSettings mysettings("./BJUT_Cloud_Music.ini",QSettings::IniFormat);
        mysettings.setIniCodec("UTF8");
        mysettings.setValue("background/image-url",fileName);
        setStyleSheet(QString("QWidget#Widget{"
                              "border-radius:10px;"
                              "border-image: url(%1);}").arg(fileName));
    }
}

void MainWidget::musicListWidget_refresh()//歌单列表刷新
{
    if(musiclist_index != -1){
        ui->musicListNameLabel->setText(u8"歌单 - "+musiclist[musiclist_index].getName());
        ui->musicListWidget->setMusicList_playing(musiclist[musiclist_index]);
    }
}

void MainWidget::on_playListWidget_customContextMenuRequested(const QPoint &pos)
{
    if(ui->playListWidget->itemAt(pos)==Q_NULLPTR)
    {
        return;
    }
    menu_playlist->exec(QCursor::pos());
}

void MainWidget::playlist_removeMusic()     //当前播放列表移除歌曲
{
    int pos=ui->playListWidget->currentRow();
    int playing_pos=playlist->currentIndex();
    ui->playListWidget->removeMusic();          //MusicListWidget类型的移除歌曲
    if(pos<playing_pos){
        //移除前备份
        QMediaPlayer::State state=player->state();//记录下删除前的播放状态
        qint64 position_backup=player->position();      //播放进度
        playing_pos=playing_pos-1;
        playlist->removeMedia(pos);
        //移除后恢复
        playlist->setCurrentIndex(playing_pos);
        player->setPosition(position_backup);
        ui->positionSlider->setValue(position_backup);
        if(state==QMediaPlayer::PlayingState){
            player->play();
        }else if(state==QMediaPlayer::PlayingState){
            player->pause();
        }
    }else if(pos<playing_pos){
        playlist->removeMedia(pos);
        playlist->setCurrentIndex(pos);
    }else{
        playlist->removeMedia(pos);
    }
}

void MainWidget::play_to_favor()    //歌曲从当前播放添加到喜欢
{
    int pos=ui->playListWidget->currentRow();
    ui->favorMusicWidget->musicList.addMusic(ui->playListWidget->musicList.getMusic(pos));
    ui->favorMusicWidget->refresh();
}

void MainWidget::local_to_favor()
{
    int pos=ui->localMusicWidget->currentRow();
    ui->favorMusicWidget->musicList.addMusic(ui->localMusicWidget->musicList.getMusic(pos));
    ui->favorMusicWidget->refresh();
}

void MainWidget::local_to_playlist()
{
    int pos=ui->localMusicWidget->currentRow();
    Music tempMusic=ui->localMusicWidget->musicList.getMusic(pos);
    ui->playListWidget->musicList.addMusic(tempMusic);
    ui->playListWidget->refresh();
    //添加到播放器
    playlist->addMedia(tempMusic.getUrl());
}

void MainWidget::local_to_upload(){
    int pos=ui->localMusicWidget->currentRow();     //获取当前位置
    Music tempmusic=ui->localMusicWidget->musicList.getMusic(pos);


    //进行文件传送
    qDebug()<<"upload1";
    QStringList arguments;//用于传参数
    QString program = "‪D:\\Transfer\\send.exe"; //外部程序地址
    arguments <<"10.24.10.40"<<"8888";
    qDebug()<<"upload2";
    QProcess process(this);
    process.startDetached(program, arguments);//启动程序

    chuanshu *start=new chuanshu("0######0#");         //在准备登录时发送一个预告 告诉服务器准备登录了
    QString tempID="2";
    QString singerID="22";

    //替换掉#防止冲突
    QString originText = tempmusic.albumTitle;
    QString searchText = "#";
    QString replaceText = "";
    QString result = originText.replace(QRegExp(searchText), replaceText);

    QString s1="";
    s1+=tempmusic.title;
    s1+="$";
    s1+=tempmusic.author+"$";
    s1+="$";
    s1+=tempID+"$";
    s1+="type$";
    s1+=result+"$";         //唱片名称拼接
    s1+=singerID+"$";
    s1+=tempmusic.getUrl().toString();

    qDebug()<<"upload data:"<<s1;

    start->type = 2;
    start->info = s1;
    start->timer = "";
    start->name = userID;
    start->fileName = tempmusic.title;
    start->wantsendto = "";
    start->size = 0;
    start->ip = "10.24.10.40";

    QString sender="";
    sender+=QString::number(start->type)+"#"+(QString)start->info+"#"+(QString)start->timer+"#"+(QString)start->name+"#"+(QString)start->fileName+"#"+(QString)start->wantsendto+"#"+QString::number(start->size)+"#"+(QString)start->ip;

//  执行上传数据包
    char la=0xff;
    qDebug() <<sender.toUtf8();
    tcpSocket->write(sender.toUtf8()+la);
    tcpSocket->flush();
    qDebug() <<"upload over";


    process.close();
    qDebug()<<"upload3";

//    ui->favorMusicWidget->musicList.addMusic(ui->localMusicWidget->musicList.getMusic(pos));

    QMessageBox::information(this,QStringLiteral("上传"),QStringLiteral("上传成功"));
}

void MainWidget::favor_to_playlist()
{
    int pos=ui->favorMusicWidget->currentRow();
    Music tempMusic=ui->favorMusicWidget->musicList.getMusic(pos);
    ui->playListWidget->musicList.addMusic(tempMusic);
    ui->playListWidget->refresh();
    //添加到播放器
    playlist->addMedia(tempMusic.getUrl());
}

void MainWidget::namelist_delete()      //歌单列表中，某个歌曲的删除
{
    int pos=ui->nameListWidget->currentRow();   //定位歌单在歌单列表中的位置
    musiclist[pos].remove_SQL_all();//先从数据库中删除歌曲信息
    //移除歌单
    int i=0;
    for(auto it=musiclist.begin();it!=musiclist.end();){
        if(i==pos){
            it= musiclist.erase(it);
            break;
        }
        else{
            it++;
        }
        i++;
    }
    namelist_refresh();
}

void MainWidget::download_music(){      //在此处实现音乐的下载功能
    chuanshu *ss=new chuanshu("0######0#");
    ss->type = 3;
    ss->info = ui->SearchlineEdit->text();
    ss->timer = "";
    ss->name = userID;
    ss->fileName = "斯卡布罗集市";
    ss->wantsendto = "";
    ss->size = 0;
    ss->ip = "";

    QString sender="";
    sender+=QString::number(ss->type)+"#"+(QString)ss->info+"#"+(QString)ss->timer+"#"+(QString)ss->name+"#"+(QString)ss->fileName+"#"+(QString)ss->wantsendto+"#"+QString::number(ss->size)+"#"+(QString)ss->ip;

//        // 发送
    char la=0xff;
    qDebug() <<sender.toUtf8();
    tcpSocket->write(sender.toUtf8()+la);
    tcpSocket->flush();

    QMessageBox::information(this, QStringLiteral("下载"), QStringLiteral("下载成功"));
}

void MainWidget::musiclist_removeMusic()        //某一歌单中的歌曲进行移除
{
    int pos=ui->musicListWidget->currentRow();
    musiclist[musiclist_index].removeMusic(pos);
    musicListWidget_refresh();
}

void MainWidget::musiclist_to_favor()           //某一歌单中的歌曲添加到我喜欢
{
    int pos=ui->musicListWidget->currentRow();//获取当前歌曲所在歌单中的位置
    ui->favorMusicWidget->musicList.addMusic(musiclist[musiclist_index].getMusic(pos));
    ui->favorMusicWidget->refresh();
}

void MainWidget::musiclist_to_playlist()
{
    int pos=ui->musicListWidget->currentRow();
    Music tempMusic=ui->musicListWidget->musicList.getMusic(pos);
    ui->playListWidget->musicList.addMusic(tempMusic);
    ui->playListWidget->refresh();
    //添加到播放器
    playlist->addMedia(tempMusic.getUrl());
}

void MainWidget::background_to_default()        //默认的背景设置
{
    QString fileName=":/image/image/background/default.jpg";
    QSettings mysettings("./BJUT_Cloud_Music.ini",QSettings::IniFormat);
    mysettings.setIniCodec("UTF8");
    mysettings.setValue("background/image-url",fileName);
    setStyleSheet(QString("QWidget#Widget{"
                          "border-radius:10px;"
                          "border-image: url(%1);}").arg(fileName));
}

void MainWidget::background_setting()       //进行背景选择设置
{
    //从默认图片位置打开文件选择框
    QString fileName=QFileDialog::getOpenFileName(this,("选择自定义背景图片"),QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first(),u8"图片文件(*jpg *png)");
    if(!fileName.isEmpty())
    {
        QImage testImage(fileName);
        if(!testImage.isNull()){
            QSettings mysettings("./BJUT_Cloud_Music.ini",QSettings::IniFormat);
            mysettings.setIniCodec("UTF8");
            mysettings.setValue("background/image-url",fileName);
            setStyleSheet(QString("QWidget#Widget{"
                                  "border-radius:10px;"
                                  "border-image: url(%1);}").arg(fileName));    
        }
    }
}



void MainWidget::on_localMusicWidget_customContextMenuRequested(const QPoint &pos)
{
    if(ui->localMusicWidget->itemAt(pos)==Q_NULLPTR)
    {
        return;
    }
    menu_locallist->exec(QCursor::pos());
}

void MainWidget::on_favorMusicWidget_customContextMenuRequested(const QPoint &pos)
{
    if(ui->favorMusicWidget->itemAt(pos)==Q_NULLPTR)
    {
        return;
    }
    menu_favorlist->exec(QCursor::pos());
}

void MainWidget::on_nameListWidget_customContextMenuRequested(const QPoint &pos)
{
    if(ui->nameListWidget->itemAt(pos)==Q_NULLPTR)
    {
        return;
    }
    menu_namelist->exec(QCursor::pos());
}

void MainWidget::on_searchWidget_customContextMenuRequested(const QPoint &pos)
{
    if(ui->searchWidget->itemAt(pos)==Q_NULLPTR)
    {
        return;
    }
    menu_searchlist->exec(QCursor::pos());
}


QString formatTime(qint64 timeMilliSeconds)     //进行（歌曲播放）时间格式转换
{
    qint64 seconds = timeMilliSeconds / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}

void MainWidget::updatePosition(qint64 position)        //更新歌曲播放进度条设置
{
    ui->positionSlider->setValue(static_cast<int>(position));
    ui->positionLabel->setText(formatTime(position)+"/"+formatTime(player->duration()));    //player->duration()为歌的总时长
    if(playlist->currentIndex()>=0)ui->lyricWidget->show(position);
}

void MainWidget::updateDuration(qint64 duration)
{
    ui->positionSlider->setRange(0, static_cast<int>(duration));    //根据歌曲时间调整进度条时间范围
    ui->positionSlider->setEnabled(static_cast<int>(duration) > 0);
    if(!(static_cast<int>(duration) > 0)) { //设置一些默认的显示
        //无音乐播放时，界面元素
        ui->infoLabel->setText("当前暂未播放歌曲...");   //系统初始显示播放title
        mySystemTray->setToolTip(u8"BJUT Cloud Music");
        QImage image(":/image/image/image/non-music.png");      //设置一个无音乐播放的图标
        ui->coverLabel->setPixmap(QPixmap::fromImage(image));
        ui->musicTitleLabel->setText("BJUT Cloud Music");       //TitleLabel
        ui->musicAlbumLabel->setText("");
        ui->musicAuthorLabel->setText("");
        ui->lyricWidget->clear();
    }
    ui->positionSlider->setPageStep(static_cast<int>(duration) / 10);       //设置进度条步长
}

void MainWidget::setPosition(int position)
{
//    qDebug()<<"player->position():"<<player->position()<<"psoition:"<<position;
    if (qAbs(player->position() - position) > 99)
        player->setPosition(position);          //保持进度一致
}

void MainWidget::updateInfo()           //更新当前播放歌曲信息
{
    if (player->isMetaDataAvailable()) {
        //返回可用MP3元数据列表（调试时可以查看）
        QStringList listInfo_debug=player->availableMetaData();
        //歌曲信息
        QString info="";
        QString author = player->metaData(QStringLiteral("Author")).toStringList().join(",");   //使用metaData获取歌曲信息数据
        info.append(author);
        QString title = player->metaData(QStringLiteral("Title")).toString();
        QString albumTitle = player->metaData(QStringLiteral("AlbumTitle")).toString();
        info.append(" - "+title);
        info.append(" ["+formatTime(player->duration())+"]");       //歌曲时长
        ui->infoLabel->setText(info);           //将获取完的歌曲信息进行展示
        mySystemTray->setToolTip("正在播放："+info);
        //封面图片（应获取"ThumbnailImage" From: https://www.zhihu.com/question/36859497）
        QImage picImage= player->metaData(QStringLiteral("ThumbnailImage")).value<QImage>();
        if(picImage.isNull()) picImage=QImage(":/image/image/image/non-music.png");
        ui->coverLabel->setPixmap(QPixmap::fromImage(picImage));
        ui->coverLabel->setScaledContents(true);
        //改变正在播放歌曲的图标
        for(int i=0;i<playlist->mediaCount();i++){
            QListWidgetItem *p=ui->playListWidget->item(i);
            p->setIcon(ui->playListWidget->getIcon());
        }
        int index=playlist->currentIndex();         //在播放列表中设置当前播放的图标
        QListWidgetItem *p=ui->playListWidget->item(index);
        p->setIcon(QIcon(":/image/image/image/music-playing.png"));
        
        //歌词界面显示的信息
        ui->musicTitleLabel->setText(title);
        ui->musicAlbumLabel->setText(u8"专辑："+albumTitle);
        ui->musicAuthorLabel->setText(u8"歌手："+author);
        //解析歌词
        ui->lyricWidget->process(ui->playListWidget->musicList.music[index].getLyricFile());
    }
}

void MainWidget::updatePlayBtn()        //播放状态改变  根据状态设置按钮样式
{
    if(player->state()==QMediaPlayer::PlayingState)     //当播放状态为正在播放时
    {
        ui->btnPlay->setStyleSheet(PlayStyle());
        action_systemTray_play->setIcon(QIcon(":/image/image/image/pause2.png"));
        action_systemTray_play->setText(u8"暂停");
    }else{
        ui->btnPlay->setStyleSheet(PaseStyle());
        action_systemTray_play->setIcon(QIcon(":/image/image/image/play2.png"));
        action_systemTray_play->setText(u8"播放");
    }
}

void MainWidget::systemTrayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        //显/隐主界面
        if(isHidden()){
            show();
        }else{
            hide();
        }
        break;
    default:
        break;
    }
}

void MainWidget::quitMusicPlayer()      //退出
{
    //退出应用
    QCoreApplication::quit();
}

void MainWidget::init_systemTrayIcon()      //初始化应用底部的系统托盘
{
    mySystemTray=new QSystemTrayIcon(this);
    mySystemTray->setIcon(QIcon(":/image/image/image/systemTrayIcon.png"));
    mySystemTray->setToolTip(u8"BJUT Cloud Music");
    connect(mySystemTray,&QSystemTrayIcon::activated,this,&MainWidget::systemTrayIcon_activated);
    //添加菜单项
    QAction *action_systemTray_pre = new QAction(QIcon(":/image/image/image/pre2.png"), u8"上一首");
    connect(action_systemTray_pre, &QAction::triggered, this, &MainWidget::on_btnPre_clicked);
    action_systemTray_play = new QAction(QIcon(":/image/image/image/play2.png"), u8"播放");
    connect(action_systemTray_play, &QAction::triggered, this, &MainWidget::on_btnPlay_clicked);
    QAction *action_systemTray_next = new QAction(QIcon(":/image/image/image/next2.png"), u8"下一首");
    connect(action_systemTray_next, &QAction::triggered, this, &MainWidget::on_btnNext_clicked);
    action_systemTray_playmode = new QAction(QIcon(":/image/image/image/loop2.png"), u8"循环播放");
    connect(action_systemTray_playmode, &QAction::triggered, this, &MainWidget::on_btnPlayMode_clicked);
    QAction *action_systemTray_quit = new QAction(QIcon(":/image/image/image/exit.png"), u8"退出应用");
    connect(action_systemTray_quit, &QAction::triggered, this, &MainWidget::quitMusicPlayer);
    
    QMenu *pContextMenu = new QMenu(this);
    pContextMenu->addAction(action_systemTray_pre);
    pContextMenu->addAction(action_systemTray_play);
    pContextMenu->addAction(action_systemTray_next);
    pContextMenu->addAction(action_systemTray_playmode);
    pContextMenu->addAction(action_systemTray_quit);
    mySystemTray->setContextMenu(pContextMenu);
    mySystemTray->show();
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
    //实现点击界面中某点，音量条隐藏
    int x=event->pos().x();
    int y=event->pos().y();
    int x_widget=ui->volumeSlider->geometry().x(); //注：这里“wiget”的名字是要从UI文件编译后生成的ui_xxx.h文件中得知（在UI布局中看不到）
    int y_widget=ui->volumeSlider->geometry().y();
    int w=ui->volumeSlider->geometry().width();
    int h=ui->volumeSlider->geometry().height();
    if(!(x>=x_widget&&x<=x_widget+w && y>=y_widget&&y<=y_widget+h)){        //判断鼠标 点击是否在音量条之外
        ui->volumeSlider->hide();       //音量条隐藏
    }
    
    //记录窗口移动的初始位置
    offset = event->globalPos() - pos();
    event->accept();
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    int x=event->pos().x();
    int y=event->pos().y();
    //注：这里“layoutWidget1”的名字是要从UI文件编译后生成的ui_xxx.h文件中得知（在UI布局中看不到）
    if((y<ui->titleLabel->geometry().height())&&(x<ui->layoutWidget1->geometry().x())){     //这里设置了主界面的title
        move(event->globalPos() - offset);
        event->accept();    
        setCursor(Qt::ClosedHandCursor);
    }
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
    offset = QPoint();
    event->accept();
    setCursor(Qt::ArrowCursor);
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    //最小化到托盘
    if(!mySystemTray->isVisible()){
        mySystemTray->show();
    }
    hide();        //界面隐藏
    event->ignore();
}

void MainWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWidget::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    ui->localMusicWidget->musicList.addMusic(urls);
    ui->localMusicWidget->refresh();
    ui->stackedWidget->setCurrentIndex(1);//切换到本地音乐
    
}


void MainWidget::on_btnQuit_clicked()
{
//    close();
    QMessageBox::StandardButton result=QMessageBox::question(this, "确认", "确定要退出工大云音乐吗？",
                          QMessageBox::Yes|QMessageBox::No |QMessageBox::Cancel,
                          QMessageBox::No);

        if (result==QMessageBox::Yes){      //其他情况代表不做响应
            exit(0);            //在主界面关闭窗口代表整个程序的退出
        }
}

void MainWidget::on_btnPlay_clicked()
{
    if(player->state()==QMediaPlayer::PlayingState)
     {
        player->pause();

     }
    else if(player->state()==QMediaPlayer::PausedState){

        player->play();
    }
    else if(!playlist->isEmpty() && (player->state()==QMediaPlayer::StoppedState))
    {
        playlist->setCurrentIndex(0);
        player->play();
    }
}

void MainWidget::on_btnNext_clicked()
{
    playlist->next();
}

void MainWidget::on_btnPre_clicked()
{
    playlist->previous();
}

void MainWidget::on_btnPlayMode_clicked()
{
    if(playlist->playbackMode()==QMediaPlaylist::Loop){
        ui->btnPlayMode->setStyleSheet(RandomStyle());
        ui->btnPlayMode->setToolTip(u8"随机播放");
        action_systemTray_playmode->setIcon(QIcon(":/image/image/image/random2.png"));
        action_systemTray_playmode->setText(u8"随机播放");
        playlist->setPlaybackMode(QMediaPlaylist::Random);
    }
    else if(playlist->playbackMode()==QMediaPlaylist::Random){
        ui->btnPlayMode->setStyleSheet(LoopOneStyle());
        ui->btnPlayMode->setToolTip(u8"单曲循环");
        action_systemTray_playmode->setIcon(QIcon(":/image/image/image/loop-one2.png"));
        action_systemTray_playmode->setText(u8"单曲循环");
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }
    else if(playlist->playbackMode()==QMediaPlaylist::CurrentItemInLoop){
        ui->btnPlayMode->setStyleSheet(LoopStyle());
        ui->btnPlayMode->setToolTip(u8"顺序播放");
        action_systemTray_playmode->setIcon(QIcon(":/image/image/image/loop2.png"));
        action_systemTray_playmode->setText(u8"顺序播放");
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }
}

void MainWidget::on_btnMin_clicked()
{
    showMinimized();//窗口最小化
}

void MainWidget::on_btnAdd_clicked()
{    
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setWindowTitle(tr("添加本地音乐（注：按下\"Ctrl+A\"全选添加即可；不支持添加文件夹）"));
    QStringList list;list<<"application/octet-stream";
    fileDialog.setMimeTypeFilters(list);
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted){
       QList<QUrl> urls=fileDialog.selectedUrls();
       qDebug()<<"URLS:"<<urls;
       ui->localMusicWidget->musicList.addMusic(urls);      //从文件夹自动添加音乐到列表
       ui->localMusicWidget->refresh();
       ui->stackedWidget->setCurrentIndex(1);//切换到本地音乐
    }
}


void MainWidget::on_btnVolume_clicked()     //音量按键
{
    if(ui->volumeSlider->isHidden()){
        ui->volumeSlider->show();
    }else{
        ui->volumeSlider->hide();
    }
}

void MainWidget::on_volumeSlider_valueChanged(int value)
{
    player->setVolume(value);
}

void MainWidget::on_btnAddMusicList_clicked()       //新建歌单按钮响应
{
    bool ok;
    QString text=QInputDialog::getText(this,u8"新建歌单",u8"请输入新歌单的名字：",QLineEdit::Normal,"",&ok);
    if(ok && !text.isEmpty()){
        MusicList tempMusic;
        tempMusic.setName(text);
        musiclist.push_back(tempMusic);
        namelist_refresh();
    }
}


void MainWidget::on_btnCurMusic_clicked()
{
    //切换到“当前播放”界面
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWidget::on_btnLocalMusic_clicked()
{
    //切换到“本地音乐”界面
    ui->stackedWidget->setCurrentIndex(1);
}
void MainWidget::on_btnFavorMusic_clicked()
{
    //切换到“我喜欢”界面
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWidget::on_playListWidget_doubleClicked(const QModelIndex &index)  //播放列表双击进行播放
{
    int i=index.row();
    playlist->setCurrentIndex(i);
    player->play();
}
void MainWidget::on_localMusicWidget_doubleClicked(const QModelIndex &index)
{
    playlist->clear();  //将当前播放列表进行清空
    ui->localMusicWidget->musicList.addToPlayList(playlist);
    ui->playListWidget->setMusicList_playing(ui->localMusicWidget->musicList);
    int i=index.row();
    playlist->setCurrentIndex(i);
    player->play();
    ui->stackedWidget->setCurrentIndex(0);//跳转到当前播放列表
}
void MainWidget::on_favorMusicWidget_doubleClicked(const QModelIndex &index)
{
    playlist->clear();
    ui->favorMusicWidget->musicList.addToPlayList(playlist);
    ui->playListWidget->setMusicList_playing(ui->favorMusicWidget->musicList);
    int i=index.row();
    playlist->setCurrentIndex(i);
    player->play();
    ui->stackedWidget->setCurrentIndex(0);//跳转到当前播放列表
}

void MainWidget::on_nameListWidget_doubleClicked(const QModelIndex &index)
{
    ui->stackedWidget->setCurrentIndex(3);//跳转到歌单内容列表
    musiclist_index=index.row();
    musicListWidget_refresh();
}

void MainWidget::on_btnSkin_clicked()
{
    menu_changeSkin->exec(QCursor::pos());
}

void MainWidget::on_btnAddtoMusicList_clicked()         //添加本地中的歌曲到自己的歌单中
{
    MusicListDialog *dialog=new MusicListDialog(this);
    int num=ui->localMusicWidget->count();
    bool *results=new bool[num];
    dialog->setMusicList(ui->localMusicWidget->musicList,results);
    if(dialog->exec()==QDialog::Accepted){
        for(int i=0;i<num;i++){
            if(results[i]){
                musiclist[musiclist_index].addMusic(ui->localMusicWidget->musicList.getMusic(i));
            }
        }
        musicListWidget_refresh();      //在此处进行更新显示
    }
    delete []results;
}

void MainWidget::on_musicListWidget_doubleClicked(const QModelIndex &index)
{
    playlist->clear();
    musiclist[musiclist_index].addToPlayList(playlist);
    ui->playListWidget->setMusicList_playing(musiclist[musiclist_index]);   //把整个列表的歌曲都放置在当前播放列表中
    int i=index.row();
    playlist->setCurrentIndex(i);
    player->play();
    ui->stackedWidget->setCurrentIndex(0);//跳转到当前播放列表
}

void MainWidget::on_musicListWidget_customContextMenuRequested(const QPoint &pos)
{
    if(ui->musicListWidget->itemAt(pos)==Q_NULLPTR)
    {
        return;
    }
    menu_musiclist->exec(QCursor::pos());
}

void MainWidget::on_btnAddtoFavor_clicked()
{
    MusicListDialog *dialog=new MusicListDialog(this);
    int num=ui->localMusicWidget->count();
    bool *results=new bool[num];
    dialog->setMusicList(ui->localMusicWidget->musicList,results);
    if(dialog->exec()==QDialog::Accepted){
        for(int i=0;i<num;i++){
            if(results[i]){
                ui->favorMusicWidget->musicList.addMusic(ui->localMusicWidget->musicList.getMusic(i));
            }
        }
        ui->favorMusicWidget->refresh();
    }
    delete []results;
}

void MainWidget::on_btnSortSinger_clicked()     //按歌手名排序
{
    ui->localMusicWidget->musicList.sort_by(AUTHOR);
    ui->localMusicWidget->refresh();
}
void MainWidget::on_btnSortTitle_clicked()      //按歌名排序
{
    ui->localMusicWidget->musicList.sort_by(TITLE);
    ui->localMusicWidget->refresh();
}
void MainWidget::on_btnSortDuration_clicked()   //按播放时间排序
{
    ui->localMusicWidget->musicList.sort_by(DURATION);
    ui->localMusicWidget->refresh();
}

void MainWidget::on_btnSortSinger_2_clicked()       //我喜欢歌单列表中的按歌名排序
{
    ui->favorMusicWidget->musicList.sort_by(AUTHOR);
    ui->favorMusicWidget->refresh();
}
void MainWidget::on_btnSortTitle_2_clicked()
{
    ui->favorMusicWidget->musicList.sort_by(TITLE);
    ui->favorMusicWidget->refresh();
}
void MainWidget::on_btnSortDuration_2_clicked()
{
    ui->favorMusicWidget->musicList.sort_by(DURATION);
    ui->favorMusicWidget->refresh();
}

void MainWidget::on_btnSortSinger_4_clicked()
{
    musiclist[musiclist_index].sort_by(AUTHOR);
    musicListWidget_refresh();
}
void MainWidget::on_btnSortTitle_4_clicked()
{
    musiclist[musiclist_index].sort_by(TITLE);
    musicListWidget_refresh();
}
void MainWidget::on_btnSortDuration_4_clicked()
{
    musiclist[musiclist_index].sort_by(DURATION);
    musicListWidget_refresh();
}

void MainWidget::on_btnNeaten_clicked() //整理歌单：将歌单中的重复歌曲去掉并排序
{
    ui->localMusicWidget->musicList.neaten();
    ui->localMusicWidget->refresh();
}
void MainWidget::on_btnNeaten_2_clicked()
{
    ui->favorMusicWidget->musicList.neaten();
    ui->favorMusicWidget->refresh();
}
void MainWidget::on_btnNeaten_3_clicked()
{
    musiclist[musiclist_index].neaten();
    musicListWidget_refresh();
}

void MainWidget::on_btnTitle_clicked()
{
    on_btnAbout_clicked();
}

//void MainWidget::on_btnLyric_clicked()
//{
//    ui->stackedWidget->setCurrentIndex(4);
//}

void MainWidget::on_btnClear_clicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "提示", "此操作不可逆！\n确实要清空吗?", QMessageBox::Yes|QMessageBox::No);
    if (btn == QMessageBox::Yes) {
        ui->playListWidget->musicList.clear();
        ui->playListWidget->refresh();
        playlist->clear();
    }
}

void MainWidget::on_btnClear_2_clicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "提示", "此操作不可逆！\n确实要清空吗?", QMessageBox::Yes|QMessageBox::No);
    if (btn == QMessageBox::Yes) {
        ui->localMusicWidget->musicList.clear();
        ui->localMusicWidget->refresh();
    }
}

void MainWidget::on_btnClear_3_clicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "提示", "此操作不可逆！\n确实要清空吗?", QMessageBox::Yes|QMessageBox::No);
    if (btn == QMessageBox::Yes) {
        ui->favorMusicWidget->musicList.clear();
        ui->favorMusicWidget->refresh();
    }
}

void MainWidget::on_btnClear_4_clicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "提示", "此操作不可逆！\n确实要清空吗?", QMessageBox::Yes|QMessageBox::No);
    if (btn == QMessageBox::Yes) {
        musiclist[musiclist_index].clear();
        musicListWidget_refresh();
    }
}

void MainWidget::on_btnAbout_clicked()
{
    QMessageBox::about(this,u8"关于","BJUT Cloud Music\n"
                                   "【歌词文件说明】需要与对应的歌曲MP3在同目录且同名（.lry文件）\n"
                                   "【快捷键说明】\n"
                                   "播放/暂停  -  空格\n"
                                   "上一曲/下一曲  -  Alt键+方向键←/→\n"
                                   "【添加本地音乐】可直接拖拽至软件界面内或者点击本地音乐界面的添加按钮（Ctrl键+O）\n"
                                   "【音乐文件类型】添加过程中会自动过滤得到可播放的文件类型（.mp3/.flac/.mpga文件），所以添加时无需考虑文件类型，使用\"Ctrl+A\"选择文件夹内全部文件添加即可\n"
                                   "\n注：鼠标移动到不认识的按钮上，会有说明哦~\n");
}

void MainWidget::on_musicsraech_clicked(){
    //this->hide();         //背景主界面不应该被藏起来
//    dialog1.transfer(tcpSocket,userID);         //进行参数传递
//    dialog1.show();         //展示搜索框
//    dialog1.exec();            //搜索框退出
    //this->show();
    ui->stackedWidget->setCurrentIndex(4);          //设置widget在第4个page


    //首先建立一个music类
    Music testMusic;
    testMusic.url="abcd";
    testMusic.title="for test";
    testMusic.author="xwh";
    testMusic.duration=120;
    testMusic.albumTitle="test case";
    testMusic.audioBitRate=32000;
    ui->searchWidget->musicList.addMusic(testMusic);        //添加歌曲
//    ui->searchWidget->musicList.addMusic(musiclist[musiclist_index].getMusic(pos));
    ui->searchWidget->refresh();

    QString strText = ui->SearchlineEdit->text();
    //若在数据库中搜寻到某个歌曲 则显将结果显示在搜索弹框中
    if (!strText.isEmpty())             //此处应该修改为当搜索不到歌曲时  则显示“无当前搜索歌曲”（这得结合数据库去实现）
    {
        chuanshu *ss=new chuanshu("0######0#");
        ss->type = 17;
        ss->info = ui->SearchlineEdit->text();
        ss->timer = "";
        ss->name = userID;
        ss->fileName = "";
        ss->wantsendto = "";
        ss->size = 0;
        ss->ip = "";

        QString sender="";
        sender+=QString::number(ss->type)+"#"+(QString)ss->info+"#"+(QString)ss->timer+"#"+(QString)ss->name+"#"+(QString)ss->fileName+"#"+(QString)ss->wantsendto+"#"+QString::number(ss->size)+"#"+(QString)ss->ip;

//        // 发送
        char la=0xff;
        qDebug() <<sender.toUtf8();
        tcpSocket->write(sender.toUtf8()+la);
        tcpSocket->flush();
//        qDebug() <<"search send";


        QMessageBox::information(this, QStringLiteral("搜索"), QStringLiteral("搜索内容为%1").arg(strText));
    }



    //在此处调用widget添加搜索歌曲
}

void MainWidget::on_btnMusiclist_clicked(){
    btnMusiclist_index++;       //每次按钮按下次数加1
    if(btnMusiclist_index%2==1){    //代表奇数次按下  则显示歌单
        ui->nameListWidget->show();         //歌单展示
    }else ui->nameListWidget->hide();         //歌单隐藏

}

int MainWidget::doExec()
{
    this->result = Rejected;
    loop = new QEventLoop();
    loop->exec();
    return result;
}

void MainWidget::on_btnPersonal_clicked(){      //此处主界面不隐藏起来
    extern int quit_login;
    quit_login=0;           //退出状态重置
    personal= new PersonalDialog();

    personal->show();
    personal->exec();

    if(quit_login==1){
        qDebug()<<"quit"<<endl;
        close();
        this->hide();
        this->~MainWidget();        //尝试进行销毁

        loop->exit();
        loop->deleteLater();
    }
}

void MainWidget::on_dianzan_clicked(){      //同时也需要在数据库中进行记录
    dianzan_num++;      //点赞次数加1
    if(dianzan_num%2==1){       //效果
        ui->dianzan->setStyleSheet("QPushButton{image: url(:/image/image/image/dianzan1.png);border:none;color:rgb(255, 255, 255);}");
    }else{
        ui->dianzan->setStyleSheet("QPushButton{image: url(:/image/image/image/dianzan.png);border:none;color:rgb(255, 255, 255);}");
    }
}


void MainWidget::on_commment_clicked(){
    commentDialog.show();           //展示评论界面
    commentDialog.exec();           //评论界面退出
}

//void MainWidget::on_login_clicked1(){
//    qDebug()<<"真正登录";
//    login1.show();
//    login1.exec();

//}
void MainWidget::testlogin(){
    test.show();
    test.exec();
}

void MainWidget::socket_Read_Data()
{
    qDebug()<<"mainwidget";
    QByteArray buffer;
    //读取缓冲区数据
    buffer = tcpSocket->readAll();

    if(!buffer.isEmpty())
    {
        str=QString::fromLocal8Bit(buffer.data());//将收到的utf-8格式转换回String
        qDebug()<<"rec:"<<str;
//        ui->id->clear();
//        ui->id->setText(str);
    }

    QStringList sstr=str.split("#");
    qDebug()<<"type:"<<sstr[0].toInt();


    if(sstr[0].toInt()==3){        //判断回传包的类型
        qDebug()<<"s1";
        QStringList arguments;//用于传参数
        QString program = "D:\\Transfer\\receive.exe"; //外部程序地址
        arguments <<"10.24.10.40"<<"8888";
        qDebug()<<"s2";
        QProcess process(this);
        process.startDetached(program, arguments);//启动程序
        process.close();
        qDebug()<<"s3";
    }
}

