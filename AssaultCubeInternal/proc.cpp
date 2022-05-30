#include "proc.h"

/*This function attempts to find a process id based on a string for the process name
inputs:
- proc_name:	the name of a process as string
output:
- proc_id:		the DWORD (unsigned long) value of a process
*/
DWORD getProcID(const wchar_t* proc_name)
{
	/*Making a function parameter const enlists the compiler’s help to
	ensure that the parameter’s value is not changed inside the function*/
	//DWORD is defined as unsigned long
	DWORD proc_id = 0;
	
	//Takes a snapshot of the specified processes, as well as the heaps, modules, 
	//and threads used by these processes.
	//HANDLE is just a pointer to a void type
	HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	
	if (h_snap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 proc_entry;
		proc_entry.dwSize = sizeof(proc_entry);

		//Process32First retrieves information about the first process 
		//encountered in a system snapshot.
		if (Process32First(h_snap, &proc_entry))
		{
			do
			{	
				//_wcsicmp performs a case-insensitive comparison of strings
				//between const wchar_t *string1, *string2
				if (!_wcsicmp(proc_entry.szExeFile, proc_name))
				{
					proc_id = proc_entry.th32ProcessID;
					break;
				}
			} while (Process32Next(h_snap, &proc_entry));
		}
	}
	CloseHandle(h_snap);
	return proc_id;
}




/*This function attempts to find an executable or dll of a process using it's ID.
It is usually used for finding where an .exe is placed in memory, if we know the
process ID.
inputs:
- proc_id:	The ID of a procces to which a memory location is needed
- mod_name:	the module (.exe or .dll)
output:
- address:	the address located by going through a pointer chain
*/
uintptr_t getModuleBaseAddress(DWORD proc_id, const wchar_t* mod_name)
{
	/*as long as the program is compiled as x64 or x86 like the game,
	then uintptr_t works correctly and is assigned either 64 bits or 32 bits of memory
	respectively*/
	uintptr_t module_base_address = 0;
	HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id);
	if (h_snap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 mod_entry;
		mod_entry.dwSize = sizeof(mod_entry);
		if (Module32First(h_snap, &mod_entry))
		{
			do
			{
				if (!_wcsicmp(mod_entry.szModule, mod_name))
				{
					module_base_address = (uintptr_t)mod_entry.modBaseAddr;
					break;
				}
			} while (Module32Next(h_snap, &mod_entry)); 
		}
	}
	CloseHandle(h_snap);
	return module_base_address;
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
uintptr_t findDMAAddy(HANDLE hproc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t address = ptr;
	for (unsigned int i = 0; i < offsets.size(); i++)
	{
		ReadProcessMemory(hproc, (BYTE*)address, &address, sizeof(address), nullptr);
		address += offsets[i];
	}
	return address;
}
