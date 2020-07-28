#include "pch.h"
#include "cmd.h"

BOOL issue_AP_Key_Set(HANDLE hDrive)
{
	/*
	 * non-data
	 */
	UINT transf_len = 512;
	BYTE* transf_buf = new BYTE[transf_len];
	RtlZeroMemory(transf_buf, transf_len);

	transf_buf[0] = (byte)'N';
	transf_buf[1] = (byte)'V';
	transf_buf[2] = (byte)'M';
	transf_buf[3] = (byte)'E';
	transf_buf[8] = 0xd0; // non-data opcode
	transf_buf[0x3c] = 0x6f; // 0x3c = 60d (CDW13 - AP_KEY_PARAMETER used)
	transf_buf[0x3d] = 0xfe;
	transf_buf[0x3e] = 0xef;
	transf_buf[0x3f] = 0xfa;
	transf_buf[0x46] = 0x91; // 0x46 = 70d (CDW15[2:3] - CRC)
	transf_buf[0x47] = 0xd2;

	BOOL ret = FALSE;
	do {
		// step 1 - NVM Command Set Payload
		if (!send_payload(hDrive, transf_buf, transf_len, 1)) {
			TRACE("\n[Error] AP KEY step 1 failed.\n");
			break;
		}
		// step 2 - non-data
		if (!send_non_data(hDrive, 1)) {
			TRACE("\n[Error] AP KEY step 2 failed.\n");
			break;
		}
		// step 3 - return response information
		RtlZeroMemory(transf_buf, transf_len);
		if (!send_return_response(hDrive, transf_buf, transf_len, 1)) {
			TRACE("\n[Error] AP KEY step 3 failed.\n");
			break;
		}
		ret = TRUE;
	} while (0);

	delete[] transf_buf;

	return ret;
}

BOOL issue_Scan_Flh_ID(HANDLE hDrive, LPBYTE read_data_buf, UINT read_len)
{
	/*
	 * data out (read data)
	 */
	UINT transf_len = 512;
	BYTE* transf_buf = new BYTE[transf_len];
	RtlZeroMemory(transf_buf, transf_len);

	transf_buf[0] = (byte)'N';
	transf_buf[1] = (byte)'V';
	transf_buf[2] = (byte)'M';
	transf_buf[3] = (byte)'E';
	transf_buf[8] = 0xd2; // data-out opcode
	transf_buf[0x30] = 0x80; // 0x30 = 48d (CDW10 - Ndt)
	transf_buf[0x38] = 0x90; // 0x38 = 56d (CDW12 - feature & sub-feature)
	//transf_buf[0x38] = 0xe3; // HT define
	//transf_buf[0x39] = 0xe0; // HT define
	transf_buf[0x46] = 0xf7; // CDW15[2:3] - CRC
	transf_buf[0x47] = 0xd4;

	BOOL ret = FALSE;
	do {
		// step 1 - NVM Command Set Payload
		if (!send_payload(hDrive, transf_buf, transf_len, 1)) {
			TRACE("\n[Error] Scan flash id step 1 failed.\n");
			break;
		}
		// step 2 - data-out
		if (!send_read_data(hDrive, read_data_buf, read_len, 1)) {
			TRACE("\n[Error] Scan flash id step 2 failed.\n");
			break;
		}
		// step 3 - return response information
		RtlZeroMemory(transf_buf, transf_len);
		if (!send_return_response(hDrive, transf_buf, transf_len, 1)) {
			TRACE("\n[Error] Scan flash id step 3 failed.\n");
			break;
		}
		ret = TRUE;
	} while (0);

	delete[] transf_buf;
	return ret;
}

BOOL issue_Read_Status(HANDLE hDrive, LPBYTE read_data_buf, UINT read_len)
{
	/*
	 * data out (read data)
	 */
	UINT transf_len = 512;
	BYTE* transf_buf = new BYTE[transf_len];
	RtlZeroMemory(transf_buf, transf_len);

	transf_buf[0] = (byte)'N';
	transf_buf[1] = (byte)'V';
	transf_buf[2] = (byte)'M';
	transf_buf[3] = (byte)'E';
	transf_buf[8] = 0xd2; // data-out opcode
	transf_buf[0x30] = 0x80; // CDW10 - Ndt
	transf_buf[0x38] = 0xe0; // CDW12 - feature & sub-feature
	transf_buf[0x46] = 0xf7; // CRC
	transf_buf[0x47] = 0xd4;

	BOOL ret = FALSE;
	do {
		// step 1 - NVM Command Set Payload
		if (!send_payload(hDrive, transf_buf, transf_len, 1)) {
			TRACE("\n[Error] Read status step 1 failed.\n");
			break;
		}
		// step 2 - data-out
		if (!send_read_data(hDrive, read_data_buf, read_len, 1)) {
			TRACE("\n[Error] Read status step 2 failed.\n");
			break;
		}
		// step 3 - return response information
		RtlZeroMemory(transf_buf, transf_len);
		if (!send_return_response(hDrive, transf_buf, transf_len, 1)) {
			TRACE("\n[Error] Read status step 3 failed.\n");
			break;
		}
		ret = TRUE;
	} while (0);

	delete[] transf_buf;
	return ret;
}

BOOL issue_Erase(HANDLE hDrive, UINT ce, UINT blk)
{
	/*
	 * non-data
	 */
	UINT transf_len = 512;
	BYTE* transf_buf = new BYTE[transf_len];
	RtlZeroMemory(transf_buf, transf_len);

	transf_buf[0] = (byte)'N';
	transf_buf[1] = (byte)'V';
	transf_buf[2] = (byte)'M';
	transf_buf[3] = (byte)'E';
	transf_buf[8] = 0xd0; // non-data opcode
	transf_buf[0x38] = 0xe9; // CDW12 - feature
	transf_buf[0x40] = (byte)(blk & 0xff); // CDW14[1:0] - block
	transf_buf[0x41] = (byte)((blk >> 8) & 0xff);
	transf_buf[0x43] = (byte)(ce & 0xff); // CDW14[3] - ce
	transf_buf[0x46] = 0xf7; // CDW15[2:3] - CRC
	transf_buf[0x47] = 0xd4;

	BOOL ret = FALSE;
	do {
		// step 1 - NVM Command Set Payload
		if (!send_payload(hDrive, transf_buf, transf_len, 1)) {
			TRACE("\n[Error] Erase step 1 failed.\n");
			break;
		}
		// step 2 - non-data
		if (!send_non_data(hDrive, 1)) {
			TRACE("\n[Error] Erase step 2 failed.\n");
			break;
		}
		// step 3 - return response information
		RtlZeroMemory(transf_buf, transf_len);
		if (!send_return_response(hDrive, transf_buf, transf_len, 1)) {
			TRACE("\n[Error] Erase step 3 failed.\n");
			break;
		}
		ret = TRUE;
	} while (0);

	delete[] transf_buf;
	return ret;
}
