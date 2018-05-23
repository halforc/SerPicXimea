
// MFCApplication1Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg �Ի���



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMFCApplication1Dlg ��Ϣ�������

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
    InitSocket();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
BOOL CMFCApplication1Dlg::InitSocket()
{
    WSADATA wsaData;
    WORD sockVersion = MAKEWORD(2, 2);
    if(WSAStartup(sockVersion, &wsaData)) {
        AfxMessageBox(_T("failed to load winsock!"), MB_OK | MB_ICONSTOP);
        return 0;
    }
    listensocketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    saServer.sin_family = AF_INET;
    saServer.sin_addr.S_un.S_addr = inet_addr("192.168.1.100");
    saServer.sin_port = htons(8888);
    bind(listensocketfd, (sockaddr*)&saServer, sizeof(saServer));
    listen(listensocketfd, 50);
    return TRUE;
}
BOOL CMFCApplication1Dlg::StartService()
{
    /* �����ȴ������߳� */
    DWORD dwThreadId = 0;

    m_hWaitThread = ::CreateThread(NULL, NULL, CMFCApplication1Dlg::WaitProc, ((LPVOID)this), 0, &dwThreadId);

    CloseHandle(m_hWaitThread);
    return TRUE;
}

DWORD WINAPI CMFCApplication1Dlg::WaitProc(LPVOID lpPar)
{
    sockaddr_in remoteAddr;
    int nAddrLen = sizeof(remoteAddr);

    CMFCApplication1Dlg *pThis = (CMFCApplication1Dlg*)lpPar;

    CString strNotice; /* ֪ͨ��Ϣ */

    while(1) {
        Sleep(10);
        TRACE("WAIT");
        /* ����ͨ���׽��� */
        SOCKET *pClientSocket = new SOCKET;
        *pClientSocket = ::accept(pThis->listensocketfd, (SOCKADDR*)&remoteAddr, &nAddrLen);

        if(INVALID_SOCKET == *pClientSocket) {
            strNotice = "accept()ʧ�ܣ��ٴγ��� ...... ";
            ::AfxMessageBox(strNotice);
            continue;
        }

        DWORD dwThreadId = 1;

        /* ������Ӧ�߳���ͻ���ͨ�� */
        pThis->m_hRespondThread = ::CreateThread(NULL, NULL, CMFCApplication1Dlg::RespondProc, ((LPVOID)pClientSocket), 0, &dwThreadId);

        CloseHandle(pThis->m_hRespondThread);
    }
    return 0;
}

DWORD WINAPI CMFCApplication1Dlg::RespondProc(LPVOID lpPar)
{
    SOCKET *pClientSocket = (SOCKET*)lpPar;
    char buff[4096]; /* �������ݻ����� */

    struct sockaddr_in sa;
    int len = sizeof(sa);
    getpeername(*pClientSocket, (struct sockaddr *)&sa, &len);
    TRACE("START");
    while(1) {
        Sleep(10);
        int nRecv = ::recv(*pClientSocket, buff, 4096, 0);
        if(nRecv > 0) {
            buff[nRecv] = '\0';
            buff[nRecv+1] = '\n';
        }

        TRACE(buff);
        //�Խ��յ����ݽ��д���  
    }
}



void CMFCApplication1Dlg::AddFileFromFolder(const CString strFolderPath)
{
    CString strMatch = strFolderPath + _T("\\*.*");
    CString strFullName;
    CFileFind finder;

    BOOL bWorking = finder.FindFile(strMatch);
    while(bWorking) {
        bWorking = finder.FindNextFile();

        if(finder.IsDots())
            continue;

        if(finder.IsDirectory())
            AddFileFromFolder(finder.GetFileName() + _T("\\*.*"));
        else {
            strFullName = finder.GetFilePath();
            OutputDebugString((LPCTSTR)(strFullName + L"\n"));//����ļ���  
        }
    }

    finder.Close();
}




