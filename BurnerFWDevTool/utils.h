#pragma once

#include <vector>

#include "Device.h"

using namespace std;

int scan_physical_drive(vector<Device>& device_list, int limit_cnt = 32);

void SetDropDownHeight(CComboBox* pMyComboBox, int itemsToShow);