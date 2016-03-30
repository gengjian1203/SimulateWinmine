
// SimulateWinmineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SimulateWinmine.h"
#include "SimulateWinmineDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimulateWinmineDlg 对话框




CSimulateWinmineDlg::CSimulateWinmineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSimulateWinmineDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimulateWinmineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LEVEL, m_comboLevel);
}

BEGIN_MESSAGE_MAP(CSimulateWinmineDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CSimulateWinmineDlg::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_START, &CSimulateWinmineDlg::OnBnClickedButtonStart)
END_MESSAGE_MAP()


// CSimulateWinmineDlg 消息处理程序

BOOL CSimulateWinmineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	Init();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSimulateWinmineDlg::OnPaint()
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
HCURSOR CSimulateWinmineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSimulateWinmineDlg::OnBnClickedButtonInit()
{
	// TODO: 在此添加控件通知处理程序代码
	// 通过标题名和类名，获取游戏的窗口句柄
	if (GetGameHwnd())
	{
		// 获取进程ID
		GetGamePID();
		// 注入游戏进程
		OpenGameProcess();
		// 等待开始
		ShowStatus(2);
		// 更新界面
		SetWindowEnable(1);

	}
	else
	{
		// 未找到扫雷
		ShowStatus(1);
		// 更新界面
		SetWindowEnable(0);
	}

}

void CSimulateWinmineDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (GetGameHwnd())
	{
		// 获取游戏数据
		m_nCount = ReadMemoryInt32(m_nAddCount);
		m_nTime = ReadMemoryInt32(m_nAddTime);
		m_nWidth = ReadMemoryInt32(m_nAddWidth);
		m_nHeight = ReadMemoryInt32(m_nAddHeight);
		// 扫雷中……
		ShowStatus(3);
		if (RunAction())
		{
			// 扫雷成功
			ShowStatus(4);
		}
		else
		{
			// 扫雷失败
			ShowStatus(5);
			// 更新界面
			SetWindowEnable(0);
		}
	}
	else
	{
		// 未找到扫雷
		ShowStatus(1);
		// 更新界面
		SetWindowEnable(0);
	}

}

bool CSimulateWinmineDlg::SetWindowEnable(int nEnable)
{
	if (1 == nEnable)
	{
		// 可开始扫雷
		GetDlgItem(IDC_BUTTON_INIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_LEVEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

	}
	else if (0 == nEnable)
	{
		// 不可开始扫雷
		GetDlgItem(IDC_BUTTON_INIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	}
	else
	{
		;
	}
	return true;
}

bool CSimulateWinmineDlg::ShowStatus(int nStatus)
{
// 	CString strWnd;
// 	strWnd.Format(_T("Hwnd:\t%d\nPID:\t%d\nhand:\t%d\nCount:\t%d\nTime:\t%d\nWidth:\t%d\nHeight:\t%d\n"), 
// 		m_hGameHwnd, m_hGamePID, m_hGameHandle, 
// 		m_nCount, m_nTime, m_nWidth, m_nHeight);
	
	GetDlgItem(IDC_STATUS)->SetWindowTextW(m_strStatus[nStatus]);
	return true;
}

bool CSimulateWinmineDlg::ReadINI(void)
{

	return true;
}

bool CSimulateWinmineDlg::Init(void)
{
	// 初始化提示信息
	m_strStatus[0] = "【当前状态】等待初始化";
	m_strStatus[1] = "【当前状态】未找到扫雷";
	m_strStatus[2] = "【当前状态】等待开始";
	m_strStatus[3] = "【当前状态】扫雷中……";
	m_strStatus[4] = "【当前状态】扫雷完毕";
	m_strStatus[5] = "异常错误，建议初始化";
	m_strStatus[6] = "【当前状态】空闲";
	m_strStatus[7] = "【当前状态】空闲";
	m_strStatus[8] = "【当前状态】空闲";
	m_strStatus[9] = "【当前状态】空闲";
	// 初始化成员变量
	m_hGameHwnd = 0x00;
	m_hGamePID = 0x00;
	m_hGameHandle = 0x00;
	m_nAddCount = 0x01005330;
	m_nAddTime = 0x0100579C;
	m_nAddWidth = 0x01005334;
	m_nAddHeight = 0x01005338;
	m_nAddData = 0x01005360;
	m_nCount = 0x00;
	m_nTime = 0x00;
	m_nWidth = 0x00;
	m_nHeight = 0x00;
	// 初始化控件
	SetWindowEnable(0);
	m_comboLevel.SetCurSel(2);

	// 读取配置文件
	ReadINI();
	
	return true;
}

bool CSimulateWinmineDlg::GetGameHwnd(void)
{
	// 查找指定条件的句柄
	m_hGameHwnd = ::FindWindowW(_T("扫雷"), _T("扫雷"));
	if (0x00 == m_hGameHwnd)
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool CSimulateWinmineDlg::GetGamePID(void)
{
	//根据窗口句柄获取到进程ID
	::GetWindowThreadProcessId(m_hGameHwnd, &m_hGamePID);

	return true;
}

bool CSimulateWinmineDlg::OpenGameProcess(void)
{
	//根据进程ID打开进程
	m_hGameHandle = ::OpenProcess(PROCESS_ALL_ACCESS, false, m_hGamePID);

	return true;
}

int CSimulateWinmineDlg::ReadMemoryByte(int nAddr)
{
	DWORD bytes = 0x00;
	int nBuffer = 0x00;
	//根据返回的句柄读取内存指定内存地址的数据
	::ReadProcessMemory(m_hGameHandle, (LPCVOID)nAddr, &nBuffer, 1, &bytes);
	return nBuffer;
}

int CSimulateWinmineDlg::ReadMemoryInt32(int nAddr)
{
	DWORD bytes = 0x00;
	int nBuffer = 0x00;
	//根据返回的句柄读取内存指定内存地址的数据
	::ReadProcessMemory(m_hGameHandle, (LPCVOID)nAddr, &nBuffer, sizeof(int), &bytes);
	return nBuffer;
}

bool CSimulateWinmineDlg::RunAction(void)
{
	CString strSign;
	int nSign;
	int nDelay = (3 - m_comboLevel.GetCurSel()) * 6;

	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 1; j <= m_nWidth; j++)
		{
			nSign = ReadMemoryByte(m_nAddData + i * 32 + j);
			// 143为雷，15为无雷
			if (15 == nSign)
			{
				ClickLeftButton(j, i);
			}
			else if(143 == nSign)
			{
				ClickRightButton(j, i);
			} 
			else
			{
				//return false;
				
			}
			Sleep(nDelay);
		}
	}

	return true;
}

bool CSimulateWinmineDlg::ClickLeftButton(int nX, int nY)
{
	::PostMessageW(m_hGameHwnd, WM_LBUTTONDOWN, 0, (4 + 16 * nX) + ((64 + 16 * nY) << 16));
	//Sleep(1);
	::PostMessageW(m_hGameHwnd, WM_LBUTTONUP, 0, (4 + 16 * nX) + ((64 + 16 * nY) << 16));

	return true;
}

bool CSimulateWinmineDlg::ClickRightButton(int nX, int nY)
{
	::PostMessageW(m_hGameHwnd, WM_RBUTTONDOWN, 0, (4 + 16 * nX) + ((64 + 16 * nY) << 16));
	//Sleep(1);
	::PostMessageW(m_hGameHwnd, WM_RBUTTONUP, 0, (4 + 16 * nX) + ((64 + 16 * nY) << 16));

	return true;
}



