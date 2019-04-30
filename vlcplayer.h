#ifndef VLCPLAYER_H
#define VLCPLAYER_H

#include "vlc/vlc.h"
#include <QObject>

class VLCPlayer : public QObject
{
    Q_OBJECT

public:
    explicit VLCPlayer(QObject *parent = nullptr);

    // 播放
    int Play(QString filename,  uint32_t hwnd = 0);

    //去黑边
    void RemoveBlack();

    // 播放
    int Play();

    // 暂停
    void Pause();

    // 停止
    void Stop();

    // 音量设置为nVol
    void SetVolume(int nVol);

    //获取音量值
    int GetVolume();

    // 跳到指定位置nPos
    void SeekTo(float nPos);

    // 文件是否打开
    bool IsOpen();

    // 文件是否正在播放
    bool IsPlaying();

    // 是否暂停
    bool IsPause();

    // 获取文件当前播放的位置
    float GetPos();

    //获取总时长
    int64_t GetTime();

    // 获取播放时间
    int64_t GetPlayTime();

    // 设置播放时间
    void SetPlayTime(int64_t time);

    //获取播放状态
    int GetPlayState();

    //设置音轨
    bool setTrack(int trackIndex);

    //销毁
    void Release();

signals:
    void playAllTime(int64_t sec);
    void playCurrentTime(int64_t sec);

public slots:

private:
        libvlc_instance_t     *m_pVLC_Inst;
        libvlc_media_player_t *m_pVLC_Player;
        libvlc_event_manager_t *m_pVLC_eMg;
};

#endif // VLCPLAYER_H
