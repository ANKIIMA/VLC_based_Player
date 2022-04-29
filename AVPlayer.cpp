#include "pch.h"
#include "AVPlayer.h"
#include <cmath>

#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")

#include<vlc.h>


// VLC的事件管理
void OnVLC_EndReached(const libvlc_event_t *event, void *data);
void OnVLC_PositionChanged(const libvlc_event_t *event, void *data);


CAVPlayer::CAVPlayer(void) :
m_pVLC_Inst(NULL),
m_pVLC_Player(NULL),
m_hWnd(NULL),
m_pfn(NULL)
{
}

CAVPlayer::~CAVPlayer(void)
{
    Release();
}

void CAVPlayer::Init()
{
    if (! m_pVLC_Inst)
    {
        m_pVLC_Inst = libvlc_new(0, NULL);
    }
}

void CAVPlayer::Release()
{
    Stop();

    if (m_pVLC_Inst)
    {
        libvlc_release (m_pVLC_Inst);
        m_pVLC_Inst   = NULL;
    }
}

bool CAVPlayer::Play(const std::string &strPath)
{
    if (! m_pVLC_Inst)
    {
        Init();
    }

    if(strPath.empty() || ! m_pVLC_Inst)
    {
        return false;
    }

    Stop();

    bool bRet = false;
    libvlc_media_t *m;

    if (m = libvlc_media_new_path(m_pVLC_Inst, strPath.c_str()))
    {
        if (m_pVLC_Player = libvlc_media_player_new_from_media(m))
        {
            libvlc_media_player_set_hwnd(m_pVLC_Player, m_hWnd);
            libvlc_media_player_play(m_pVLC_Player);

            // 事件管理
            libvlc_event_manager_t *vlc_evt_man = libvlc_media_player_event_manager(m_pVLC_Player);
            libvlc_event_attach(vlc_evt_man, libvlc_MediaPlayerEndReached, ::OnVLC_EndReached, this);
            libvlc_event_attach(vlc_evt_man, libvlc_MediaPlayerPositionChanged, ::OnVLC_PositionChanged, this);
            bRet = true;
        }
        libvlc_media_release(m);
    }
    return bRet;
}

void CAVPlayer::Stop()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_stop (m_pVLC_Player);      /* Stop playing */    
        libvlc_media_player_release (m_pVLC_Player);   /* Free the media_player */
        m_pVLC_Player = NULL;
    }

    /* Stop the media */
    //if (libvlc_media_player_get_state(vlcPlayer) == libvlc_Ended)
}

void CAVPlayer::Play()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_play(m_pVLC_Player);
    }
}

void CAVPlayer::Pause()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_pause(m_pVLC_Player);
    }
}

void CAVPlayer::Volume(int nVol)
{
    if (m_pVLC_Player)
    {
        libvlc_audio_set_volume(m_pVLC_Player,nVol);
    }
}

void CAVPlayer::VolumeIncrease()
{
    if (m_pVLC_Player)
    {
        int nVol = libvlc_audio_get_volume(m_pVLC_Player);
        Volume((int)ceil(nVol * 1.1));
    }
}

void CAVPlayer::VolumeReduce()
{
    if (m_pVLC_Player)
    {
        int nVol = libvlc_audio_get_volume(m_pVLC_Player);
        Volume((int)floor(nVol * 0.9));
    }
}

int CAVPlayer::GetPos()
{   
    if (m_pVLC_Player)
    {
        return (int)(100 * libvlc_media_player_get_position(m_pVLC_Player));
    }

    return 0;
}

void CAVPlayer::SetPos(int Pos)
{
	if (m_pVLC_Player)
	{
		libvlc_media_player_set_position(m_pVLC_Player,Pos);
	}

}


void CAVPlayer::SeekTo(int nPos)
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_set_position(m_pVLC_Player, nPos/(float)100.0);
    }
}

void CAVPlayer::SeekForward()
{
    int nPos = GetPos();
    //SeekTo(ceil(nPos * 1.1));
    SeekTo(nPos + 10);
}

void CAVPlayer::SeekBackward()
{
    int nPos = GetPos();
    //SeekTo(floor(nPos * 0.9));
    SeekTo(nPos - 10);
}

void CAVPlayer::SetHWND( HWND hwnd )
{
    if (::IsWindow(hwnd))
    {
        m_hWnd = hwnd;
    }
}

HWND CAVPlayer::GetHWND()
{
    return m_hWnd;
}

void CAVPlayer::SetCallback( pfnPosChanged pfn )
{
    m_pfn = pfn;
}

pfnPosChanged CAVPlayer::GetCallback()
{
    return m_pfn;
}

BOOL CAVPlayer::IsOpen()
{
    return NULL != m_pVLC_Player;
}

BOOL CAVPlayer::IsPlaying()
{
    if (m_pVLC_Player)
    {
        return libvlc_media_player_is_playing(m_pVLC_Player);
    }

    return FALSE;
}

__int64 CAVPlayer::GetTime()
{
    if (m_pVLC_Player)
    {
        return libvlc_media_player_get_time(m_pVLC_Player);
    }

    return 0;
}

__int64 CAVPlayer::GetLength()
{
	if (m_pVLC_Player)
	{
		return libvlc_media_player_get_length(m_pVLC_Player);
	}

	return 1;
}

void OnVLC_EndReached( const libvlc_event_t *event, void *data )
{

}

void OnVLC_PositionChanged( const libvlc_event_t *event, void *data )
{
    switch (event->type)
    {
    case libvlc_MediaPlayerPositionChanged:
        {
            float     fPos       = event->u.media_player_position_changed.new_position;
            CAVPlayer *pAVPlayer = (CAVPlayer *) data;

            if (pAVPlayer)
            {
                pfnPosChanged pfn = pAVPlayer->GetCallback();

                if (pfn)
                {
                    pfn(pAVPlayer, int(fPos * 100));
                }                
            }
        }

        break;
    case libvlc_MediaPlayerSnapshotTaken:
		break;
    default:
        break;
    }
}
