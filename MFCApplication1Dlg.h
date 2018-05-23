
// MFCApplication1Dlg.h : 头文件
//

#pragma once
#include "winsock2.h"
#include "WinsockMatTransmissionServer.h"

#include <afxinet.h>

// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    static DWORD WINAPI WaitProc(LPVOID lpPar);
    static DWORD WINAPI RespondProc(LPVOID lpPar);
    HANDLE m_hWaitThread; /* 等待连接线程 */
    HANDLE m_hRespondThread; /* 通信线程句柄 */

    BOOL InitSocket();
    BOOL StartService();

    SOCKET listensocketfd;
    sockaddr_in  saServer;
public:
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    WinsockMatTransmissionServer socketMat;

    void AddFileFromFolder(const CString strFolderPath);
    BOOL DownloadFromFTP(CString remotepath, CString localpath, BOOL isDir, CString ftpaddr, CString username, CString password, int port);
};
