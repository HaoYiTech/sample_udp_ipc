/******************************************************
 * ��һ�Ƽ����ṩ��¼�����Ƽ�ص�ȫƽ̨�޲�����������
 * ֧�ְ���ֱ������㲼�أ��ֲ�ʽ�����洢����̬���ݣ�
 * ֧��΢��ɨ���¼��ȫƽ̨�ʺ�ͳһ������΢��С����
 * ֧�ֶ����������룺����ͷIPC��rtmp��rtsp��MP4�ļ���
 * ֧��ȫʵʱ��ȫ��̬��ȫ��ҳ������ҳǰ��̨����IE8��
 * ֧�ֶ��ն��޲������Ӧ���ţ�flvjs/hls/rtmp�Զ����䣻
 ******************************************************
 * �ٷ���վ => https://myhaoyi.com
 * �������� => http://blog.csdn.net/haoyitech
 * ��Դ���� => https://github.com/HaoYiTech/
 * �Ƽ��Ⱥ => 483663026��QQȺ��
 * ��¼��Ⱥ => 630379661��QQȺ��
 ******************************************************
 * VS2010�Զ����ɵĶԻ�����ʵ�ִ���
 ******************************************************/

#include "stdafx.h"
#include "sample_udp_ipc.h"
#include "sample_udp_ipcDlg.h"
#include "afxdialogex.h"

#include "HKUdpThread.h"
#include "SocketUtils.h"
#include "UtilTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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


Csample_udp_ipcDlg::Csample_udp_ipcDlg(CWnd* pParent /*=NULL*/)
  : CDialogEx(Csample_udp_ipcDlg::IDD, pParent)
{
	m_lpHKUdpThread = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

Csample_udp_ipcDlg::~Csample_udp_ipcDlg()
{
	// ɾ���Ѿ�������̽���߳�...
	if( m_lpHKUdpThread != NULL ) {
		delete m_lpHKUdpThread;
		m_lpHKUdpThread = NULL;
	}
	// �ͷŷ����ϵͳ��Դ...
	SocketUtils::UnInitialize();
	OSThread::UnInitialize();
	//::WSACleanup();
}

void Csample_udp_ipcDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_HOME_WEBSITE, m_ctrlHome);

	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Csample_udp_ipcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &Csample_udp_ipcDlg::OnBnClickedButtonStart)
END_MESSAGE_MAP()

BOOL Csample_udp_ipcDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// ��ʼ�����硢�̡߳��׽���...
	WORD	wsVersion = MAKEWORD(2, 2);
	WSADATA	wsData	  = {0};
	(void)::WSAStartup(wsVersion, &wsData);
	OSThread::Initialize();
	SocketUtils::Initialize();

	// ���÷������ӵ�ַ...
	m_ctrlHome.SetUnderline(TRUE);
	m_ctrlHome.SetURL("https://myhaoyi.com");
	m_ctrlHome.SetWindowText("��ϵ���� => https://myhaoyi.com");
	m_ctrlHome.SetLinkCursor(CUtilTool::GetSysHandCursor());
	m_ctrlHome.SetAutoSize();

	return TRUE;
}

void Csample_udp_ipcDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Csample_udp_ipcDlg::OnPaint()
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR Csample_udp_ipcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ��� ��ʼ̽�� ��ť...
void Csample_udp_ipcDlg::OnBnClickedButtonStart()
{
	// ɾ���Ѿ�������̽���߳�...
	if( m_lpHKUdpThread != NULL ) {
		delete m_lpHKUdpThread;
		m_lpHKUdpThread = NULL;
	}
	// �����µ�̽���̣߳�������...
	GM_Error theErr = GM_NoErr;
	m_lpHKUdpThread = new CHKUdpThread();
	theErr = m_lpHKUdpThread->InitMulticast();
}
