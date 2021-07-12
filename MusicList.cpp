#include "MusicList.h"
#include <QCoreApplication>
#include <QProgressDialog>
#include <QDesktopServices>
#include <QtSql>
#include <algorithm>
#include "MusicListWidget.h"

MusicList::MusicList(const QList<QUrl> &urls, QString iname)
{
    addMusic(urls);
    setName(iname);
}

void MusicList::addMusic(const QList<QUrl> &urls)
{
    //实测这里耗时较长，所以添加一个进度显示对话框
    QProgressDialog proDialog(u8"添加进度",u8"取消",0,urls.size());
    proDialog.setMinimumSize(350,150);
    proDialog.setWindowModality(Qt::WindowModal);
    proDialog.setWindowTitle("添加中...请稍后");
    proDialog.show();
    int x=0;
    foreach (QUrl i, urls) {
        x++;
        proDialog.setValue(x);
        //过滤Url的类型
        QMimeDatabase db;
        QMimeType mime = db.mimeTypeForFile(i.toLocalFile());
        if(mime.name()!="audio/mpeg"&&mime.name()!="audio/flac"){
            continue;
        }
        //剩下的符合类型
        music.push_back(Music(i));
        if(sql_flag){
            music[music.size()-1].insertSQL(name);
        }
        if(proDialog.wasCanceled()) break;
    }
}

void MusicList::addMusic(const Music &iMusic)
{
    music.push_back(iMusic);
    if(sql_flag){
        music[music.size()-1].insertSQL(name);
    }
}

Music MusicList::getMusic(int pos)
{
    return music[pos];
}

void MusicList::addToPlayList(QMediaPlaylist *playlist)
{
    for(auto i=music.begin();i!=music.end();i++){
        playlist->addMedia(i->getUrl());
    }
}

void MusicList::addToListWidget(MusicListWidget *listWidget)
{
    foreach(const Music &i,music){      //从music的vecotr数组（是之前通过数据库查询获取的）中一个一个添加
        QListWidgetItem *item = new QListWidgetItem;
        item->setIcon(listWidget->getIcon());
        item->setText(i.getInfo());
        listWidget->addItem(item);      //在MusicListWidget中进行显示
    }
}

void MusicList::removeMusic(int pos)
{
    if(sql_flag){
        remove_SQL_all();
        int i=0;
        for(auto it=music.begin();it!=music.end();){
            if(i==pos){
                it= music.erase(it);
                break;
            }
            else{
                it++;
            }
            i++;
            
        }
        insert_SQL_all();    
    }else{
        int i=0;
        for(auto it=music.begin();it!=music.end();){
            if(i==pos){
                it= music.erase(it);
                break;
            }
            else{
                it++;
            }
            i++;
            
        }
    }
    
}

void MusicList::showInExplorer(int pos)
{
    QString str=music[pos].getUrl().toString();
    str.remove(str.split("/").last());//切割字符串获得所在的文件夹路径
    QDesktopServices::openUrl(str);     //通过URL打开文件夹
}

void MusicList::detail(int pos)
{
    music[pos].detail();    //返回歌曲详细信息
}

void MusicList::remove_SQL_all()
{
    QSqlQuery sql_query;
    QString delete_sql = "delete from MusicInfo where name = ?";
    sql_query.prepare(delete_sql);
    sql_query.addBindValue(name);
    sql_query.exec();
}

void MusicList::insert_SQL_all()
{
    for(auto i=music.begin();i!=music.end();i++){
        i->insertSQL(name);
    }
}

void MusicList::read_fromSQL()
{
    QSqlQuery sql_query;
    QString select_sql = "select url, author, title, duration, albumTitle, audioBitRate from MusicInfo where name = ?";
    sql_query.prepare(select_sql);
    sql_query.addBindValue(name);
    if(sql_query.exec())
    {
        while(sql_query.next())
        {
            Music tempMusic;
            tempMusic.url=QUrl(sql_query.value(0).toString());
            tempMusic.author=sql_query.value(1).toString();
            tempMusic.title=sql_query.value(2).toString();
            tempMusic.duration=sql_query.value(3).toLongLong();
            tempMusic.albumTitle=sql_query.value(4).toString();
            tempMusic.audioBitRate=sql_query.value(5).toInt();
            music.push_back(tempMusic);     //使用Music类型对象的vector数组来储存歌单在数据库中的所有歌曲信息
        }  
    }
}

void MusicList::sort_by(COMPARE key)
{
    sort(music.begin(),music.end(),MusicCompare(key));
    if(sql_flag){
        remove_SQL_all();
        insert_SQL_all();
    }
}

void MusicList::neaten()
{
    sort(music.begin(),music.end(),MusicCompare(DEFAULT));
    music.erase(unique(music.begin(),music.end(),MusicCompare(EQUALITY)), music.end()); //擦除重名的歌曲
    if(sql_flag){
        remove_SQL_all();
        insert_SQL_all();
    }
}

void MusicList::clear()
{
    music.clear();
    if(sql_flag){
        remove_SQL_all();    
    }
}
