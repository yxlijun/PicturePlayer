
// UserDlg.h : ͷ�ļ�
//

#pragma once

#include <vector>
// CUserDlg �Ի���
class CUserDlg : public CDialogEx
{
// ����
public:
	CUserDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_USER_DIALOG };

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
public:
	
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ReSize();
	POINT old;
	bool AutoOn;
	afx_msg void OnBnClickedHandOn();
	bool bFullScreen;
	WINDOWPLACEMENT m_struOldWndpl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedAutoOn();
};
