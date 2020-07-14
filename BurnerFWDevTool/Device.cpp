#include "pch.h"

#include <WinIoCtl.h>
#include <Ntddscsi.h>

#include "Device.h"

Device::Device()
{
	this->idx = NULL;
}

Device::Device(char idx)
{
	this->idx = idx;
}

HANDLE Device::openDevice()
{
	char device_path[25];

	// initial handle
	sprintf_s(device_path, "\\\\.\\PhysicalDrive%u", this->idx);
	HANDLE hDevice = CreateFileA(device_path,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	/*if (hDevice == INVALID_HANDLE_VALUE) {
		TRACE("\n[Error] Open %c: fail. Error Code = %u\n", this->ident, GetLastError());
	}*/

	return hDevice;
}

void Device::set_name(char* name)
{
	memcpy(this->device_name, name, 16);
}

CString Device::showText()
{
	// convert ascii to unicode
	size_t len = strlen(this->device_name);
	wchar_t uni_name[35]; // ascii length (17) * 2 + 1
	MultiByteToWideChar(CP_OEMCP, 0, this->device_name, -1, uni_name, 35);

	CString text;
	text.Format(_T("%s"), uni_name);

	return text;
}
