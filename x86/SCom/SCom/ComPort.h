#pragma once

#include <Windows.h>

#include <tchar.h>
#include <string>

#define COM_PORT_NAME_MAX_LENGTH sizeof(_T("COM0"))

class CComPort
{
public:
	// Retrieves a list of all COM port devices in the system
	static std::string& Enumerate(void);

	CComPort(void);
	virtual ~CComPort(void);

	BOOL Open(const _TCHAR* portName);
	BOOL Close(void);
	
	BOOL SetBaudRate(DWORD baudRate);
	inline DWORD GetBaudRate(void) { return dcb.BaudRate; };

	// API's to send/receive one byte
	BOOL SendByte(BYTE byte);
	BOOL RecvByte(BYTE* byte);

	// API's to send/receive more data at once
	DWORD SendData(const BYTE* data, DWORD size);
	DWORD RecvData(BYTE* data, DWORD size);

private:
	HANDLE hCom; // COM port hanle
	_TCHAR name[COM_PORT_NAME_MAX_LENGTH]; // COM port name
	DCB    dcb; // COM port params
	DWORD  baudRate; // COM port baud rate

	CComPort(const CComPort&);
	CComPort& operator = (const CComPort&);
};
