#pragma once
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

DWORD getProcID(const wchar_t* proc_name);

uintptr_t getModuleBaseAddress(DWORD proc_id, const wchar_t* mod_name);

uintptr_t findDMAAddy(HANDLE hproc, uintptr_t ptr, std::vector<unsigned int> offsets);

