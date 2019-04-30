#include "vlcplayer.h"
#include<QFile>
#include<QDebug>

static void handleEvents(const libvlc_event_t *event, void *userData)
{
    VLCPlayer* obj = static_cast<VLCPlayer *>(userData);

    switch (event->type) {
        //播放中
        case libvlc_MediaPlayerPlaying:{
            emit obj->playAllTime(obj->GetTime()/1000);
            break;
        }

        // media player 位置改变
        case libvlc_MediaPlayerPositionChanged: {
            break;
        }

        // media player 时间改变
        case libvlc_MediaPlayerTimeChanged: {
              int64_t time = event->u.media_player_time_changed.new_time;
              emit obj->playCurrentTime(time/1000);
              break;
          }
          default:
              break;
      }
}

VLCPlayer::VLCPlayer(QObject *parent) : QObject(parent)
{
       m_pVLC_Player = nullptr;
       m_pVLC_Inst = libvlc_new(0, nullptr);
}

int VLCPlayer::Play(QString filename, uint32_t hwnd)
{
    if (!QFile::exists(filename)) {
        qDebug()<<"file is not exist:"<<filename;
        return -1 ;
    }

    if (this->IsPlaying()) {
        this->Stop();
    }

    libvlc_media_t *m;
    m = libvlc_media_new_path(m_pVLC_Inst, filename.toStdString().data());
    if (m)
    {
        m_pVLC_Player = libvlc_media_player_new_from_media(m);
        if (m_pVLC_Player)
        {
            m_pVLC_eMg = libvlc_media_player_event_manager(m_pVLC_Player);
            libvlc_event_attach(m_pVLC_eMg, libvlc_MediaPlayerPositionChanged, handleEvents, this);
            libvlc_event_attach(m_pVLC_eMg, libvlc_MediaPlayerTimeChanged, handleEvents, this);
            libvlc_event_attach(m_pVLC_eMg, libvlc_MediaPlayerPlaying, handleEvents, this);
            if (hwnd != 0)
                libvlc_media_player_set_xwindow(m_pVLC_Player, hwnd);
            this->Play();
        }

        libvlc_media_release(m);
    }

    return -1;
}

void VLCPlayer::RemoveBlack()
{
    //可以设置视频的比例
    //    "4:3"
    //    "1:1"
    //    "16:10"
    //    "2.21:1"
    //    "2.35:1"
    //    "2.39:1"
    //    "5:4"
    if(!m_pVLC_Player)
        return;

    libvlc_video_set_aspect_ratio(m_pVLC_Player,"16:10");
}

int VLCPlayer::Play()
{
    if (m_pVLC_Player)
    {
       return  libvlc_media_player_play(m_pVLC_Player);
    }

    return -1;
}

void VLCPlayer::Pause()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_pause(m_pVLC_Player);
    }
}

void VLCPlayer::Stop()
{
    if (m_pVLC_Player)
    {
        if(GetPlayState() != libvlc_Ended){
            libvlc_media_player_pause(m_pVLC_Player);
            libvlc_media_player_stop(m_pVLC_Player);
        }

        libvlc_media_player_release(m_pVLC_Player);
        m_pVLC_Player = nullptr;
    }
}

void VLCPlayer::SetVolume(int nVol)
{
    if (m_pVLC_Player)
    {
        libvlc_audio_set_volume(m_pVLC_Player, nVol);
    }
}

int VLCPlayer::GetVolume()
{
    if (m_pVLC_Player)
    {
        return libvlc_audio_get_volume(m_pVLC_Player);
    }
    return 0;
}

void VLCPlayer::SeekTo(float nPos)
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_set_position(m_pVLC_Player, nPos);
    }
}

bool VLCPlayer::IsOpen()
{
    return nullptr != m_pVLC_Player;
}

bool VLCPlayer::IsPlaying()
{
    if (m_pVLC_Player)
    {
        return libvlc_media_player_is_playing(m_pVLC_Player);
    }

    return false;
}

bool VLCPlayer::IsPause()
{
    bool tem = false;

    if (m_pVLC_Player)
    {
        int state = libvlc_media_player_get_state(m_pVLC_Player);
        switch (state)
        {
            case libvlc_Paused:
            case libvlc_Stopped:
                tem = true;
                break;
        }
    }

    return tem;
}

float VLCPlayer::GetPos()
{
    if (m_pVLC_Player)
    {
        return 100 * libvlc_media_player_get_position(m_pVLC_Player);
    }

    return 0;
}

int64_t VLCPlayer::GetTime()
{
    if (m_pVLC_Player)
    {
        return libvlc_media_player_get_length(m_pVLC_Player);
    }

    return 0;
}

int64_t VLCPlayer::GetPlayTime()
{
    if (m_pVLC_Player)
    {
        return libvlc_media_player_get_time(m_pVLC_Player);
    }

    return 0;
}

void VLCPlayer::SetPlayTime(int64_t time)
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_set_time(m_pVLC_Player, time);
    }
}

int VLCPlayer:: GetPlayState()
{
    if (m_pVLC_Player) {
        return libvlc_media_player_get_state(m_pVLC_Player);
    }
    return -1;
}

bool VLCPlayer::setTrack(int trackIndex)
{
    if(m_pVLC_Player){
       return  libvlc_audio_set_track(m_pVLC_Player,trackIndex)  == 0;
    }
    return false;
}

void VLCPlayer::Release()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_stop(m_pVLC_Player);
        libvlc_media_player_release(m_pVLC_Player);
        m_pVLC_Player = nullptr;

    }
    if (m_pVLC_Inst)
    {
        libvlc_release(m_pVLC_Inst);
        m_pVLC_Inst = nullptr;
    }
}
