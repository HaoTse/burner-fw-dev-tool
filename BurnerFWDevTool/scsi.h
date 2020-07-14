#pragma once

#include <Windows.h>
#include <WinIoCtl.h>
#include <Ntddscsi.h>

#define SCSI_WRITE TRUE
#define SCSI_READ FALSE
#define SCSI_TIMEOUT 0x0200

LPBYTE get_inquiry_cdb_command();

BOOL issue_SCSI(HANDLE hDrive, LPBYTE Cdb, UINT CdbLen, LPBYTE buffer, UINT buffSize, BOOLEAN is_write);
