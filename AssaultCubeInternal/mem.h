#pragma once
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

namespace mem
{
	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE h_process);
	void NopEx(BYTE* dst, unsigned int size, HANDLE h_process);
}
