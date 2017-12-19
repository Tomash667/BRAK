#include "Pch.h"
#include "Core.h"
#include "File.h"
#define WIN32_LEAN_AND_MEAN

~FileReader()
{
	if(own_handle && file != INVALID_HANDLE_VALUE)
	{
		CloseHandle(file);
		file = INVALID_HANDLE_VALUE;
	}
}

bool Open(cstring filename)
{
	assert(filename);
	file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	own_handle = true;
	return (file != INVALID_HANDLE_VALUE);
}

bool Read(void* ptr, uint size)
{
	ReadFile(file, ptr, size, &tmp, nullptr);
	return size == tmp;
}
