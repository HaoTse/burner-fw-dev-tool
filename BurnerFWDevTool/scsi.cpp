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
	if ((!buffer && buffSize) || (buffer && !buffSize)) {
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

BOOL send_payload(HANDLE hDrive, LPBYTE payload_buf, UINT transf_len, UINT admin)
{
	UINT cdb_len = 16;
	LPBYTE cdb = new BYTE[cdb_len];
	RtlZeroMemory(cdb, cdb_len);

	cdb[0] = 0xa1; // operation code
	cdb[1] = (byte)((admin << 7) + 0); // ADMIN + PROTOCOL
	cdb[3] = (byte)((transf_len >> 16) & 0xff); // PARAMETER LIST LENGTH (23:16)
	cdb[4] = (byte)((transf_len >> 8) & 0xff);  // PARAMETER LIST LENGTH (23:16)
	cdb[5] = (byte)(transf_len & 0xff);			// PARAMETER LIST LENGTH (23:16)

	BOOL ret = issue_SCSI(hDrive, cdb, cdb_len, payload_buf, transf_len, SCSI_WRITE);
	delete[] cdb;

	return ret;
}

BOOL send_read_data(HANDLE hDrive, LPBYTE data_buf, UINT transf_len, UINT admin)
{
	UINT cdb_len = 16;
	LPBYTE cdb = new BYTE[cdb_len];
	RtlZeroMemory(cdb, cdb_len);

	cdb[0] = 0xa1; // operation code
	cdb[1] = (byte)((admin << 7) + 2); // ADMIN + PROTOCOL (DMA-IN)
	cdb[3] = (byte)((transf_len >> 16) & 0xff); // PARAMETER LIST LENGTH (23:16)
	cdb[4] = (byte)((transf_len >> 8) & 0xff);  // PARAMETER LIST LENGTH (23:16)
	cdb[5] = (byte)(transf_len & 0xff);			// PARAMETER LIST LENGTH (23:16)

	BOOL ret = issue_SCSI(hDrive, cdb, cdb_len, data_buf, transf_len, SCSI_READ);
	delete[] cdb;

	return ret;
}

BOOL send_non_data(HANDLE hDrive, UINT admin)
{
	UINT cdb_len = 16;
	LPBYTE cdb = new BYTE[cdb_len];
	RtlZeroMemory(cdb, cdb_len);

	cdb[0] = 0xa1; // operation code
	cdb[1] = (byte)((admin << 7) + 1); // ADMIN + PROTOCOL (Non-data)

	BOOL ret = issue_SCSI(hDrive, cdb, cdb_len, nullptr, 0, SCSI_READ);
	delete[] cdb;

	return ret;
}

BOOL send_return_response(HANDLE hDrive, LPBYTE response_buf, UINT transf_len, UINT admin)
{
	UINT cdb_len = 16;
	LPBYTE cdb = new BYTE[cdb_len];
	RtlZeroMemory(cdb, cdb_len);

	cdb[0] = 0xa1; // operation code
	cdb[1] = (byte)((admin << 7) + 15); // ADMIN + PROTOCOL (return response information)
	cdb[3] = (byte)((transf_len >> 16) & 0xff); // PARAMETER LIST LENGTH (23:16)
	cdb[4] = (byte)((transf_len >> 8) & 0xff);  // PARAMETER LIST LENGTH (23:16)
	cdb[5] = (byte)(transf_len & 0xff);			// PARAMETER LIST LENGTH (23:16)

	BOOL ret = issue_SCSI(hDrive, cdb, cdb_len, response_buf, transf_len, SCSI_READ);
	delete[] cdb;

	return ret;
}



