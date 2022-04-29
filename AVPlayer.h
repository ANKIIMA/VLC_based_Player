// ----------------------------------------------------------------------------------------------------------------------
// Build on December/01/2018 by zhangyongxin
// Email:   iszhangyongxin@163.com  
// ----------------------------------------------------------------------------------------------------------------------

#ifndef __AVPlayer_H__
#define __AVPlayer_H__
//ifndef/define/endif  ��ҪĿ���Ƿ�ֹAVPlayer.h���ͷ�ļ����ظ������ͱ���
#include <iostream>

typedef void (* pfnPosChanged) (void *data, int iPos);  // �ļ�λ�ñ仯
struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_t;

class CAVPlayer
{
public:
    CAVPlayer(void);
    ~CAVPlayer(void);

    bool Play(const std::string &strPath);  // ����·��ΪstrPath���ļ�
    void Play ();                           // ����
    void Pause();                           // ��ͣ
    void Stop ();                           // ֹͣ

    void Volume(int nVol);                  // ��������ΪnVol
    void VolumeIncrease();                  // ��������
    void VolumeReduce();                    // ������С 

    void SeekTo(int nPos);                  // ����ָ��λ��nPos
    void SeekForward();                     // ���
    void SeekBackward();                    // ����

    void SetHWND(HWND hwnd);                // ������Ƶ��ʾ�Ĵ��ھ��
    HWND GetHWND();                         // ��ȡ��Ƶ��ʾ�Ĵ��ھ��

    void SetCallback(pfnPosChanged pfn);    // �����ļ�λ�øı�ʱ�Ļص�����
    pfnPosChanged GetCallback();            // ��ȡ�ļ�λ�øı�ʱ�Ļص�����

    BOOL    IsOpen();                       // �ļ��Ƿ��
    BOOL    IsPlaying();                    // �ļ��Ƿ����ڲ���
    int     GetPos();                       // ��ȡ�ļ���ǰ���ŵ�λ��
	void    SetPos(int Pos);                // �����ļ�����λ��
    __int64 GetTime();                      // ��ȡʱ��
	__int64 GetLength();

private:
    libvlc_instance_t       *m_pVLC_Inst;   // VLCʵ��
    libvlc_media_player_t   *m_pVLC_Player; // VLC������
    HWND                    m_hWnd;         // ��Ƶ��ʾ�Ĵ��ھ��
    pfnPosChanged           m_pfn;          // �ļ�λ�øı�ʱ�Ļص�����

    void Init();                            // ��ʼ��
    void Release();                         // �����ڴ�
};

#endif
