
// UserDlg.cpp : 实现文件
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


// CUserDlg 对话框



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


// CUserDlg 消息处理程序

BOOL CUserDlg::OnInitDialog()
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
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUserDlg::OnPaint()
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
HCURSOR CUserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//HBRUSH CUserDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  在此更改 DC 的任何特性
//	if (nCtlColor == CTLCOLOR_DLG)
//	{
//		hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
//	}
//	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
//	return hbr;
//}



void CUserDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	number++;
	if (number<m_FileList.size())
	{
		CDC *pDC = GetDlgItem(IDC_STATIC_PPT)->GetDC();//根据ID获得窗口指针再获取与该窗口关联的上下文指针
		HDC hdc = pDC->GetSafeHdc();                      // 获取设备上下文句柄
		CRect rect;
		// 矩形类
		GetDlgItem(IDC_STATIC_PPT)->GetClientRect(&rect); //获取box1客户区
		IplImage *src; // 定义IplImage指针变量src  
		CvvImage cimg;
		std::string tempname = (LPCSTR)CStringA(m_FileList[number]);
		//tmp = tempname.c_str();
		src = cvLoadImage(("../image/" + tempname).c_str(), -1); // 将src指向当前工程文件目录下的图像me.bmp    
		cimg.CopyOf(src, src->nChannels);

		cimg.DrawToHDC(hdc, &rect);
		//输出图像
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
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN){
		CDC *pDC = GetDlgItem(IDC_STATIC_PPT)->GetDC();//根据ID获得窗口指针再获取与该窗口关联的上下文指针
		HDC hdc = pDC->GetSafeHdc();                      // 获取设备上下文句柄
		CRect rect;
		// 矩形类
		GetDlgItem(IDC_STATIC_PPT)->GetClientRect(&rect); //获取box1客户区
		IplImage *src; // 定义IplImage指针变量src  
		CvvImage cimg;
		if (pMsg->wParam == VK_LEFT){
			if (number > 0){
				number--;
				std::string tempname = (LPCSTR)CStringA(m_FileList[number]);
				src = cvLoadImage(("../image/" + tempname).c_str(), -1); // 将src指向当前工程文件目录下的图像me.bmp    
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
				src = cvLoadImage(("../image/" + tempname).c_str(), -1); // 将src指向当前工程文件目录下的图像me.bmp    
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
	// TODO:  在此添加控件通知处理程序代码
	AutoOn = false;
	number = 0;
	CDC *pDC = GetDlgItem(IDC_STATIC_PPT)->GetDC();//根据ID获得窗口指针再获取与该窗口关联的上下文指针
	HDC hdc = pDC->GetSafeHdc();                      // 获取设备上下文句柄
	CRect rect;
	// 矩形类
	GetDlgItem(IDC_STATIC_PPT)->GetClientRect(&rect); //获取box1客户区
	IplImage *src; // 定义IplImage指针变量src  
	CvvImage cimg;
	std::string tempname = (LPCSTR)CStringA(m_FileList[number]);
	//tmp = tempname.c_str();
	src = cvLoadImage(("../image/" + tempname).c_str(), -1); // 将src指向当前工程文件目录下的图像me.bmp    
	cimg.CopyOf(src, src->nChannels);

	cimg.DrawToHDC(hdc, &rect);
	//输出图像
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
	POINT Newp; //获取现在对话框的大小  
	CRect recta;
	GetClientRect(&recta);     //取客户区大小    
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //左上角  
	CPoint OldBRPoint, BRPoint; //右下角  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件    
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID  
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
	// TODO:  在此添加控件通知处理程序代码
	number = 0;
	CDC *pDC = GetDlgItem(IDC_STATIC_PPT)->GetDC();//根据ID获得窗口指针再获取与该窗口关联的上下文指针
	HDC hdc = pDC->GetSafeHdc();                      // 获取设备上下文句柄
	CRect rect;
	// 矩形类
	GetDlgItem(IDC_STATIC_PPT)->GetClientRect(&rect); //获取box1客户区
	IplImage *src; // 定义IplImage指针变量src  
	CvvImage cimg;
	std::string tempname = (LPCSTR)CStringA(m_FileList[number]);
	//tmp = tempname.c_str();
	src = cvLoadImage(("../image/" + tempname).c_str(), -1); // 将src指向当前工程文件目录下的图像me.bmp    
	cimg.CopyOf(src, src->nChannels);

	cimg.DrawToHDC(hdc, &rect);
	//输出图像
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
