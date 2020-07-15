#pragma once

#include <Windows.h>
#include <WinIoCtl.h>
#include <Ntddscsi.h>

#define SCSI_WRITE TRUE
#define SCSI_READ FALSE
#define SCSI_TIMEOUT 40

LPBYTE get_inquiry_cdb_command();

BOOL issue_SCSI(HANDLE hDrive, LPBYTE Cdb, UINT CdbLen, LPBYTE buffer, UINT buffSize, BOOLEAN is_write);

BOOL send_payload(HANDLE hDrive, LPBYTE payload_buf, UINT transf_len, UINT admin);

BOOL send_read_data(HANDLE hDrive, LPBYTE data_buf, UINT transf_len, UINT admin);

BOOL send_non_data(HANDLE hDrive, UINT admin);

BOOL send_return_response(HANDLE hDrive, LPBYTE response_buf, UINT transf_len, UINT admin);