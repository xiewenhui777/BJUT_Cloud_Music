#include "MusicListWidget.h"

MusicListWidget::MusicListWidget(QWidget *parent)
    :QListWidget (parent)
{
    
}

void MusicListWidget::refresh()
{
    clear();//先清空
    musicList.addToListWidget(this);   //增加歌曲到列表    同时传参 this代表当前的MusicListWidget列表
}

void MusicListWidget::setMusicList(const MusicList &music)
{
    musicList=music;
    refresh();
}

void MusicListWidget::setMusicList_playing(const MusicList &music)
{
    musicList=music;
    musicList.setSQL(false);
    refresh();
}

void MusicListWidget::removeMusic()
{
    int pos=currentRow();    
    //移除
    musicList.removeMusic(pos);
    
    //从ListWidget中移除
    QListWidgetItem *tempItem=takeItem(pos);
    removeItemWidget(tempItem);
    delete tempItem;
}

void MusicListWidget::showInExplorer()
{
    int pos=currentRow();       //在MusicListWidget中选中歌曲
    musicList.showInExplorer(pos);
}

void MusicListWidget::detail()
{
    int pos=currentRow();//在MusicListWidget中选中歌曲
    musicList.detail(pos);
}
