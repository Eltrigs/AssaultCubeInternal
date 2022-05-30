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
