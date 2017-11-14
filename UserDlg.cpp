
// UserDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "User.h"
#include "UserDlg.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include "resource.h"
#include <opencv\cvwimage.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <vector>
#include <string>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
vector<CString> m_FileList;
int number = 0;
CString sample;
CString sum;
CString divide;
UINT timer_num = 0;

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


// CUserDlg �Ի���



CUserDlg::CUserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserDlg::IDD, pParent),bFullScreen(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUserDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CUserDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_EDIT1, &CUserDlg::OnBnClickedHandOn)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_AUTO, &CUserDlg::OnBnClickedAutoOn)
END_MESSAGE_MAP()


// CUserDlg ��Ϣ�������

BOOL CUserDlg::OnInitDialog()
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
	CString csDirPath = _T("../image/*.jpg");
	HANDLE file1;
	WIN32_FIND_DATA fileData;
	file1 = FindFirstFile(csDirPath.GetBuffer(), &fileData);
	m_FileList.push_back(fileData.cFileName);
	bool bState = false;
	bState = FindNextFile(file1, &fileData);
	while (bState){
		m_FileList.push_back(fileData.cFileName);
		bState = FindNextFile(file1, &fileData);
	}
	divide.Format(_T("%s"), "/");
	sum.Format(_T("%d"), m_FileList.size());
	
	sample.Format(_T("%d"), 0);
	SetDlgItemTextW(IDC_STATIC_TEXT, sample + divide + sum);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CUserDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUserDlg::OnPaint()
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
HCURSOR CUserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//HBRUSH CUserDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  �ڴ˸��� DC ���κ�����
//	if (nCtlColor == CTLCOLOR_DLG)
//	{
//		hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
//	}
//	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
//	return hbr;
//}



void CUserDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	number++;
	if (number<m_FileList.size())
	{
		CDC *pDC = GetDlgItem(IDC_STATIC_PPT)->GetDC();//����ID��ô���ָ���ٻ�ȡ��ô��ڹ�����������ָ��
		HDC hdc = pDC->GetSafeHdc();                      // ��ȡ�豸�����ľ��
		CRect rect;
		// ������
		GetDlgItem(IDC_STATIC_PPT)->GetClientRect(&rect); //��ȡbox1�ͻ���
		IplImage *src; // ����IplImageָ�����src  
		CvvImage cimg;
		std::string tempname = (LPCSTR)CStringA(m_FileList[number]);
		//tmp = tempname.c_str();
		src = cvLoadImage(("../image/" + tempname).c_str(), -1); // ��srcָ��ǰ�����ļ�Ŀ¼�µ�ͼ��me.bmp    
		cimg.CopyOf(src, src->nChannels);

		cimg.DrawToHDC(hdc, &rect);
		//���ͼ��
		ReleaseDC(pDC);
		cvReleaseImage(&src);
		cimg.Destroy();
		divide.Format(_T("%s"), "/");
		sum.Format(_T("%d"), m_FileList.size());
		sample.Format(_T("%d"), number + 1);
		SetDlgItemTextW(IDC_STATIC_TEXT, sample + divide + sum);
	}
	else
	{
		KillTimer(timer_num);
	}
	
	CDialogEx::OnTimer(nIDEvent);
}


void CUserDlg::OnBnClickedCancel()
{
	 CDialogEx::OnCancel();
}


