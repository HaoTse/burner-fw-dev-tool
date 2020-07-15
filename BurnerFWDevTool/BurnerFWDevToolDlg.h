
// BurnerFWDevToolDlg.h : header file
//

#pragma once

#include <vector>

#include "Device.h"

using namespace std;

// CBurnerFWDevToolDlg dialog
class CBurnerFWDevToolDlg : public CDialogEx
{
// Construction
public:
	CBurnerFWDevToolDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BURNERFWDEVTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	vector<Device> device_list;
	Device selected_device = NULL;
	void set_msg_edit(CString msg);
	void setup_btns(BOOL setup);
public:
	CComboBox device_list_ctrl;
	CButton scan_flh_id_btn_ctrl;
	CEdit msg_edit_ctrl;
	afx_msg void OnCbnDropdownList();
	afx_msg void OnCbnSelchangeList();
	afx_msg void OnBnClickedFlhIdBtn();
};
