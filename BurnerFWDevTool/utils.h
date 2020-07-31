#pragma once

#include <vector>

#include "Device.h"

#define LSB_PAGE 1
#define CSB_PAGE 2
#define MSB_PAGE 3

using namespace std;

void get_write_pattern(UINT page_type, LPBYTE buf, UINT len);

int scan_physical_drive(vector<Device>& device_list, int limit_cnt = 32);

void SetDropDownHeight(CComboBox* pMyComboBox, int itemsToShow);