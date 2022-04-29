
// playerDlg.cpp: 实现文件
//
#include "pch.h"
#include "framework.h"
#include "player.h"
#include "playerDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include<vlc/vlc.h>
#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")

#define WM_USER_POS_CHANGED WM_USER + 1
std::string strpath = "";
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CplayerDlg 对话框



CplayerDlg::CplayerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLAYER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CplayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER, m_slider);
}

BEGIN_MESSAGE_MAP(CplayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

ON_BN_CLICKED(IDC_PLAY, &CplayerDlg::OnBnClickedButtonPlay)
ON_BN_CLICKED(IDC_PAUSE, &CplayerDlg::OnBnClickedPause)
ON_BN_CLICKED(IDC_FORWARD, &CplayerDlg::OnBnClickedForwardButton)
ON_BN_CLICKED(IDC_BACKWARD, &CplayerDlg::OnBnClickedBackwardButton)
ON_BN_CLICKED(IDC_CANCEL, &CplayerDlg::OnBnClickedCancelButton)
ON_COMMAND(ID_OPEN, &CplayerDlg::OnCliickedOpenfileButton)
ON_BN_CLICKED(IDC_EXIT, &CplayerDlg::OnBnClickedExitButton)
ON_WM_HSCROLL()
ON_MESSAGE(WM_USER_POS_CHANGED,OnPosChange)
ON_WM_SIZE()
ON_WM_GETMINMAXINFO()
ON_BN_CLICKED(IDC_PREVIOUS, &CplayerDlg::OnBnClickedPrevious)
ON_COMMAND(ID_32771, &CplayerDlg::OnABOUT)
ON_BN_CLICKED(IDC_SINGLE, &CplayerDlg::OnBnClickedSingle)
END_MESSAGE_MAP()



void CallbackPosChanged(void* data, int iPos)
{
	VLCplayer* pAVPlayer = (VLCplayer*)data;

	if (pAVPlayer)
	{
		HWND hWnd = pAVPlayer->GetHWND();

		if (::IsWindow(hWnd) && ::IsWindow(::GetParent(hWnd)))
		{
			::PostMessage(::GetParent(hWnd), WM_USER_POS_CHANGED, (WPARAM)data, iPos);
		}
	}
}


/*CStringA UnicodeToUTF8(const CStringW& strWide)
{
	CStringA strUTF8;
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, strWide, -1, NULL, 0, NULL, NULL);

	if (nLen > 1)
	{
		::WideCharToMultiByte(CP_UTF8, 0, strWide, -1, strUTF8.GetBuffer(nLen - 1), nLen, NULL, NULL);
		strUTF8.ReleaseBuffer();
	}

	return strUTF8;
}*/

// CplayerDlg 消息处理程序

BOOL CplayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_VLCplayer.SetHWND(GetDlgItem(IDC_STATIC)->GetSafeHwnd());   // 设置播放器的窗口句柄
	m_VLCplayer.SetCallback(CallbackPosChanged);

	CWnd* pic1 = (CWnd*)GetDlgItem(IDC_FRAME);
	pic1->ShowWindow(FALSE);

	m_Menu.LoadMenuW(IDR_MENU1);
	SetMenu(&m_Menu);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CplayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CplayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CplayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CplayerDlg::OnBnClickedButtonPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd* pic1 = (CWnd*)GetDlgItem(IDC_STATIC);
	pic1->ShowWindow(TRUE);

	CWnd* pic2 = (CWnd*)GetDlgItem(IDC_FRAME);
	pic2->ShowWindow(FALSE);

	m_VLCplayer.Play(strpath);
}


void CplayerDlg::OnBnClickedPause()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VLCplayer.Pause();

}

void CplayerDlg::OnBnClickedForwardButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VLCplayer.SeekForward();
}

void CplayerDlg::OnBnClickedBackwardButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VLCplayer.SeekBackward();
}


void CplayerDlg::OnBnClickedCancelButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VLCplayer.Stop();
	GetDlgItem(IDC_NAME)->SetWindowText(_T("正在播放：无"));
}


