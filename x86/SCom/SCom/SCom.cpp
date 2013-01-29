// SCom.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>
#include <tchar.h>

#include "ComPort.h"

#include <iostream>

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	CComPort comPort;

	comPort.Open(_T("COM7"));
	comPort.SetBaudRate(115200);

	// do somethimg interesting here
	std::string data;
	do {
		// read character from the input
		cout << "Enter a string to send to COM port ('q' to exit): ";
		cin >> data;

		// try to send it to the COM port
		if (!comPort.SendData(reinterpret_cast<const BYTE*>(data.c_str()),
			static_cast<DWORD>(data.size()))) {
			cout << "Failed to send a byte to the COM port" << endl;
			break;
		}

		// try to get one byte from the COM port
		char tmp[80] = {0};
		if (!comPort.RecvData(reinterpret_cast<BYTE*>(tmp), 80)) {
			cout << "Failed to receive a byte from the COM port" << endl;
			break;
		}

		// write the received character to the output
		cout << "Character '" << tmp << "' has been received from the COM port" << endl;

	} while ('q' != data.c_str()[0]);

	cout << "Close COM port" << endl;
	comPort.Close();

	return 0;
}
