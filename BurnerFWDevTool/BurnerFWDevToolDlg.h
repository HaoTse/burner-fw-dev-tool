
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
	void setup_btns(BOOL setup);
	void insert_msg_edit(CString msg);
	void insert_buffer_result_edit(CString msg);
	void insert_write_pattern_edit(CString msg);
	void show_buffer_result(LPBYTE buf, UINT len, UINT show_col_n = 16);
	void show_write_pattern(LPBYTE buf, UINT len, UINT show_col_n = 32);
public:
	CFont m_font;
	CComboBox device_list_ctrl;
	CButton scan_flh_id_btn_ctrl;
	CEdit msg_edit_ctrl;
	CEdit buf_result_edit_ctrl;
	CEdit ce_edit_ctrl;
	CEdit block_edit_ctrl;
	CEdit plane_edit_ctrl;
	CEdit wl_edit_ctrl;
	CEdit write_buf_edit_ctrl;
	CComboBox page_list_ctrl;
	CComboBox write_buf_page_list_ctrl;
	CComboBox mode_list_ctrl;
	CButton erase_btn_ctrl;
	CButton read_page_btn_ctrl;
	CButton write_btn_ctrl;
//	CStatic type_radio_ctrl;
//	int page_type_value;
	afx_msg void OnCbnDropdownList();
	afx_msg void OnCbnSelchangeList();
	afx_msg void OnCbnSelchangeBufPageList();
	afx_msg void OnBnClickedFlhIdBtn();
	afx_msg void OnBnClickedEraseBtn();
	afx_msg void OnBnClickedReadPageBtn();
	afx_msg void OnBnClickedWriteBtn();
};
