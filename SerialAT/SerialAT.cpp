// SerialAT.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
* To Send SMSs
Set the modem to text mode with the command AT+CMGF=1.
Enter the recipient’s number with the command AT+CMGS="<recipient_number>". Replace <recipient_number> with the actual phone number of the recipient.
Type your message. After the AT+CMGS command, you should be able to type your message1.
Send the message. After typing your message, send it by pressing Ctrl+Z

* To Read SMSs
This code sends the AT command AT+CMGF=1 to set the modem to text mode, then sends the AT command AT+CMGL="ALL" to list all SMS messages1. AT+CMGL=? to see all possible values.
*/

#include <windows.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include "SerialAT.h"

std::mutex writeMutex;
bool keepReading = true;

int WriteSerial(HANDLE hSerial, LPCVOID lpMessage, DWORD nNumberOfBytes, DWORD& bytes_written)
{
	if (!WriteFile(hSerial, lpMessage, nNumberOfBytes, &bytes_written, NULL))
	{
		fprintf(stderr, "Error writing to serial port.\n");

		CloseHandle(hSerial);
		return 1;
	}
	fprintf(stderr, "%d bytes written. Message: %s\n", bytes_written, (char*)lpMessage);

	return 0;
}

const int readBufSize = 1024;
char readSzBuff[readBufSize] = { 0 };
DWORD dwBytesRead = 0;
int readCount = 0;
int ReadSerial(HANDLE hSerial)
{
	if (!ReadFile(hSerial, readSzBuff, readBufSize, &dwBytesRead, NULL))
	{
		fprintf(stderr, "Error reading from serial port\n");

		CloseHandle(hSerial);
		return 1;
	}

	fprintf(stderr, "Response(%d): %s\n", ++readCount, readSzBuff);

	return 0;
}

void ReadSerialContinously(HANDLE hSerial)
{
	while (keepReading)
	{
		if (ReadSerial(hSerial) == 1)
		{
			keepReading = false;
		}
	}
}

int main(int argc, char* argv[])
{
	HANDLE hSerial;
	DCB dcbSerialParams = { 0 };
	COMMTIMEOUTS timeouts = { 0 };
	int retVal = -1;

	if (argc != 1 && argc != 3)
	{
		fprintf(stderr, "Improper usage, use 0 arguments for test, or 2 for sending a message");
	}

	std::string recipient;
	std::string message;
	if (argc == 1)
	{
		recipient = "+447874608569";
		message = "Test message!";
	}
	else
	{
		recipient = argv[1];
		message = argv[2];
	}


	// Open the highest available serial port number
	fprintf(stderr, "Opening serial port...");
	hSerial = CreateFile(L"\\\\.\\COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Error\n");
		return 1;
	}
	else fprintf(stderr, "OK\n");

	// Set device parameters (9600 baud, 1 start bit, 1 stop bit, no parity)
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hSerial, &dcbSerialParams) == 0)
	{
		fprintf(stderr, "Error getting device state\n");
		CloseHandle(hSerial);
		return 1;
	}
	fprintf(stderr, "Device state OK\n");

	dcbSerialParams.BaudRate = CBR_9600; // Changed baud rate to 9600
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (SetCommState(hSerial, &dcbSerialParams) == 0)
	{
		fprintf(stderr, "Error setting device parameters\n");
		CloseHandle(hSerial);
		return 1;
	}
	fprintf(stderr, "Device parameters set\n");

	// Set COM port timeout settings
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	if (SetCommTimeouts(hSerial, &timeouts) == 0)
	{
		fprintf(stderr, "Error setting timeouts\n");
		CloseHandle(hSerial);
		return 1;
	}
	fprintf(stderr, "Device timeouts set\n");

	//// Start the thread to read from the serial port
	//std::thread readThread(ReadSerialContinously, hSerial);
	//readThread.detach();

	// Set to SMS mode
	DWORD bytes_written = 0, total_bytes_written = 0;
	retVal = WriteSerial(hSerial, "AT+CMGF=1\r", 10, bytes_written);


	retVal = ReadSerial(hSerial);
	if (retVal == 1) return retVal;

	// Send the recipient's number
	std::string cmd = "AT+CMGS=\"" + recipient + "\"\r";
	retVal = WriteSerial(hSerial, cmd.c_str(), cmd.length(), bytes_written);


	retVal = ReadSerial(hSerial);
	if (retVal == 1) return retVal;

	// Type your message
	cmd = message + "\r";
	retVal = WriteSerial(hSerial, cmd.c_str(), cmd.length(), bytes_written);
	if (retVal == 1) return retVal;

	// Send the message ("Ctrl+Z" character)
	retVal = WriteSerial(hSerial, "\x1A", 1, bytes_written);
	if (retVal == 1) return retVal;

	retVal = ReadSerial(hSerial);
	if (retVal == 1) return retVal;

	keepReading = false;

	// Close serial port
	fprintf(stderr, "Closing serial port...");
	if (CloseHandle(hSerial) == 0)
	{
		fprintf(stderr, "Error\n");
		return 1;
	}
	fprintf(stderr, "OK\n");

	// exit normally
	return 0;
}