/*
����˵��:
remotefile   Ҫ���ص�ftpĿ¼���ļ�,��"/����/����/��ˮ����.rar"
localpath    ���浽���ص�Ŀ¼,��"D:\\Download"
isDir        ָ��Ҫ���ص����ļ������ļ���
ftpaddr      ftp��������ַ
username��������½ftp���������û���
ftpPass       ��½f����
*/
BOOL CMFCApplication1Dlg::DownloadFromFTP(CString remotepath, CString localpath, BOOL isDir, CString ftpaddr, CString username, CString password, int port)
{
    int index = remotepath.ReverseFind('/');
    if(index == -1) {
        return FALSE;
    }
    CString remotefile = remotepath.Mid(index + 1, remotepath.GetLength());
    CInternetSession sess(_T("Download Files Session"));
    CFtpConnection* pFtpCon = NULL;
    try {
        pFtpCon = sess.GetFtpConnection(ftpaddr, username, password, port);
        if(pFtpCon == NULL)    //�������ʧ��
        {
            return FALSE;
        }
        CFtpFileFind ftpFinder(pFtpCon);
        BOOL bWorking = ftpFinder.FindFile(remotepath);
        if(bWorking != TRUE)   //�����FTP��������û���ҵ����ļ�
        {
            return FALSE;
        }
        if(isDir)   //���Ҫ���ص����ļ��У�����Ŀ¼
        {
            CreateDirectory(localpath + '\\' + remotefile, NULL);
        } else        //���Ҫ���ص����ļ���ֱ������
        {
            CString str1 = remotepath;
            CString str2 = localpath + '\\' + remotefile;
            pFtpCon->GetFile(remotefile, localpath + '\\' + remotefile, TRUE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY, 1);
        }
        while(bWorking) {
            bWorking = ftpFinder.FindNextFile();
            if(ftpFinder.IsDots())     //����ҵ�����"."����ʾ���ض���Ŀ¼������".."����ʾ������һ��Ŀ¼��
            {
                TRACE("%s\n", ftpFinder.GetFileName());
                continue;
            } else if(ftpFinder.IsDirectory())    //����ҵ������ļ��У��ݹ����DownloadFromFTP()
            {
                TRACE("%s\n", ftpFinder.GetFileName());
                DownloadFromFTP(remotepath + '/' + ftpFinder.GetFileName(), localpath + '\\' + remotefile, TRUE, ftpaddr, username, password, port);
            } else    //����ҵ������ļ�,ֱ������
            {
                TRACE("%s\n", ftpFinder.GetFileName());
                CString str = ftpFinder.GetFileName();
                CString str1 = remotepath + '\\' + ftpFinder.GetFileName();
                CString str2 = localpath + '\\' + remotefile + '\\' + ftpFinder.GetFileName();
                pFtpCon->GetFile(remotepath + '\\' + ftpFinder.GetFileName(), localpath + '\\' + remotefile + '\\' + ftpFinder.GetFileName(), TRUE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY, 1);
            }
        }
    } catch(CInternetException* pEx) {
        TCHAR sz[1024];
        pEx->GetErrorMessage(sz, 1024);
        printf("%s", sz);
        pEx->Delete();
    }
    if(pFtpCon != NULL) {
        pFtpCon->Close();
        delete pFtpCon;
        pFtpCon = NULL;
    }

    return TRUE;
}

void CMFCApplication1Dlg::OnBnClickedButton1()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    //StartService();
    AddFileFromFolder(_T("�½��ļ���"));
   // bool flag = DownloadFromFTP(_T(".//�������.doc"), _T("D:"), false, L"192.168.1.102", L"anonymous", NULL, 21);
    std::ofstream of("time.txt");
    DWORD start = GetTickCount();
    bool flag = DownloadFromFTP(_T(".//�ܽ���"), _T("D:"), true, L"192.168.1.100", L"anonymous", NULL, 21);
    DWORD end = GetTickCount();
    of << "time cost: " << end - start;
    of.close();
}


void CMFCApplication1Dlg::OnBnClickedButton2()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������

    if(socketMat.socketConnect(6666) < 0) {
        return;
    }

    cv::Mat image;
    while(1) {
        if(socketMat.receive(image) > 0) {
            cv::imshow("", image);
            break;
        }
    }

    socketMat.socketDisconnect();

}
