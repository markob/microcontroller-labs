#include "StdAfx.h"
#include "ComPort.h"

#pragma warning(disable:4996)

CComPort::CComPort(void):
	hCom(INVALID_HANDLE_VALUE)
{
}


CComPort::~CComPort(void)
{
	if (INVALID_HANDLE_VALUE != hCom) {
		Close();
	}
}


BOOL CComPort::Open(const _TCHAR* portName)
{
	hCom = CreateFile(portName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == hCom) {
		return true;
	}

	// store port name
	_tcscpy(name, portName);

	// get COP port params
	GetCommState(hCom, &dcb);

	return false;
}


BOOL CComPort::Close(void)
{
	BOOL rv = CloseHandle(hCom);
	if (rv) {
		hCom = INVALID_HANDLE_VALUE;
	}
	return !rv;
}


BOOL CComPort::SetBaudRate(DWORD baudRate)
{
	if (baudRate != dcb.BaudRate) {
		DWORD prevBaudRate = dcb.BaudRate;
		dcb.BaudRate = baudRate;

		if (!SetCommState(hCom, &dcb)) {
			dcb.BaudRate = prevBaudRate;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CComPort::SendByte(BYTE byte)
{
	BOOL rv = TRUE;
	DWORD bytesWritten = 0;

	rv = WriteFile(hCom, &byte, sizeof(byte), &bytesWritten, NULL);

	return rv;
}


BOOL CComPort::RecvByte(BYTE* byte)
{
	BOOL rv = TRUE;
	DWORD bytesRead = 0;

	rv = ReadFile(hCom, byte, sizeof(BYTE), &bytesRead, NULL);

	return rv;
}


DWORD CComPort::SendData(const BYTE* data, DWORD size)
{
	BOOL rv = TRUE;
	DWORD bytesWritten = 0;

	rv = WriteFile(hCom, data, size, &bytesWritten, NULL);

	return bytesWritten;
}


DWORD CComPort::RecvData(BYTE* data, DWORD size)
{
	BOOL rv = TRUE;
	DWORD bytesRead = 0;

	rv = ReadFile(hCom, data, size, &bytesRead, NULL);

	return bytesRead;
}
