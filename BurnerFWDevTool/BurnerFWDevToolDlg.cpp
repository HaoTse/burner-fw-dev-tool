
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
	DDX_Control(pDX, PLANE_EDIT, plane_edit_ctrl);
	DDX_Control(pDX, PAGE_LIST, page_list_ctrl);
	DDX_Control(pDX, WL_EDIT, wl_edit_ctrl);
	DDX_Control(pDX, READ_PAGE_BTN, read_page_btn_ctrl);
	DDX_Control(pDX, WRITE_BUF_PAGE_LIST, write_buf_page_list_ctrl);
	DDX_Control(pDX, WRITE_BUF_EDIT, write_buf_edit_ctrl);
	DDX_Control(pDX, WRITE_BTN, write_btn_ctrl);
	DDX_Control(pDX, MODE_LIST, mode_list_ctrl);
	//  DDX_Control(pDX, TYPE_RADIO_GROUP, type_radio_ctrl);
	//  DDX_Text(pDX, TYPE_RADIO_GROUP, page_type_value);
}

BEGIN_MESSAGE_MAP(CBurnerFWDevToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_DROPDOWN(DEVICE_LIST, &CBurnerFWDevToolDlg::OnCbnDropdownList)
	ON_CBN_SELCHANGE(DEVICE_LIST, &CBurnerFWDevToolDlg::OnCbnSelchangeList)
	ON_CBN_SELCHANGE(WRITE_BUF_PAGE_LIST, &CBurnerFWDevToolDlg::OnCbnSelchangeBufPageList)
	ON_BN_CLICKED(SCAN_FLH_ID_BTN, &CBurnerFWDevToolDlg::OnBnClickedFlhIdBtn)
	ON_BN_CLICKED(ERASE_BTN, &CBurnerFWDevToolDlg::OnBnClickedEraseBtn)
	ON_BN_CLICKED(READ_PAGE_BTN, &CBurnerFWDevToolDlg::OnBnClickedReadPageBtn)
	ON_BN_CLICKED(WRITE_BTN, &CBurnerFWDevToolDlg::OnBnClickedWriteBtn)
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

	// set buffer display limit
	buf_result_edit_ctrl.SetLimitText(60000);
	write_buf_edit_ctrl.SetLimitText(60000);

	// set a Font with same width
	m_font.CreateStockObject(ANSI_FIXED_FONT);
	buf_result_edit_ctrl.SetFont(&m_font);
	write_buf_edit_ctrl.SetFont(&m_font);

	// set page list
	page_list_ctrl.InsertString(0, _T("Lower page"));
	page_list_ctrl.InsertString(1, _T("Middle page"));
	page_list_ctrl.InsertString(2, _T("Upper page"));
	SetDropDownHeight(&page_list_ctrl, 3);
	write_buf_page_list_ctrl.InsertString(0, _T("Lower page"));
	write_buf_page_list_ctrl.InsertString(1, _T("Middle page"));
	write_buf_page_list_ctrl.InsertString(2, _T("Upper page"));
	SetDropDownHeight(&write_buf_page_list_ctrl, 3);

	// set mode list
	mode_list_ctrl.InsertString(0, _T("PIO"));
	mode_list_ctrl.InsertString(1, _T("FPU"));
	mode_list_ctrl.InsertString(2, _T("MT"));
	SetDropDownHeight(&mode_list_ctrl, 3);

	// set default page type
	CButton* page_type_tlc = (CButton*)GetDlgItem(TYPE_TLC_RADIO);
	page_type_tlc->SetCheck(1);

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
	erase_btn_ctrl.EnableWindow(setup);
	read_page_btn_ctrl.EnableWindow(setup);
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

void CBurnerFWDevToolDlg::insert_write_pattern_edit(CString msg)
{
	write_buf_edit_ctrl.SetSel(-1, -1);
	write_buf_edit_ctrl.ReplaceSel(msg);
	write_buf_edit_ctrl.PostMessage(WM_VSCROLL, SB_BOTTOM, 0); // scroll location
}

void CBurnerFWDevToolDlg::show_buffer_result(LPBYTE buf, UINT len, UINT show_col_n)
{
	buf_result_edit_ctrl.SetWindowText(_T(""));
	UINT cur_idx = 0;
	CString str = _T(""), tmp;
	while (cur_idx < len)
	{
		tmp.Format(_T("%02x "), buf[cur_idx]);
		str += tmp;
		cur_idx++;

		if (!(cur_idx % show_col_n)) {
			str += _T("\n");
			insert_buffer_result_edit(str);
			str = _T("");
		}
	}

	if (!str.IsEmpty()) {
		str += _T("\n");
		insert_buffer_result_edit(str);
	}
}

void CBurnerFWDevToolDlg::show_write_pattern(LPBYTE buf, UINT len, UINT show_col_n)
{
	write_buf_edit_ctrl.SetWindowText(_T(""));
	UINT cur_idx = 0;
	CString str = _T(""), tmp;
	while (cur_idx < len)
	{
		tmp.Format(_T("%02x "), buf[cur_idx]);
		str += tmp;
		cur_idx++;

		if (!(cur_idx % show_col_n)) {
			str += _T("\n");
			insert_write_pattern_edit(str);
			str = _T("");
		}
	}

	if (!str.IsEmpty()) {
		str += _T("\n");
		insert_write_pattern_edit(str);
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


void CBurnerFWDevToolDlg::OnCbnSelchangeBufPageList()
{
	DWORD selected_mode = mode_list_ctrl.GetCurSel();
	if (selected_mode == CB_ERR) {
		setup_btns(FALSE);
		MessageBox(_T("Please select mode."), _T("Error"), MB_ICONERROR);
		return;
	}

	// empty write buf
	write_buf_edit_ctrl.SetWindowText(_T(""));
	// check if select
	DWORD selected_wpage_idx = write_buf_page_list_ctrl.GetCurSel();
	if (selected_wpage_idx == CB_ERR) {
		return;
	}

	// get write buf parrtern
	UINT len = (selected_mode == 0) ? 18336 : 16384;
	LPBYTE write_buf = new BYTE[len];
	get_write_pattern(selected_wpage_idx + 1, write_buf, len);

	show_write_pattern(write_buf, len);

	delete[] write_buf;
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
	DWORD selected_ce, selected_blk, selected_plane, selected_mode, slc_mode;

	DWORD selected_device_idx = device_list_ctrl.GetCurSel();
	if (selected_device_idx == CB_ERR) {
		setup_btns(FALSE);
		MessageBox(_T("Please select a device."), _T("Error"), MB_ICONERROR);
		return;
	}
	
	selected_mode = mode_list_ctrl.GetCurSel();
	if (selected_mode == CB_ERR) {
		setup_btns(FALSE);
		MessageBox(_T("Please select mode."), _T("Error"), MB_ICONERROR);
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
		MessageBox(_T("Must input plane."), _T("Error"), MB_ICONERROR);
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

	UpdateData(TRUE);
	slc_mode = (GetCheckedRadioButton(TYPE_TLC_RADIO, TYPE_SLC_RADIO) == TYPE_TLC_RADIO) ? 0 : 1;
	if (selected_mode == 2) {
		selected_mode += slc_mode;
	}

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
	if (!issue_Erase(hDrive, selected_mode, selected_ce, selected_blk)) {
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

	buf_result_edit_ctrl.SetWindowText(_T(""));
	if (selected_mode == 0 || selected_mode == 1) { // only store status in PIO and FPU mode
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
	}

	msg.Format(_T("End %s.\n"), cur_op);
	insert_msg_edit(msg);
	msg.Format(_T("%s finished.\n"), cur_op); 
	MessageBox(msg, _T("Information"), MB_ICONINFORMATION);
	CloseHandle(hDrive);
}


void CBurnerFWDevToolDlg::OnBnClickedReadPageBtn()
{
	// check value
	CString tmp;
	DWORD selected_ce, selected_blk, selected_plane, selected_wl, selected_page, selected_mode, slc_mode;

	DWORD selected_device_idx = device_list_ctrl.GetCurSel();
	if (selected_device_idx == CB_ERR) {
		setup_btns(FALSE);
		MessageBox(_T("Please select a device."), _T("Error"), MB_ICONERROR);
		return;
	}

	selected_mode = mode_list_ctrl.GetCurSel();
	if (selected_mode == CB_ERR) {
		setup_btns(FALSE);
		MessageBox(_T("Please select mode."), _T("Error"), MB_ICONERROR);
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
		MessageBox(_T("Must input plane."), _T("Error"), MB_ICONERROR);
		return;
	}
	selected_plane = _ttoi(tmp);

	wl_edit_ctrl.GetWindowText(tmp);
	if (tmp.IsEmpty()) {
		MessageBox(_T("Must input wordline."), _T("Error"), MB_ICONERROR);
		return;
	}
	selected_wl = _ttoi(tmp);

	selected_page = page_list_ctrl.GetCurSel();
	if (selected_page == CB_ERR) {
		MessageBox(_T("Must select page."), _T("Error"), MB_ICONERROR);
		return;
	}

	if (selected_plane >= 2) {
		MessageBox(_T("Only 2 plane per LUN."), _T("Error"), MB_ICONERROR);
		return;
	}
	if (selected_blk > 989) {
		MessageBox(_T("The block number is 990."), _T("Error"), MB_ICONERROR);
		return;
	}
	if (selected_wl >= 384) {
		MessageBox(_T("The wl number is 384."), _T("Error"), MB_ICONERROR);
		return;
	}
	selected_blk = selected_blk * 2 + selected_plane;

	UpdateData(TRUE);
	slc_mode = (GetCheckedRadioButton(TYPE_TLC_RADIO, TYPE_SLC_RADIO) == TYPE_TLC_RADIO) ? 0 : 1;
	if (selected_mode == 2) {
		selected_mode += slc_mode;
	}

	if (selected_mode == 0) {
		selected_wl = (selected_wl << 2) + (selected_page);
	}
	else if (slc_mode == 0 || selected_mode == 1) {
		selected_wl = (selected_wl * 3) + (selected_page);
	}
	else {
		selected_page = 0;
	}

	/*
	 * read page
	 */
	CString cur_op = _T("Read Page"), msg;
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

	// issue read page
	UINT buf_len;
	if (selected_mode == 0) { // PIO mode
		buf_len = 18336;
	}
	else {
		buf_len = 16384;
	}
	LPBYTE read_buf = new BYTE[buf_len];
	if (!issue_Read_Page(hDrive, selected_mode, selected_ce, selected_blk, selected_wl, read_buf, buf_len)) {
		msg.Format(_T("\t%s failed.\n"), cur_op);
		insert_msg_edit(msg);
		msg.Format(_T("End %s.\n"), cur_op);
		insert_msg_edit(msg);
		msg.Format(_T("%s failed.\n"), cur_op);
		MessageBox(msg, _T("Error"), MB_ICONERROR);
		CloseHandle(hDrive);
		delete[] read_buf;
		return;
	}
	insert_msg_edit(_T("\tRead page finished.\n"));

	// show read buffer
	UINT err_byte, err_bit;
	msg.Format(_T("\tShow read buffer (%d bytes).\n"), buf_len);
	insert_msg_edit(msg);
	show_buffer_result(read_buf, buf_len, 32);
	if (diff_page_pattern(selected_page + 1, read_buf, buf_len, &err_byte, &err_bit)) {
		insert_msg_edit(_T("\tNo error byte.\n"));
	}
	else {
		msg.Format(_T("\tError byte: %d.\n"), err_byte);
		insert_msg_edit(msg);
		msg.Format(_T("\tError bit: %d.\n"), err_bit);
		insert_msg_edit(msg);
	}
	delete[] read_buf;

	msg.Format(_T("End %s.\n"), cur_op);
	insert_msg_edit(msg);
	msg.Format(_T("%s succeed.\n"), cur_op);
	MessageBox(msg, _T("Information"), MB_ICONINFORMATION);
	CloseHandle(hDrive);
}


void CBurnerFWDevToolDlg::OnBnClickedWriteBtn()
{
	// check value
	CString tmp;
	DWORD selected_ce, selected_blk, selected_plane, selected_wl, selected_mode, slc_mode;

	DWORD selected_device_idx = device_list_ctrl.GetCurSel();
	if (selected_device_idx == CB_ERR) {
		setup_btns(FALSE);
		MessageBox(_T("Please select a device."), _T("Error"), MB_ICONERROR);
		return;
	}

	selected_mode = mode_list_ctrl.GetCurSel();
	if (selected_mode == CB_ERR) {
		setup_btns(FALSE);
		MessageBox(_T("Please select mode."), _T("Error"), MB_ICONERROR);
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
		MessageBox(_T("Must input plane."), _T("Error"), MB_ICONERROR);
		return;
	}
	selected_plane = _ttoi(tmp);

	wl_edit_ctrl.GetWindowText(tmp);
	if (tmp.IsEmpty()) {
		MessageBox(_T("Must input wordline."), _T("Error"), MB_ICONERROR);
		return;
	}
	selected_wl = _ttoi(tmp);

	if (selected_plane >= 2) {
		MessageBox(_T("Only 2 plane per LUN."), _T("Error"), MB_ICONERROR);
		return;
	}
	if (selected_blk > 989) {
		MessageBox(_T("The block number is 990."), _T("Error"), MB_ICONERROR);
		return;
	}
	if (selected_wl >= 384) {
		MessageBox(_T("The wl number is 384."), _T("Error"), MB_ICONERROR);
		return;
	}
	selected_blk = selected_blk * 2 + selected_plane;

	UpdateData(TRUE);
	slc_mode = (GetCheckedRadioButton(TYPE_TLC_RADIO, TYPE_SLC_RADIO) == TYPE_TLC_RADIO) ? 0 : 1;
	if (selected_mode == 2) {
		selected_mode += slc_mode;
	}
	else {
		slc_mode = 0;
	}

	/*
	 * write
	 */
	CString cur_op = _T("Write"), msg;
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

	// issue write
	UINT page_len;
	if (selected_mode == 0) { // PIO mode
		page_len = 18336;
	}
	else {
		page_len = 16384;
	}
	UINT data_buf_len = (slc_mode) ? page_len : page_len * 3;
	LPBYTE write_buf = new BYTE[data_buf_len];
	get_write_pattern(LSB_PAGE, write_buf, page_len);
	if (slc_mode == 0) {
		get_write_pattern(CSB_PAGE, write_buf + page_len, page_len);
		get_write_pattern(MSB_PAGE, write_buf + page_len * 2, page_len);
	}
	if (!issue_Write(hDrive, selected_mode, selected_ce, selected_blk, selected_wl, write_buf, data_buf_len)) {
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
	delete[] write_buf;

	buf_result_edit_ctrl.SetWindowText(_T(""));
	if (selected_mode == 0 || selected_mode == 1) { // only store status in PIO and FPU mode
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
		UINT status_buf_len = 32;
		LPBYTE status_buf = new BYTE[status_buf_len];
		if (!issue_Read_Status(hDrive, status_buf, status_buf_len)) {
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
		show_buffer_result(status_buf, status_buf_len);
		delete[] status_buf;
	}

	msg.Format(_T("End %s.\n"), cur_op);
	insert_msg_edit(msg);
	msg.Format(_T("%s succeed.\n"), cur_op);
	MessageBox(msg, _T("Information"), MB_ICONINFORMATION);
	CloseHandle(hDrive);
}


