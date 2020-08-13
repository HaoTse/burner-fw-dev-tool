#pragma once

#include "scsi.h"

BOOL issue_AP_Key_Set(HANDLE hDrive);

BOOL issue_Scan_Flh_ID(HANDLE hDrive, LPBYTE read_data_buf, UINT read_len);

BOOL issue_Read_Status(HANDLE hDrive, LPBYTE read_data_buf, UINT read_len);

BOOL issue_Erase(HANDLE hDrive, UINT mode, UINT ce, UINT blk);

BOOL issue_Read_Page(HANDLE hDrive, UINT mode, UINT ce, UINT blk, UINT page, LPBYTE read_data_buf, UINT read_len, UINT cache_num = 0);

BOOL issue_Write(HANDLE hDrive, UINT mode, UINT ce, UINT blk, UINT page, LPBYTE write_data_buf, UINT write_len, UINT cache_num = 0);