BOOL CUserDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN){
		CDC *pDC = GetDlgItem(IDC_STATIC_PPT)->GetDC();//����ID��ô���ָ���ٻ�ȡ��ô��ڹ�����������ָ��
		HDC hdc = pDC->GetSafeHdc();                      // ��ȡ�豸�����ľ��
		CRect rect;
		// ������
		GetDlgItem(IDC_STATIC_PPT)->GetClientRect(&rect); //��ȡbox1�ͻ���
		IplImage *src; // ����IplImageָ�����src  
		CvvImage cimg;
		if (pMsg->wParam == VK_LEFT){
			if (number > 0){
				number--;
				std::string tempname = (LPCSTR)CStringA(m_FileList[number]);
				src = cvLoadImage(("../image/" + tempname).c_str(), -1); // ��srcָ��ǰ�����ļ�Ŀ¼�µ�ͼ��me.bmp    
				cimg.CopyOf(src, src->nChannels);
				cimg.DrawToHDC(hdc, &rect);
				ReleaseDC(pDC);
				cvReleaseImage(&src);
				cimg.Destroy();
				divide.Format(_T("%s"), "/");
				sum.Format(_T("%d"), m_FileList.size());
				sample.Format(_T("%d"), number+1);
				SetDlgItemTextW(IDC_STATIC_TEXT, sample + divide + sum);
			}
		}
		else if (pMsg->wParam == VK_RIGHT){
			if (number < m_FileList.size()-1){
				number++;
				std::string tempname = (LPCSTR)CStringA(m_FileList[number]);
				src = cvLoadImage(("../image/" + tempname).c_str(), -1); // ��srcָ��ǰ�����ļ�Ŀ¼�µ�ͼ��me.bmp    
				cimg.CopyOf(src, src->nChannels);
				cimg.DrawToHDC(hdc, &rect);
				ReleaseDC(pDC);
				cvReleaseImage(&src);
				cimg.Destroy();
				divide.Format(_T("%s"), "/");
				sum.Format(_T("%d"), m_FileList.size());
				sample.Format(_T("%d"), number+1);
				SetDlgItemTextW(IDC_STATIC_TEXT, sample + divide + sum);
			}
		}
		else if (pMsg->wParam == VK_RETURN){
			int iShowStat = bFullScreen ? SW_HIDE : SW_SHOW;

			if (!bFullScreen)
			{
				//get current system resolution  
				int g_iCurScreenWidth = GetSystemMetrics(SM_CXSCREEN);
				int g_iCurScreenHeight = GetSystemMetrics(SM_CYSCREEN);

				//for full screen while backplay  
				GetWindowPlacement(&m_struOldWndpl);

				CRect rectWholeDlg;//entire client(including title bar)  
				CRect rectClient;//client area(not including title bar)  
				CRect rectFullScreen;
				GetWindowRect(&rectWholeDlg);
				RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &rectClient);
				ClientToScreen(&rectClient);

				rectFullScreen.left = rectWholeDlg.left - rectClient.left;
				rectFullScreen.top = rectWholeDlg.top - rectClient.top;
				rectFullScreen.right = rectWholeDlg.right + g_iCurScreenWidth - rectClient.right;
				rectFullScreen.bottom = rectWholeDlg.bottom + g_iCurScreenHeight - rectClient.bottom;
				//enter into full screen;  
				WINDOWPLACEMENT struWndpl;
				struWndpl.length = sizeof(WINDOWPLACEMENT);
				struWndpl.flags = 0;
				struWndpl.showCmd = SW_SHOWNORMAL;
				struWndpl.rcNormalPosition = rectFullScreen;
				SetWindowPlacement(&struWndpl);

				bFullScreen = true;
			}
		
		}
		else if (pMsg->wParam == VK_BACK){
			if (bFullScreen){
				SetWindowPlacement(&m_struOldWndpl);
				bFullScreen = false;
			}
		}
		
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CUserDlg::OnBnClickedHandOn()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	AutoOn = false;
	number = 0;
	CDC *pDC = GetDlgItem(IDC_STATIC_PPT)->GetDC();//����ID��ô���ָ���ٻ�ȡ��ô��ڹ�����������ָ��
	HDC hdc = pDC->GetSafeHdc();                      // ��ȡ�豸�����ľ��
	CRect rect;
	// ������
	GetDlgItem(IDC_STATIC_PPT)->GetClientRect(&rect); //��ȡbox1�ͻ���
	IplImage *src; // ����IplImageָ�����src  
	CvvImage cimg;
	std::string tempname = (LPCSTR)CStringA(m_FileList[number]);
	//tmp = tempname.c_str();
	src = cvLoadImage(("../image/" + tempname).c_str(), -1); // ��srcָ��ǰ�����ļ�Ŀ¼�µ�ͼ��me.bmp    
	cimg.CopyOf(src, src->nChannels);

	cimg.DrawToHDC(hdc, &rect);
	//���ͼ��
	ReleaseDC(pDC);
	cvReleaseImage(&src);
	cimg.Destroy();
	divide.Format(_T("%s"), "/");
	sum.Format(_T("%d"), m_FileList.size());
	sample.Format(_T("%d"), 1);
	SetDlgItemTextW(IDC_STATIC_TEXT, sample + divide + sum);
}


void CUserDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here  
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		ReSize();
	}
	Invalidate();
}

void CUserDlg::ReSize()
{
	float fsp[2];
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С  
	CRect recta;
	GetClientRect(&recta);     //ȡ�ͻ�����С    
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //���Ͻ�  
	CPoint OldBRPoint, BRPoint; //���½�  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ�    
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID  
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = Newp;
}

void CUserDlg::OnBnClickedAutoOn()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	number = 0;
	CDC *pDC = GetDlgItem(IDC_STATIC_PPT)->GetDC();//����ID��ô���ָ���ٻ�ȡ��ô��ڹ�����������ָ��
	HDC hdc = pDC->GetSafeHdc();                      // ��ȡ�豸�����ľ��
	CRect rect;
	// ������
	GetDlgItem(IDC_STATIC_PPT)->GetClientRect(&rect); //��ȡbox1�ͻ���
	IplImage *src; // ����IplImageָ�����src  
	CvvImage cimg;
	std::string tempname = (LPCSTR)CStringA(m_FileList[number]);
	//tmp = tempname.c_str();
	src = cvLoadImage(("../image/" + tempname).c_str(), -1); // ��srcָ��ǰ�����ļ�Ŀ¼�µ�ͼ��me.bmp    
	cimg.CopyOf(src, src->nChannels);

	cimg.DrawToHDC(hdc, &rect);
	//���ͼ��
	ReleaseDC(pDC);
	cvReleaseImage(&src);
	cimg.Destroy();
	divide.Format(_T("%s"), "/");
	sum.Format(_T("%d"), m_FileList.size());
	sample.Format(_T("%d"), 1);
	SetDlgItemTextW(IDC_STATIC_TEXT, sample + divide + sum);
	CString timeEdit;
	int timerInt = 1;
	AutoOn = true;
	GetDlgItem(IDC_EDIT2)->GetWindowText(timeEdit);
	timerInt = (_wtoi(timeEdit))>0 ? _wtoi(timeEdit) : 1;
	if (AutoOn){
		timer_num = SetTimer(1, timerInt * 1000, NULL);
	}
	//CDialogEx::OnOK();
}
