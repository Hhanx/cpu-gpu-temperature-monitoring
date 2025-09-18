
// MFCApplication1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#include "Resource.h"


#include <thread>
#include <iostream>
#include <fstream>
using namespace std;








#include "getTemp.h"


#include <math.h>

#include <windows.h>
#include <winuser.h>
#include <VersionHelpers.h>
#include <ShellScalingAPI.h>
#pragma comment(lib, "Shcore.lib")




typedef struct {
	int width;
	int height;
} Size;

Size calculateWindowSize(int screenWidth, int screenHeight) {
	// 已知数据点: (分辨率宽, 分辨率高) -> (窗口宽, 窗口高)
	const int x1 = 800, y1 = 600;
	const int width1 = 85, height1 = 46;

	const int x2 = 1920, y2 = 1080;  // 新增数据点
	const int width2 = 110, height2 = 55;

	const int x3 = 2560, y3 = 1600;
	const int width3 = 144, height3 = 75;

	// 计算当前屏幕的宽高比
	double currentAspect = (double)screenWidth / screenHeight;

	// 计算三个已知点的宽高比
	double aspect1 = (double)x1 / y1;
	double aspect2 = (double)x2 / y2;
	double aspect3 = (double)x3 / y3;

	// 基于宽高比计算权重 (距离越近权重越大)
	double weight1 = 1.0 / (fabs(currentAspect - aspect1) + 0.0001); // 避免除零
	double weight2 = 1.0 / (fabs(currentAspect - aspect2) + 0.0001);
	double weight3 = 1.0 / (fabs(currentAspect - aspect3) + 0.0001);

	// 归一化权重
	double totalWeight = weight1 + weight2 + weight3;
	weight1 /= totalWeight;
	weight2 /= totalWeight;
	weight3 /= totalWeight;

	// 加权计算窗口尺寸
	Size result;
	result.width = (int)(weight1 * width1 + weight2 * width2 + weight3 * width3);
	result.height = (int)(weight1 * height1 + weight2 * height2 + weight3 * height3);

	return result;
}


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	//DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnBnClickedButton1();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}




// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//析构函数
CMFCApplication1Dlg::~CMFCApplication1Dlg()
{
	//程序结束释放程序释放内存







	exit(0);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()


	ON_MESSAGE(WM_USER + 100, OnTrayNotify)  // 托盘通知消息
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()

	ON_COMMAND(ID_TRAY_EXIT, OnTrayExit)

	ON_MESSAGE(WM_DPICHANGED, &CMFCApplication1Dlg::OnDpiChanged)

	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()





CString sToc(string str) {
	char buf[16] = "";
	sprintf(buf, "%s", str.c_str());
	return CString(buf);
}


void thread_fun(void* p) {
	CMFCApplication1Dlg* a = (CMFCApplication1Dlg*)p;
	getTemp get;


	//获取数据
	string strpath = "C:\\温度监视缓存\\temp.txt";
	string tmp;
	fstream fp;
	fp.open(strpath, std::ios::in);
	if (fp.is_open()) {
		getline(fp, tmp);
	}

	int aa, bb;
	char buf[128] = "";
	strcpy(buf, tmp.c_str());
	aa = (buf[4] == '0') ? 1 : 0;
	bb = (buf[9] == '0') ? 1 : 0;

	while (1)
	{
		string cpuTmp, gpuTmp;
		//获取温度
		//cpu
		if (aa == 1)
			cpuTmp = get.getCpuTemp(get.computer);
		else
			cpuTmp = get.getIntelCpuTemp(get.computer);
		//gpu
		if (bb == 1)
			gpuTmp = get.getGpuTemp(get.computer);
		else
			gpuTmp = get.getNVGpuTemp(get.computer);

		a->SetDlgItemTextW(IDC_STATIC1, L"CPU : " + sToc(cpuTmp) + L"℃");

		a->SetDlgItemTextW(IDC_STATIC2, L"GPU : " + sToc(gpuTmp) + L"℃");

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void CMFCApplication1Dlg::ShowTrayIcon(BOOL bShow)
{
	if (bShow)
	{
		Shell_NotifyIcon(NIM_ADD, &m_nid);
	}
	else
	{
		Shell_NotifyIcon(NIM_DELETE, &m_nid);
	}
}


//退出
void CMFCApplication1Dlg::OnTrayExit()
{
	// 移除托盘图标
	ShowTrayIcon(FALSE);
	delete this;
}

// 处理托盘图标消息
LRESULT CMFCApplication1Dlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
		return 0;

	switch (LOWORD(lParam))
	{
		case WM_RBUTTONUP:
		{
			// 创建菜单
			CMenu menu;
			menu.CreatePopupMenu();

			// 添加菜单项
			menu.AppendMenu(MF_STRING, ID_TRAY_EXIT, _T("退出"));

			// 获取鼠标位置
			CPoint pt;
			GetCursorPos(&pt);

			// 显示菜单
			SetForegroundWindow();
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
			menu.DestroyMenu();
			break;
		}

		case WM_LBUTTONDBLCLK:  // 双击
		{
			// 恢复窗口显示
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
			break;
		}
	}

	return 0;
}


void CMFCApplication1Dlg::OnClose()
{
	// 不直接关闭窗口，而是隐藏到托盘
	ShowWindow(SW_HIDE);
}


LRESULT CMFCApplication1Dlg::OnDpiChanged(WPARAM wParam, LPARAM lParam)
{
	// 获取新的 DPI 值
	UINT newDpi = HIWORD(wParam);

	// 获取建议的窗口位置和大小
	RECT* suggestedRect = (RECT*)lParam;
	SetWindowPos(NULL,
		suggestedRect->left,
		suggestedRect->top,
		suggestedRect->right - suggestedRect->left,
		suggestedRect->bottom - suggestedRect->top,
		SWP_NOZORDER | SWP_NOACTIVATE);

	// 计算缩放比例
	double scaleFactor = (double)newDpi / 96.0;

	// 调整所有子控件
	CRect rect;
	CWnd* childWnd = GetWindow(GW_CHILD);
	while (childWnd)
	{
		childWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		// 缩放控件位置和大小
		rect.left = (int)(rect.left * scaleFactor);
		rect.top = (int)(rect.top * scaleFactor);
		rect.right = (int)(rect.right * scaleFactor);
		rect.bottom = (int)(rect.bottom * scaleFactor);

		childWnd->MoveWindow(&rect);
		childWnd = childWnd->GetNextWindow();
	}

	return 0;
}

// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
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


	HKEY hKey;
	//std::string strRegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";


	//1、找到系统的启动项  
	//if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) ///打开启动项       
	//{
	//	//2、得到本程序自身的全路径
	//	TCHAR strExeFullDir[MAX_PATH];
	//	GetModuleFileName(NULL, strExeFullDir, MAX_PATH);


	//	//3、判断注册表项是否已经存在
	//	TCHAR strDir[MAX_PATH] = {};
	//	DWORD nLength = MAX_PATH;
	//	long result = RegGetValue(hKey, nullptr, _T("温度监测"), RRF_RT_REG_SZ, 0, strDir, &nLength);


	//	//4、已经存在
	//	if (result != ERROR_SUCCESS || _tcscmp(strExeFullDir, strDir) != 0)
	//	{
	//		//5、添加一个子Key,并设置值，"GISRestart"是应用程序名字（不加后缀.exe） 
	//		RegSetValueEx(hKey, _T("温度监测"), 0, REG_SZ, (LPBYTE)strExeFullDir, (lstrlen(strExeFullDir) + 1) * sizeof(TCHAR));


	//		//6、关闭注册表
	//		RegCloseKey(hKey);
	//	}
	//}


	//创建快捷方式到桌面

	TCHAR strExeFullDir[MAX_PATH];
	GetModuleFileName(NULL, strExeFullDir, MAX_PATH);

	CString fname_to_create_link = strExeFullDir;
	CString lnk_fname = L"温度监视";

	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		IShellLink* pisl;
		hr = CoCreateInstance(CLSID_ShellLink, NULL,
			CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pisl);
		if (SUCCEEDED(hr))
		{
			IPersistFile* pIPF;
			pisl->SetPath(fname_to_create_link);
			hr = pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF);
			if (SUCCEEDED(hr))
			{
				CString csStartPath;
				SHGetSpecialFolderPath(NULL, (LPWSTR)(LPCTSTR)csStartPath, CSIDL_DESKTOP, 0);
				CString csLnkPath;
				csLnkPath.Format(_T("%s\\%s.lnk"), csStartPath, lnk_fname);
				USES_CONVERSION;
				LPCOLESTR lpOleStr = csLnkPath;
				pIPF->Save(lpOleStr, FALSE);
				pIPF->Release();
			}
			pisl->Release();
		}
		CoUninitialize();
	}







	memset(&m_nid, 0, sizeof(NOTIFYICONDATA));
	m_nid.cbSize = sizeof(NOTIFYICONDATA);
	m_nid.hWnd = m_hWnd;
	m_nid.uID = IDR_MAINFRAME;  // 图标ID
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_nid.uCallbackMessage = WM_USER + 100;  // 自定义消息ID
	m_nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);  // 加载图标

	// 设置提示文本
	_tcscpy_s(m_nid.szTip, _countof(m_nid.szTip), _T("Hhanx温度监测"));

	// 显示托盘图标
	ShowTrayIcon(TRUE);
	//m_bTrayIconVisible = TRUE;










	// 移除标题栏和尺寸调整边框样式
	LONG style = GetWindowLong(m_hWnd, GWL_STYLE);
	style &= ~(WS_CAPTION | WS_THICKFRAME);
	SetWindowLong(m_hWnd, GWL_STYLE, style);

	//// 移除最大化/最小化按钮
	//LONG exStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	//exStyle &= ~WS_EX_DLGMODALFRAME;
	////exStyle |= WS_EX_LAYERED;  // 启用分层窗口样式
	/*SetWindowLong(m_hWnd, GWL_EXSTYLE, exStyle);*/

	//// 禁用系统菜单中的最大化和调整大小命令
	//pSysMenu = GetSystemMenu(FALSE);
	//if (pSysMenu != NULL)
	//{
	//	pSysMenu->EnableMenuItem(SC_MAXIMIZE, MF_BYCOMMAND | MF_GRAYED);
	//	pSysMenu->EnableMenuItem(SC_SIZE, MF_BYCOMMAND | MF_GRAYED);
	//}

	// 设置窗口整体透明度（0-255，255为完全不透明）
	SetLayeredWindowAttributes(0,233, LWA_ALPHA);

	// 重新计算窗口大小
	//SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	CRect rect;
	m_DlgRect.SetRect(0, 0, 0, 0);

	//设置窗口大小
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);  // 屏幕宽度
	int screenHeight = GetSystemMetrics(SM_CYSCREEN); // 屏幕高度
	Size windowSize = calculateWindowSize(screenWidth, screenHeight);
	int width = windowSize.width;
	int height = windowSize.height;
	MoveWindow(screenWidth - width, screenHeight - height, width, height, TRUE);


	// 创建圆角区域
	GetClientRect(&rect);
	HRGN hRgn = CreateRoundRectRgn(0, 0, rect.Width(), rect.Height(), 10, 10); // 20是圆角半径

	// 应用圆角区域到窗口
	SetWindowRgn(hRgn, TRUE);

	// 释放区域资源（SetWindowRgn会自动获取区域所有权）
	DeleteObject(hRgn);

	// 创建背景画刷（例如浅灰色）
	m_titleBrush.CreateSolidBrush(RGB(240, 240, 240));




	//创建一个static对象
	m_static1.Create(_T("CPU : 0℃"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(5, 10, 135, 50), this, IDC_STATIC1);

	m_static2.Create(_T("GPU : 0℃"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(5, 40, 135, 70), this, IDC_STATIC2);

	CFont* pFont = GetFont();
	m_static1.SetFont(pFont);
	m_static2.SetFont(pFont);


	//启用高dpi

	//-------------------------------------------------------------------

	//创建一个线程 获取，显示一下数据
	std::thread th(thread_fun, this);
	th.detach();


	SetTimer(1, 100, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CMFCApplication1Dlg::repaint(UINT id, int last_Width, int now_Width, int last_Height, int now_Height)
{
	CRect rect;
	CWnd* wnd = NULL;
	wnd = GetDlgItem(id);
	if (NULL == wnd)
	{
		MessageBox(_T("相应控件不存在"));
	}
	wnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	double proportion_x = now_Width / (double)last_Width;
	double proportion_y = now_Height / (double)last_Height;

	rect.left = (long)(rect.left * proportion_x + 0.5);
	rect.right = (long)(rect.right * proportion_x + 0.5);
	rect.top = (long)(rect.top * proportion_y + 0.5);
	rect.bottom = (long)(rect.bottom * proportion_y + 0.5);
	wnd->MoveWindow(&rect);
}


HBRUSH CMFCApplication1Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	//// 判断是否为标题控件（根据ID）
	if (pWnd->GetDlgCtrlID() == IDC_STATIC1)
	{
		// 设置文本颜色（例如深蓝色）
		pDC->SetTextColor(RGB(255, 140, 0));

		// 返回背景画刷
		return m_titleBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC2)
	{
		// 设置文本颜色（例如深蓝色）
		pDC->SetTextColor(RGB(32, 158, 11));

		// 返回背景画刷
		return m_titleBrush;
	}

	return hbr;
}

void CMFCApplication1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 调用默认处理
	CDialogEx::OnLButtonDown(nFlags, point);

	// 发送系统消息以实现拖动
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
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




void CMFCApplication1Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
}

void CMFCApplication1Dlg::movewindowStyle() {
	CRect rect;
	GetWindowRect(&rect);
	int x = rect.left;
	int y = rect.top;
	int w = rect.Width();
	int h = rect.Height();

	//获取屏幕的宽高
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);  // 屏幕宽度
	int screenHeight = GetSystemMetrics(SM_CYSCREEN); // 屏幕高度
	if (x < 0)
	{
		int newX = x - w + 10;
		while (!(w < x+w)) {
			Sleep(10);
			x += 2;
			UpdateWindow();
			MoveWindow(x, y, w, h, TRUE);
		}

		KillTimer(2);
	}

	//横向
	if (x + w > screenWidth)
	{

		MoveWindow(x + w - 10, y, w, h, TRUE);
	}
	//纵向
	if (y + h > screenHeight)
	{
		MoveWindow(x, y + h - 10, w, h, TRUE);
	}

}

