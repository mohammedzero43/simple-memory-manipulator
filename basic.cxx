#include <iostream>
#include <Windows.h>
#include <winerror.h>
using namespace std;

int main()
{
	int newVal = 10000;
	int readval = 10;

	HWND  hwnd = FindWindowA(NULL, "Step 2");
	if (hwnd == NULL) {
		cout << "Cannot find window." << "endl";
		Sleep(3000);
		exit(-1);
	}
	else {
		cout << "windowFound\n";
		DWORD procID; 
		GetWindowThreadProcessId(hwnd, &procID);

		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, procID);
		if (procID == NULL)
		{
			cout << "Cannot optain process ID." << "endl";
			Sleep(3000);
			exit(-1);
		}
		else {

			cout << "cheat activated\n";
			WriteProcessMemory(handle, (LPVOID)0x016D0EC8, (LPCVOID)&newVal, sizeof(newVal), 0);

			
			ReadProcessMemory(handle, (PBYTE*)0x016D0EC8 , &readval, sizeof(readval),0);
			cout << readval <<endl;

			
		}
		Sleep(5000);
	}

	return 0;