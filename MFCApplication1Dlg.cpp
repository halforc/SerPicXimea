
// MFCApplication1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMFCApplication1Dlg 对话框



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


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
    InitSocket();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
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
    /* 启动等待连接线程 */
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

    CString strNotice; /* 通知消息 */

    while(1) {
        Sleep(10);
        TRACE("WAIT");
        /* 创建通信套接字 */
        SOCKET *pClientSocket = new SOCKET;
        *pClientSocket = ::accept(pThis->listensocketfd, (SOCKADDR*)&remoteAddr, &nAddrLen);

        if(INVALID_SOCKET == *pClientSocket) {
            strNotice = "accept()失败，再次尝试 ...... ";
            ::AfxMessageBox(strNotice);
            continue;
        }

        DWORD dwThreadId = 1;

        /* 启动相应线程与客户端通信 */
        pThis->m_hRespondThread = ::CreateThread(NULL, NULL, CMFCApplication1Dlg::RespondProc, ((LPVOID)pClientSocket), 0, &dwThreadId);

        CloseHandle(pThis->m_hRespondThread);
    }
    return 0;
}

DWORD WINAPI CMFCApplication1Dlg::RespondProc(LPVOID lpPar)
{
    SOCKET *pClientSocket = (SOCKET*)lpPar;
    char buff[4096]; /* 接收数据缓冲区 */

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
        //对接收的数据进行处理  
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
            OutputDebugString((LPCTSTR)(strFullName + L"\n"));//输出文件名  
        }
    }

    finder.Close();
}




/*
参数说明:
remotefile   要下载的ftp目录或文件,如"/杭州/滨江/五水共治.rar"
localpath    保存到本地的目录,如"D:\\Download"
isDir        指明要下载的是文件还是文件夹
ftpaddr      ftp服务器地址
username　　　登陆ftp服务器的用户名
ftpPass       登陆f密码
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
        if(pFtpCon == NULL)    //如果连接失败
        {
            return FALSE;
        }
        CFtpFileFind ftpFinder(pFtpCon);
        BOOL bWorking = ftpFinder.FindFile(remotepath);
        if(bWorking != TRUE)   //如果在FTP服务器上没有找到该文件
        {
            return FALSE;
        }
        if(isDir)   //如果要下载的是文件夹，创建目录
        {
            CreateDirectory(localpath + '\\' + remotefile, NULL);
        } else        //如果要下载的是文件，直接下载
        {
            CString str1 = remotepath;
            CString str2 = localpath + '\\' + remotefile;
            pFtpCon->GetFile(remotefile, localpath + '\\' + remotefile, TRUE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY, 1);
        }
        while(bWorking) {
            bWorking = ftpFinder.FindNextFile();
            if(ftpFinder.IsDots())     //如果找到的是"."（表示返回顶级目录）或者".."（表示返回上一级目录）
            {
                TRACE("%s\n", ftpFinder.GetFileName());
                continue;
            } else if(ftpFinder.IsDirectory())    //如果找到的是文件夹，递归调用DownloadFromFTP()
            {
                TRACE("%s\n", ftpFinder.GetFileName());
                DownloadFromFTP(remotepath + '/' + ftpFinder.GetFileName(), localpath + '\\' + remotefile, TRUE, ftpaddr, username, password, port);
            } else    //如果找到的是文件,直接下载
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
    // TODO:  在此添加控件通知处理程序代码
    //StartService();
    AddFileFromFolder(_T("新建文件夹"));
   // bool flag = DownloadFromFTP(_T(".//毕设封面.doc"), _T("D:"), false, L"192.168.1.102", L"anonymous", NULL, 21);
    std::ofstream of("time.txt");
    DWORD start = GetTickCount();
    bool flag = DownloadFromFTP(_T(".//周杰伦"), _T("D:"), true, L"192.168.1.100", L"anonymous", NULL, 21);
    DWORD end = GetTickCount();
    of << "time cost: " << end - start;
    of.close();
}


void CMFCApplication1Dlg::OnBnClickedButton2()
{
    // TODO:  在此添加控件通知处理程序代码

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
