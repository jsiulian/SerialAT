#pragma once

int readSerial(HANDLE hSerial);

int writeSerial(HANDLE hSerial, LPCVOID lpMessage, DWORD nNumberOfBytes, DWORD& bytes_written);

static wchar_t* prefixComPort(char* comPort);
