#include <stdio.h>
#include <tchar.h>

#include <cstdlib>
#include <iostream>

#include <vector>
#include <Windows.h>
#include <TlHelp32.h>


DWORD GetProcId(const wchar_t* procName);
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
uintptr_t FindDMAAddr(HANDLE hproc, uintptr_t baseptr, std::vector<unsigned int> offsets);

int main() {
	//wchar_t ProcName = (wchar_t)L"The Ghassala Effect.exe";

	//get proc ID
	DWORD procId = GetProcId(L"The Ghassala Effect.exe");
	std::cout << "Proc ID : " << std::hex << procId<< std::endl;

	// get module base addr
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"UnityPlayer.dll");
	std::cout <<"moduleBase : " <<moduleBase<< std::endl;


	// get handle 
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, procId);


	//Resolve base addr
	uintptr_t rel_offset = 0x015F2ED0; //getfromcheatengine 
	uintptr_t dynPtrBaseAddr = moduleBase +  rel_offset;
	std::cout << "PointerBase : " <<dynPtrBaseAddr << std::endl;

	//Resolve Offsets 
	std::vector <unsigned int> offsets = { 0x100,0x28,0x58,0xB0};
	uintptr_t value_addr = FindDMAAddr(hProcess, dynPtrBaseAddr, offsets);
	std::cout << "ValueAddress : " <<  value_addr << std::endl;
	//read_val
	float val_toread = 0.0;	
	std::cout <<"reading output : " << ReadProcessMemory(hProcess, (BYTE*)value_addr, &val_toread, sizeof(val_toread), nullptr);
	std::cout << std::endl << "Value = " << val_toread << std::endl;

	//HackProcess
	


	//write_val
	//float val_towrite = 9999;
	//#WriteProcessMemory(hProcess, (BYTE*)value_addr, &val_towrite, sizeof(val_towrite), nullptr);
	getchar();
	return 0;

	while (GetAsyncKeyState(VK_SPACE)) {
		Sleep(100);
	}
}



DWORD GetProcId(const wchar_t* procName) {
	DWORD procId = 0;
	HANDLE hsnap = (CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
	if (hsnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32W procentry;
		procentry.dwSize = sizeof(procentry);

		if (Process32FirstW(hsnap, &procentry)) {
			do {
				if (!_wcsicmp(procentry.szExeFile, procName)) {
					procId = procentry.th32ProcessID;
					break;
				}
			} while (Process32NextW(hsnap, &procentry));
		}
	}
	CloseHandle(hsnap);
	return procId;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {
	uintptr_t modBaseAddr = 0;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hsnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32W modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32FirstW(hsnap, &modEntry)) {
			do {
				if (!_wcsicmp(modEntry.szModule, modName)) {
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32NextW(hsnap, &modEntry));
		}
	}
	CloseHandle(hsnap);
	return modBaseAddr;
}

uintptr_t FindDMAAddr(HANDLE hproc, uintptr_t baseptr, std::vector<unsigned int> offsets) {
	uintptr_t addr = baseptr;
	for (unsigned int i = 0; i < offsets.size(); ++i) {
		ReadProcessMemory(hproc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}
