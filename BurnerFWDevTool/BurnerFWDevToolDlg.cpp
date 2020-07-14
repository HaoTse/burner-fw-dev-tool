
// BurnerFWDevToolDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "BurnerFWDevTool.h"
#include "BurnerFWDevToolDlg.h"
#include "afxdialogex.h"

#include <string>

#include "utils.h"

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
}

BEGIN_MESSAGE_MAP(CBurnerFWDevToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_DROPDOWN(DEVICE_LIST, &CBurnerFWDevToolDlg::OnCbnDropdownList)
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
		}
		SetDropDownHeight(&device_list_ctrl, drive_cnt);
	}
	catch (const exception& exp) {
		string msg = exp.what();
		MessageBox((LPCTSTR)CA2T(msg.c_str()), _T("Error"), MB_ICONERROR);
	}
}
