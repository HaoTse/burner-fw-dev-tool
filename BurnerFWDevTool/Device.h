#pragma once

class Device
{
private:
	UINT idx;
	char device_name[17] = { '\0' };
public:
	Device();
	Device(char idx);
	HANDLE openDevice();
	void set_name(char* name);
	CString showText();
};