void CMFCApplication1Dlg::OnTimer(UINT_PTR nIDEvent)
{

	if(nIDEvent == 1)
	{
		//获取窗口的位置
		CRect rect;
		GetWindowRect(&rect);
		int x = rect.left;
		int y = rect.top;
		int w = rect.Width();
		int h = rect.Height();
		
		//获取屏幕的宽高
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);  // 屏幕宽度
		int screenHeight = GetSystemMetrics(SM_CYSCREEN); // 屏幕高度


		//如果窗口移动到了屏幕外面，就把它移动回来
		
		if (x < 0) 
		{
			x = 0;

			MoveWindow(x - w+ 10, y, w, h, TRUE);
			SetTimer(2, 10, NULL);

		}
		if (y < 0) {
			y = 0;
			MoveWindow(x, y-h/2, w, h, TRUE);

		}
		//横向
		if (x + w > screenWidth)
		{
			x = screenWidth - w;

			MoveWindow(x + w-10, y, w, h, TRUE);
		}
		//纵向
		if (y + h > screenHeight)
		{
			y = screenHeight - h;

			MoveWindow(x , y + h-10, w, h, TRUE);
			
		}

	}


	if(nIDEvent == 2)
	{
		CRect rect;
		GetWindowRect(&rect);
		int x = rect.left;
		int y = rect.top;
		int w = rect.Width();
		int h = rect.Height();

		POINT p;
		GetCursorPos(&p);
		if (p.x >= x && p.x <= x + w && p.y >= y && p.y <= y + h)
		{
			// 检查消息队列中是否有鼠标消息
			DWORD queueStatus = GetQueueStatus(QS_MOUSEBUTTON | QS_MOUSEMOVE);

			// 高字节表示最近添加的消息类型
			if (HIWORD(queueStatus) & (QS_MOUSEBUTTON))
			{
				// 有鼠标按钮消息
				MSG msg;
				if (PeekMessage(&msg, NULL, WM_LBUTTONDOWN, WM_RBUTTONDBLCLK, PM_REMOVE | PM_NOYIELD))
				{
					movewindowStyle();
				}
			}
			
		}
	}


	CDialogEx::OnTimer(nIDEvent);
}
