#include "pch.h"

#include "utils.h"
#include "scsi.h"

void get_write_pattern(UINT page_type, LPBYTE buf, UINT len)
{
	if (page_type == LSB_PAGE) {
		for (UINT i = 0; i < len; i++) {
			buf[i] = (BYTE)(0xaa);
		}
		/*for (UINT i = 0; i < len; i++) {
			buf[i] = (BYTE)(i & 0xFF);
		}*/
	}
	else if (page_type == CSB_PAGE) {
		for (UINT i = 0; i < (len >> 1); i++) {
			buf[i << 1] = (BYTE)(i & 0xFF);
			buf[(i << 1) + 1] = (BYTE)((i >> 8) & 0xFF);
		}
		if (len & 1) {
			buf[len - 1] = (BYTE)((len >> 1) & 0xFF);
		}
	}
	else {
		for (UINT i = 0; i < len; i++) {
			buf[i] = 0x55;
			/*if (i < (len >> 1)) buf[i] = 0xff;
			else buf[i] = 0x00;*/
		}
		/*for (UINT i = 0; i < (len >> 1); i++) {
			buf[i << 1] = (BYTE)((0xFF - i) & 0xFF);
			buf[(i << 1) + 1] = (BYTE)(((0xFF - i) >> 8) & 0xFF);
		}
		if (len & 1) {
			buf[len - 1] = (BYTE)((len >> 1) & 0xFF);
		}*/
	}
}

DWORD count_bits(DWORD n)
{
	if (n == 0)
		return 0;
	else
		return (n & 1) + count_bits(n >> 1);
}

BOOL diff_page_pattern(UINT page_type, LPBYTE read_buf, UINT page_len, UINT* err_byte, UINT* err_bit)
{
	UINT byte_cnt = 0, bit_cnt = 0;
	BOOL rtn = TRUE;
	LPBYTE pattern = new BYTE[page_len];
	get_write_pattern(page_type, pattern, page_len);

	for (UINT i = 0; i < page_len; i++) {
		BYTE diff = pattern[i] ^ read_buf[i];
		if (diff != 0) {
			byte_cnt++;
			bit_cnt += count_bits(diff);
			rtn = FALSE;
		}
	}

	*err_byte = byte_cnt;
	*err_bit = bit_cnt;
	delete[] pattern;

	return rtn;
}

int scan_physical_drive(vector<Device>& device_list, int limit_cnt)
{
	int device_cnt = 0;

	int i = 0;
	for (int i = 0; i < limit_cnt; i++)
	{
		// get device capacity
		Device tmp_device(i);
		HANDLE hDevice = tmp_device.openDevice();
		if (hDevice == INVALID_HANDLE_VALUE) {
			continue;
		}
		else {
			UINT cdb_length = 16, buffer_size = 64;
			LPBYTE cdb = get_inquiry_cdb_command();
			LPBYTE buffer = new BYTE[buffer_size];

			if (!issue_SCSI(hDevice, cdb, cdb_length, buffer, buffer_size, SCSI_READ)) {
				delete[] cdb;
				delete[] buffer;
				CString err_msg;
				err_msg.Format(_T("Driver %u: SCSI inquiry failed. Error Code = %u"), i, GetLastError());
				throw err_msg;
			}

			// get driver name
			char driver_name[16];
			memcpy(driver_name, buffer + 8, 16);
			if (!memcmp(driver_name, "Bridge", 6)) {
				tmp_device.set_name(driver_name);
				device_list.push_back(tmp_device);
				device_cnt++;
			}

			delete[] cdb;
			delete[] buffer;
			CloseHandle(hDevice);
		}
	}

	return device_cnt;
}

void SetDropDownHeight(CComboBox* pMyComboBox, int itemsToShow)
{
	// Get rectangles    
	CRect rctComboBox, rctDropDown;
	pMyComboBox->GetClientRect(&rctComboBox); // Combo rect    
	pMyComboBox->GetDroppedControlRect(&rctDropDown); // DropDownList rect   
	int itemHeight = pMyComboBox->GetItemHeight(-1); // Get Item height   
	pMyComboBox->GetParent()->ScreenToClient(&rctDropDown); // Converts coordinates    
	rctDropDown.bottom = rctDropDown.top + rctComboBox.Height() + itemHeight * itemsToShow; // Set height   
	pMyComboBox->MoveWindow(&rctDropDown); // enable changes  
}