void CplayerDlg::OnCliickedOpenfileButton()
{
	// TODO: 在此添加命令处理程序代码
	//文件类型过滤
	const TCHAR pszFilter[] = _T("MP4 (*.mp4)|*.mp4|MKV (*.mkv)|*.mkv|Any Files (*.*)|*.*|");
	//第一个参数为TRUE是打开文件，为FALSE是文件另存为
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		pszFilter, this);

	dlg.m_ofn.lpstrInitialDir = _T("d:\\"); //设置对话框默认呈现的路径

	CWnd* pic1 = (CWnd*)GetDlgItem(IDC_STATIC);
	pic1->ShowWindow(TRUE);

	CWnd* pic2 = (CWnd*)GetDlgItem(IDC_FRAME);
	pic2->ShowWindow(FALSE);

	if (dlg.DoModal() == IDOK)
	{
		//获取选中的文件的路径
		CString strFilePath = dlg.GetPathName();
		USES_CONVERSION;

		CFile file;
		file.Open(strFilePath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeRead);
		CArchive ach(&file, CArchive::load);

		CFile file1;
		file1.Open(_T(".\\previous.mp4"), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
		CArchive ach1(&file1, CArchive::store);

		ULONGLONG i = file.GetLength();
		char buffer[4096] = { 0 };
		do {

			int num = ach.Read(buffer, 4096);

			ach1.Write(buffer, 4096);

			i -= num;

		} while (i != 0);

		CString name = file.GetFileTitle();
		CString name1;
		name1.Format(L"正在播放：%s", name);
		GetDlgItem(IDC_NAME)->SetWindowText(name1);

		strpath = (W2A(strFilePath));
		m_VLCplayer.Play(strpath);
	}

	
}


void CplayerDlg::OnBnClickedExitButton()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
}


void CplayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_VLCplayer.SeekTo((float)m_slider.GetPos());
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CplayerDlg::OnPosChange(WPARAM wParam, LPARAM lParam)
{
	m_slider.SetPos(m_VLCplayer.GetPos());
	CString s;
	int min = m_VLCplayer.GetTime() / 1000 / 60;
	int sec = m_VLCplayer.GetTime() / 1000 % 60;

	int min2 = m_VLCplayer.GetLength() / 1000 / 60;
	int sec2 = m_VLCplayer.GetLength() / 1000 % 60;


	s.Format(L"%02d:%02d/%02d:%02d", min, sec, min2, sec2);
	s.Format(L"%02d:%02d/%02d:%02d", min, sec, min2, sec2);
	GetDlgItem(IDC_TIME)->SetWindowText(s);


	return TRUE;
}

void CplayerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd* picture = GetDlgItem(IDC_STATIC);//视频窗口
	if (picture->GetSafeHwnd())
	{
		picture->MoveWindow(0, 0, cx, cy - 100);
	}

	CWnd* picture1 = GetDlgItem(IDC_FRAME);//视频窗口
	if (picture1->GetSafeHwnd())
	{
		picture1->MoveWindow(0, 0, cx, cy - 100);
	}

	CWnd* time = GetDlgItem(IDC_TIME);//时间显示窗口
	if (time->GetSafeHwnd())
	{
		CRect timer;
		time->GetWindowRect(&timer);
		time->SetWindowPos(NULL, cx - timer.Width(), cy - 85, 0, 0, 
			SWP_NOSIZE | SWP_NOZORDER);
	}

	CWnd* slider = GetDlgItem(IDC_SLIDER);//进度条
	if (slider->GetSafeHwnd())
	{
		CRect sliderr;
		slider->GetWindowRect(&sliderr);
		slider->SetWindowPos(NULL, 20, cy - 90, 
			cx-150, sliderr.Height(),
			SWP_NOZORDER);
	}

	CWnd* Button_play = GetDlgItem(IDC_PLAY);//播放
	if (Button_play->GetSafeHwnd())
	{
		CRect play;
		Button_play->GetWindowRect(&play);
		Button_play->SetWindowPos(NULL, 20, cy - play.Height()-25, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER);
	}

	CWnd* Button_pause = GetDlgItem(IDC_PAUSE);//暂停
	if (Button_pause->GetSafeHwnd())
	{
		CRect pause;
		Button_pause->GetWindowRect(&pause);
		Button_pause->SetWindowPos(NULL, 30 + pause.Width() , cy - pause.Height()-25, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER);
	}

	CWnd* Button_previous = GetDlgItem(IDC_PREVIOUS);//上次播放
	if (Button_previous->GetSafeHwnd())
	{
		CRect previous;
		Button_previous->GetWindowRect(&previous);
		Button_previous->SetWindowPos(NULL, 40 + 2*previous.Width(), cy - previous.Height() - 25, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER);
	}

	CWnd* Button_name = GetDlgItem(IDC_NAME);//播放文件名称
	if (Button_name->GetSafeHwnd())
	{
		CRect name;
		Button_name->GetWindowRect(&name);
		Button_name->SetWindowPos(NULL, 20, cy - name.Height(), 0, 0,
			SWP_NOSIZE | SWP_NOZORDER);
	}
	
	CWnd* Button_single = GetDlgItem(IDC_SINGLE);//快退
	if (Button_single->GetSafeHwnd())
	{
		CRect single;
		Button_single->GetWindowRect(&single);
		Button_single->SetWindowPos(NULL, cx / 2 - 10 - single.Width(), cy - single.Height() - 25, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER);
	}

	CWnd* Button_backward = GetDlgItem(IDC_BACKWARD);//快退
	if (Button_backward->GetSafeHwnd())
	{
		CRect backward;
		Button_backward->GetWindowRect(&backward);
		Button_backward->SetWindowPos(NULL, cx / 2 + 10 + backward.Width(), cy - backward.Height()-25, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER);
	}

	CWnd* Button_forward = GetDlgItem(IDC_FORWARD);//快进
	if (Button_forward->GetSafeHwnd())
	{
		CRect forward;
		Button_forward->GetWindowRect(&forward);
		Button_forward->SetWindowPos(NULL, cx / 2, cy - forward.Height()-25, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER);
	}

	CWnd* Button_cancel = GetDlgItem(IDC_CANCEL);//取消播放
	if (Button_cancel->GetSafeHwnd())
	{
		CRect cancel;
		Button_cancel->GetWindowRect(&cancel);
		Button_cancel->SetWindowPos(NULL, cx/2 + 20 + 2*cancel.Width(), cy - cancel.Height()-25, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER);
	}

	CWnd* Button_exit = GetDlgItem(IDC_EXIT);//退出
	if (Button_exit->GetSafeHwnd())
	{
		CRect exit;
		Button_exit->GetWindowRect(&exit);
		Button_exit->SetWindowPos(NULL, cx - exit.Width()-20, cy - exit.Height()-25, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER);
	}
	Invalidate();
	UpdateWindow();
}

void CplayerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 1000;			//最小宽度
	lpMMI->ptMinTrackSize.y = 480;			//最小高度
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CplayerDlg::OnBnClickedPrevious()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VLCplayer.Play("./previous.mp4");
	GetDlgItem(IDC_NAME)->SetWindowText(_T("正在播放：缓存视频"));
}

void CplayerDlg::OnABOUT()
{
	// TODO: 在此添加命令处理程序代码
	INT_PTR nes;
	CAboutDlg about;
	nes = about.DoModal();
	if (ID_32771 == nes)
		return;
}


void CplayerDlg::OnBnClickedSingle()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VLCplayer.SingleFrame(strpath);
	CWnd* pic1 = (CWnd*)GetDlgItem(IDC_STATIC);
	pic1->ShowWindow(FALSE);

}


BOOL CplayerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == 'C')//空格键
		{
			m_VLCplayer.Pause();
			float cx, cy, dx, dy, k, t;//跟控件的宽和高以及图片宽和高有关的参数
			CRect   rect;//用于获取图片控件的宽和高
			CImage q;//为cimage图片类创建一个对象
			q.Load(L"./1.bmp");//构造函数的形参是所加载图片的路径 
			cx = q.GetWidth();
			cy = q.GetHeight();//获取图片的宽 高
			k = cy / cx;//获得图片的宽高比

			CWnd* pWnd = GetDlgItem(IDC_FRAME);//获取控件句柄
			pWnd->GetClientRect(&rect);//获取Picture Control控件的客户区
			dx = rect.Width();
			dy = rect.Height();//获得控件的宽高比
			t = dy / dx;//获得控件的宽高比
			if (k >= t)
			{

				rect.right = floor(rect.bottom / k);
				rect.left = (dx - rect.right) / 2;
				rect.right = floor(rect.bottom / k) + (dx - rect.right) / 2;
			}
			else
			{
				rect.bottom = floor(k * rect.right);
				rect.top = (dy - rect.bottom) / 2;
				rect.bottom = floor(k * rect.right) + (dy - rect.bottom) / 2;
			}

			CDC* pDc = NULL;
			pDc = pWnd->GetDC();
			int ModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//设置指定设备环境中的位图拉伸模式

			GetDlgItem(IDC_FRAME)->ShowWindow(TRUE);
			q.StretchBlt(pDc->m_hDC, rect, SRCCOPY);//显示函数
			SetStretchBltMode(pDc->m_hDC, ModeOld);
			ReleaseDC(pDc);//释放指针空间
			if (m_VLCplayer.IsPlaying())
				m_VLCplayer.Pause();
		}
	}
	
	
	return CDialogEx::PreTranslateMessage(pMsg);
}
