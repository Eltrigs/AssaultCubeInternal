#include "pch.h"
#include "mem.h"


/*This program rewrites a program's memory at specified bytes with custom information
inputs:
- dst:			the address where new bytes of data will be sent
- src:			the information that is going to be written
- size:			the size of the new information in bytes
- h_process:	the HANDLE for a process for which new memory information is written
*/
void mem::PatchEx(BYTE* dst, BYTE* src,unsigned int size, HANDLE h_process)
{
	DWORD oldprotect;
	VirtualProtectEx(h_process, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	WriteProcessMemory(h_process, dst, src, size, nullptr);
	VirtualProtectEx(h_process, dst, size, oldprotect, &oldprotect);
}


/*This program rewrites a program's memory at specified bytes with custom information
inputs:
- dst:			the address where new bytes of data will be sent
- size:			the size of the no operation (0x90 command) block in bytes
- h_process:	the HANDLE for a process for which new memory information is written
*/
void mem::NopEx(BYTE* dst, unsigned int size, HANDLE h_process)
{
	BYTE* nop_array = new BYTE[size];
	memset(nop_array, 0x90, size);
	PatchEx(dst, nop_array, size, h_process);
}


/*
DMA means Dynamic Memory Allocation.
inputs:
- hproc:	HANDLE to a process
- ptr:		base address of a module
- offsets:	offsets to find a specific memory location
output:
- address:	the address located by going through a pointer chain
*/
uintptr_t mem::findDMAAddyEx(HANDLE hproc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t address = ptr;
	for (unsigned int i = 0; i < offsets.size(); i++)
	{
		ReadProcessMemory(hproc, (BYTE*)address, &address, sizeof(address), nullptr);
		address += offsets[i];
	}
	return address;
}


/*This program rewrites a program's memory at specified bytes with custom information
inputs:
- dst:			the address where new bytes of data will be sent
- src:			the information that is going to be written
- size:			the size of the new information in bytes
*/
void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}


/*This program rewrites a program's memory at specified bytes with custom information
* internal version
inputs:
- dst:			the address where new bytes of data will be sent
- size:			the size of the no operation (0x90 command) block in bytes
*/
void mem::Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}


/*
DMA means Dynamic Memory Allocation.
inputs:
- hproc:	HANDLE to a process
- ptr:		base address of a module
- offsets:	offsets to find a specific memory location
output:
- address:	the address located by going through a pointer chain
*/
uintptr_t mem::findDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t address = ptr;
	for (unsigned int i = 0; i < offsets.size(); i++)
	{
		address = *(uintptr_t*)address;
		address += offsets[i];
	}
	return address;
}
