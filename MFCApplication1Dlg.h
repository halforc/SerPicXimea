
// MFCApplication1Dlg.h : ͷ�ļ�
//

#pragma once
#include "winsock2.h"
#include "WinsockMatTransmissionServer.h"

#include <afxinet.h>

// CMFCApplication1Dlg �Ի���
class CMFCApplication1Dlg : public CDialogEx
{
// ����
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    static DWORD WINAPI WaitProc(LPVOID lpPar);
    static DWORD WINAPI RespondProc(LPVOID lpPar);
    HANDLE m_hWaitThread; /* �ȴ������߳� */
    HANDLE m_hRespondThread; /* ͨ���߳̾�� */

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
