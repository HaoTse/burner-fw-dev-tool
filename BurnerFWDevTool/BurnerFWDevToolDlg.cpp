
// BurnerFWDevToolDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "BurnerFWDevTool.h"
#include "BurnerFWDevToolDlg.h"
#include "afxdialogex.h"

#include <string>

#include "utils.h"
#include "cmd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBurnerFWDevToolDlg dialog



CBurnerFWDevToolDlg::CBurnerFWDevToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BURNERFWDEVTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBurnerFWDevToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, DEVICE_LIST, device_list_ctrl);
	DDX_Control(pDX, SCAN_FLH_ID_BTN, scan_flh_id_btn_ctrl);
	DDX_Control(pDX, MSG_EDIT, msg_edit_ctrl);
	DDX_Control(pDX, BUF_RESULT_EDIT, buf_result_edit_ctrl);
	DDX_Control(pDX, CE_EDIT, ce_edit_ctrl);
	DDX_Control(pDX, BLOCK_EDIT, block_edit_ctrl);
	DDX_Control(pDX, ERASE_BTN, erase_btn_ctrl);
	DDX_Control(pDX, PAGE_EDIT, page_edit_ctrl);
	DDX_Control(pDX, PLANE_EDIT, plane_edit_ctrl);
}

BEGIN_MESSAGE_MAP(CBurnerFWDevToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_DROPDOWN(DEVICE_LIST, &CBurnerFWDevToolDlg::OnCbnDropdownList)
	ON_CBN_SELCHANGE(DEVICE_LIST, &CBurnerFWDevToolDlg::OnCbnSelchangeList)
	ON_BN_CLICKED(SCAN_FLH_ID_BTN, &CBurnerFWDevToolDlg::OnBnClickedFlhIdBtn)
	ON_BN_CLICKED(ERASE_BTN, &CBurnerFWDevToolDlg::OnBnClickedEraseBtn)
END_MESSAGE_MAP()


// CBurnerFWDevToolDlg message handlers

BOOL CBurnerFWDevToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	setup_btns(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBurnerFWDevToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBurnerFWDevToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBurnerFWDevToolDlg::setup_btns(BOOL setup)
{
	scan_flh_id_btn_ctrl.EnableWindow(setup);
}


void CBurnerFWDevToolDlg::insert_msg_edit(CString msg)
{
	msg_edit_ctrl.SetSel(-1, -1);
	msg_edit_ctrl.ReplaceSel(msg);
	msg_edit_ctrl.PostMessage(WM_VSCROLL, SB_BOTTOM, 0); // scroll location
}

void CBurnerFWDevToolDlg::insert_buffer_result_edit(CString msg)
{
	buf_result_edit_ctrl.SetSel(-1, -1);
	buf_result_edit_ctrl.ReplaceSel(msg);
	buf_result_edit_ctrl.PostMessage(WM_VSCROLL, SB_BOTTOM, 0); // scroll location
}

void CBurnerFWDevToolDlg::show_buffer_result(LPBYTE buf, UINT len)
{
	buf_result_edit_ctrl.SetWindowText(_T(""));
	for (UINT i = 0; i < (len >> 4); i++) {
		CString str = _T(""), tmp;
		for (UINT j = 0; j < 16; j++) {
			UINT cur_idx = (i << 4) + j;
			tmp.Format(_T("%02x "), buf[cur_idx]);
			str += tmp;
		}
		str += _T("\n");
		insert_buffer_result_edit(str);
	}
}


void CBurnerFWDevToolDlg::OnCbnDropdownList()
{
	// empty device list
	device_list_ctrl.ResetContent();
	// reset device_list
	for (vector<Device>::iterator iter = device_list.begin(); iter != device_list.end(); ) {
		iter = device_list.erase(iter);
	}
	vector<Device>().swap(device_list);

	// set device combo box
	try {
		int drive_cnt = scan_physical_drive(device_list);
		if (drive_cnt <= 0) {
			MessageBox(_T("No device found."), _T("Error"), MB_ICONERROR);
		}
		else {
			for (int i = 0; i < drive_cnt; i++) {
				Device cur_device = device_list.at(i);
				device_list_ctrl.InsertString(i, cur_device.showText());
			}
			SetDropDownHeight(&device_list_ctrl, drive_cnt);
		}
	}
	catch (const exception& exp) {
		string msg = exp.what();
		MessageBox((LPCTSTR)CA2T(msg.c_str()), _T("Error"), MB_ICONERROR);
	}
}


void CBurnerFWDevToolDlg::OnCbnSelchangeList()
{
	// check if select device
	DWORD selected_device_idx = device_list_ctrl.GetCurSel();
	if (selected_device_idx == CB_ERR) {
		setup_btns(FALSE);
		MessageBox(_T("Please select a device."), _T("Error"), MB_ICONERROR);
		return;
	}

	setup_btns(TRUE);
	selected_device = device_list.at(selected_device_idx);
}


void CBurnerFWDevToolDlg::OnBnClickedFlhIdBtn()
{
	// check if select device
	DWORD selected_device_idx = device_list_ctrl.GetCurSel();
	if (selected_device_idx == CB_ERR) {
		setup_btns(FALSE);
		MessageBox(_T("Please select a device."), _T("Error"), MB_ICONERROR);
		return;
	}

	/*
	 * scan flash id
	 */
	insert_msg_edit(_T("Start scan flash id.\n"));
	HANDLE hDrive = selected_device.openDevice();

	// issue AP key
	if (!issue_AP_Key_Set(hDrive)) {
		insert_msg_edit(_T("\tAP Key Set failed.\n"));
		insert_msg_edit(_T("End scan flash id.\n"));
		MessageBox(_T("Scan flash id failed."), _T("Error"), MB_ICONERROR);
		CloseHandle(hDrive);
		return;
	}
	insert_msg_edit(_T("\tAP Key Set finished.\n"));

	// issue scan flash id
	UINT buf_len = 512;
	LPBYTE flh_id_buf = new BYTE[buf_len];
	if (!issue_Scan_Flh_ID(hDrive, flh_id_buf, buf_len)) {
		insert_msg_edit(_T("\tScan flash ID failed.\n"));
		insert_msg_edit(_T("End scan flash id.\n"));
		MessageBox(_T("Scan flash id failed."), _T("Error"), MB_ICONERROR);
		CloseHandle(hDrive);
		return;
	}
	insert_msg_edit(_T("\tScan flash ID finished.\n"));

	// show flash id buffer
	show_buffer_result(flh_id_buf, buf_len);
	delete[] flh_id_buf;

	insert_msg_edit(_T("End scan flash id.\n"));
	MessageBox(_T("Scan flash id succeed."), _T("Information"), MB_ICONINFORMATION);
	CloseHandle(hDrive);

}



void CBurnerFWDevToolDlg::OnBnClickedEraseBtn()
{
	// check value
	CString tmp;
	DWORD selected_ce, selected_blk, selected_plane;
	DWORD selected_device_idx = device_list_ctrl.GetCurSel();
	if (selected_device_idx == CB_ERR) {
		setup_btns(FALSE);
		MessageBox(_T("Please select a device."), _T("Error"), MB_ICONERROR);
		return;
	}

	ce_edit_ctrl.GetWindowText(tmp);
	if (tmp.IsEmpty()) {
		MessageBox(_T("Must input CE."), _T("Error"), MB_ICONERROR);
		return;
	}
	selected_ce = _ttoi(tmp);
	
	block_edit_ctrl.GetWindowText(tmp);
	if (tmp.IsEmpty()) {
		MessageBox(_T("Must input block."), _T("Error"), MB_ICONERROR);
		return;
	}
	selected_blk = _ttoi(tmp);

	plane_edit_ctrl.GetWindowText(tmp);
	if (tmp.IsEmpty()) {
		MessageBox(_T("Must input block."), _T("Error"), MB_ICONERROR);
		return;
	}
	selected_plane = _ttoi(tmp);

	if (selected_plane >= 2) {
		MessageBox(_T("Only 2 plane per LUN."), _T("Error"), MB_ICONERROR);
		return;
	}
	if (selected_blk > 989) {
		MessageBox(_T("The block number is 989."), _T("Error"), MB_ICONERROR);
		return;
	}
	selected_blk = selected_blk * 2 + selected_plane;

	/*
	 * erase
	 */
	CString cur_op = _T("Erase"), msg;
	msg.Format(_T("Start %s.\n"), cur_op);
	insert_msg_edit(msg);
	HANDLE hDrive = selected_device.openDevice();

	// issue AP key
	if (!issue_AP_Key_Set(hDrive)) {
		insert_msg_edit(_T("\tAP Key Set failed.\n"));
		msg.Format(_T("End %s.\n"), cur_op);
		insert_msg_edit(msg);
		msg.Format(_T("%s failed.\n"), cur_op);
		MessageBox(msg, _T("Error"), MB_ICONERROR);
		CloseHandle(hDrive);
		return;
	}
	insert_msg_edit(_T("\tAP Key Set finished.\n"));

	// issue erase
	if (!issue_Erase(hDrive, selected_ce, selected_blk)) {
		msg.Format(_T("\t%s failed.\n"), cur_op);
		insert_msg_edit(msg);
		msg.Format(_T("End %s.\n"), cur_op);
		insert_msg_edit(msg);
		msg.Format(_T("%s failed.\n"), cur_op);
		MessageBox(msg, _T("Error"), MB_ICONERROR);
		CloseHandle(hDrive);
		return;
	}
	msg.Format(_T("\t%s finished.\n"), cur_op);
	insert_msg_edit(msg);

	// issue AP key
	if (!issue_AP_Key_Set(hDrive)) {
		insert_msg_edit(_T("\tAP Key Set failed.\n"));
		msg.Format(_T("End %s.\n"), cur_op);
		insert_msg_edit(msg);
		msg.Format(_T("%s failed.\n"), cur_op);
		MessageBox(msg, _T("Error"), MB_ICONERROR);
		CloseHandle(hDrive);
		return;
	}
	insert_msg_edit(_T("\tAP Key Set finished.\n"));

	// issue read status
	UINT buf_len = 32;
	LPBYTE status_buf = new BYTE[buf_len];
	if (!issue_Read_Status(hDrive, status_buf, buf_len)) {
		insert_msg_edit(_T("\tRead status failed.\n"));
		msg.Format(_T("End %s.\n"), cur_op);
		insert_msg_edit(msg);
		msg.Format(_T("%s failed.\n"), cur_op);
		MessageBox(msg, _T("Error"), MB_ICONERROR);
		CloseHandle(hDrive);
		return;
	}
	insert_msg_edit(_T("\tShow 32byte status.\n"));

	// show status buffer
	show_buffer_result(status_buf, buf_len);
	delete[] status_buf;

	msg.Format(_T("End %s.\n"), cur_op);
	insert_msg_edit(msg);
	msg.Format(_T("%s finished.\n"), cur_op); 
	MessageBox(msg, _T("Information"), MB_ICONINFORMATION);
	CloseHandle(hDrive);
}
