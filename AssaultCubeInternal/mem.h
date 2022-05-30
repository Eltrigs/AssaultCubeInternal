#pragma once
#include "pch.h"

namespace mem
{
	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE h_process);
	void NopEx(BYTE* dst, unsigned int size, HANDLE h_process);
	uintptr_t findDMAAddyEx(HANDLE hproc, uintptr_t ptr, std::vector<unsigned int> offsets);

	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void Nop(BYTE* dst, unsigned int size);
	uintptr_t findDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
}

