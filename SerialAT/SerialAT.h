#pragma once

int ReadSerial(HANDLE hSerial);

int WriteSerial(HANDLE hSerial, LPCVOID lpMessage, DWORD nNumberOfBytes, DWORD& bytes_written);

static wchar_t* prefixComPort(char* comPort);
