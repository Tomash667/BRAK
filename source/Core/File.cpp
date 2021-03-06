#include "Pch.h"
#include "Core.h"
#include "File.h"
#include <Windows.h>

static DWORD tmp;


FileReader::~FileReader()
{
	if(own_handle && file != INVALID_HANDLE_VALUE)
	{
		CloseHandle(file);
		file = INVALID_HANDLE_VALUE;
		ok = false;
	}
}

bool FileReader::Open(cstring filename)
{
	assert(filename);
	file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	own_handle = true;
	if(file != INVALID_HANDLE_VALUE)
	{
		size = GetFileSize(file, nullptr);
		ok = true;
	}
	else
	{
		size = 0;
		ok = false;
	}
	return ok;
}

bool FileReader::Read(void* ptr, uint size)
{
	ReadFile(file, ptr, size, &tmp, nullptr);
	ok = (size == tmp);
	return ok;
}

void FileReader::ReadToString(string& s)
{
	DWORD size = GetFileSize(file, nullptr);
	s.resize(size);
	ReadFile(file, (char*)s.c_str(), size, &tmp, nullptr);
	assert(size == tmp);
}

bool FileReader::Skip(uint bytes)
{
	ok = (ok && SetFilePointer(file, bytes, nullptr, FILE_CURRENT) != INVALID_SET_FILE_POINTER);
	return ok;
}

uint FileReader::GetPos() const
{
	return (uint)SetFilePointer(file, 0, nullptr, FILE_CURRENT);
}


FileWriter::~FileWriter()
{
	if(own_handle && file != INVALID_HANDLE_VALUE)
	{
		CloseHandle(file);
		file = INVALID_HANDLE_VALUE;
	}
}

bool FileWriter::Open(cstring filename)
{
	assert(filename);
	file = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	return (file != INVALID_HANDLE_VALUE);
}

void FileWriter::Write(const void* ptr, uint size)
{
	WriteFile(file, ptr, size, &tmp, nullptr);
	assert(size == tmp);
}

void FileWriter::Flush()
{
	FlushFileBuffers(file);
}

uint FileWriter::GetSize() const
{
	return GetFileSize(file, nullptr);
}
