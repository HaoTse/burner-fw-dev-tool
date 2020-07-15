#pragma once

#include "scsi.h"

BOOL issue_AP_Key_Set(HANDLE hDrive);

BOOL issue_Scan_Flh_ID(HANDLE hDrive, LPBYTE read_data_buf, UINT read_len);
