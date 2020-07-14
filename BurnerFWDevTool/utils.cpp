#include "pch.h"

#include "utils.h"
#include "scsi.h"

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

			if (!issue_SCSI(hDevice, cdb, cdb_length, buffer, buffer_size, FALSE)) {
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
