#include "pch.h"
#include "scsi.h"


LPBYTE get_inquiry_cdb_command() {
	LPBYTE cdb = new BYTE[16];
	RtlZeroMemory(cdb, 16);

	cdb[0] = 0x12; // inquiry opcode
	cdb[4] = 0x38;

	return cdb;
}


BOOL issue_SCSI(HANDLE hDrive, LPBYTE Cdb, UINT CdbLen, LPBYTE buffer, UINT buffSize, BOOLEAN is_write)
{
	SCSI_PASS_THROUGH_DIRECT srb = { 0 };	// SCSI Request Block Structure
	DWORD bytesReturned = 0;				// Number of bytes returned

	BOOL retVal = 0;
	DWORD lastErr = 0;

	// value check
	if (!buffer || !buffSize) {
		TRACE("\n[Error] Buffer setup error.\n");
		return FALSE;
	}
	if (!Cdb || !CdbLen) {
		TRACE("\n[Error] Cdb setup error.\n");
		return FALSE;
	}

	// Inizialize common SCSI_PASS_THROUGH_DIRECT members 
	RtlZeroMemory(&srb, sizeof(SCSI_PASS_THROUGH_DIRECT));
	srb.Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
	srb.PathId = 0;
	srb.TargetId = 1;
	srb.Lun = 0;
	srb.SenseInfoLength = 0;
	srb.SenseInfoOffset = sizeof(SCSI_PASS_THROUGH_DIRECT);
	if (is_write)
		srb.DataIn = SCSI_IOCTL_DATA_OUT;
	else
		srb.DataIn = SCSI_IOCTL_DATA_IN;
	srb.TimeOutValue = SCSI_TIMEOUT;

	srb.DataTransferLength = buffSize;
	srb.DataBuffer = buffer;
	
	srb.CdbLength = CdbLen;
	for (UINT i = 0; i < CdbLen; i++) {
		srb.Cdb[i] = Cdb[i];
	}

	retVal = DeviceIoControl(
		hDrive,
		IOCTL_SCSI_PASS_THROUGH_DIRECT,
		(LPVOID)&srb,
		sizeof(SCSI_PASS_THROUGH_DIRECT),
		NULL,
		0,
		&bytesReturned,
		NULL);

	if (!retVal) {
		// Errore 1: ERROR_INVALID_FUNCTION
		TRACE("\n[Error] SCSI command fail. Error code = %u.\n", GetLastError());
		return FALSE;
	}
	else
		return TRUE;
}
