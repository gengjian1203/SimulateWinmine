
// SimulateWinmineDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CSimulateWinmineDlg 对话框
class CSimulateWinmineDlg : public CDialogEx
{
// 构造
public:
	CSimulateWinmineDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SIMULATEWINMINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_comboLevel;
	CComboBox m_comboVersion;

public:
	afx_msg void OnBnClickedButtonInit();
	afx_msg void OnBnClickedButtonStart();

// 自定义
private:
	CString m_strStatus[10];
	//游戏的窗口句柄
	HWND m_hGameHwnd;
	//游戏的进程ID
	DWORD m_hGamePID;
	//游戏的实例句柄
	HANDLE m_hGameHandle;

	int m_nAddCount[3];
	int m_nAddTime[3];
	int m_nAddWidth[3];
	int m_nAddHeight[3];
	int m_nAddData[3];
	int m_nCount;
	int m_nTime;
	int m_nWidth;
	int m_nHeight;

	int m_WinmineVersion;

private:
	bool SetWindowEnable(int nEnable);
	bool ShowStatus(int nStatus);
	bool ReadINI(void);
	bool Init(void);
	bool GetGameHwnd(void);
	bool GetGamePID(void);
	bool EnableDebugPrivilege(void);
	bool OpenGameProcess(void);
	int ReadMemoryByte(int nAddr);
	int ReadMemoryInt32(int nAddr);
	bool RunAction(void);
	bool ClickLeftButton(int nX, int nY);
	bool ClickRightButton(int nX, int nY);

public:

};
