
// MFCApplication1Dlg.h: 头文件
//

#pragma once


// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();

	afx_msg void OnTrayExit();
	afx_msg LRESULT OnDpiChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	
	
private:
		NOTIFYICONDATA m_nid;    // 托盘图标数据结构

public:
	CBrush m_titleBrush;
	virtual ~CMFCApplication1Dlg();
	void ShowTrayIcon(BOOL bShow = TRUE); // 显示/隐藏托盘图标

	CStatic m_static1;
	CStatic m_static2;


	void movewindowStyle();


	CRect m_DlgRect;
	void repaint(UINT id, int last_Width, int now_Width, int last_Height, int now_Height);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
