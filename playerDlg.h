
// playerDlg.h: 头文件
//

#pragma once
#include"VLCplayer.h"

// CplayerDlg 对话框
class CplayerDlg : public CDialogEx
{
// 构造
public:
	CplayerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLAYER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	VLCplayer m_VLCplayer;
	CMenu m_Menu;
	CSliderCtrl m_slider;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonPlay();              //播放
	afx_msg void OnBnClickedPause();                   //暂停
	afx_msg void OnBnClickedForwardButton();           //快进
	afx_msg void OnBnClickedBackwardButton();          //快退
	afx_msg void OnBnClickedCancelButton();            //停止
	afx_msg void OnCliickedOpenfileButton();           //菜单打开文件
	afx_msg void OnBnClickedExitButton();              //退出
	//进度条改变
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//播放进度设置
	afx_msg LRESULT OnPosChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);   //尺寸自适应
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);   //设置窗口最小值
	afx_msg void OnBnClickedPrevious();                //播放历史视频
	afx_msg void OnABOUT();                            //菜单关于->帮助窗口
	afx_msg void OnBnClickedSingle();                  //单帧播放
	virtual BOOL PreTranslateMessage(MSG* pMsg);       //重载以捕获键盘消息
};
