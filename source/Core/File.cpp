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
	}
}

bool FileReader::Open(cstring filename)
{
	assert(filename);
	file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	own_handle = true;
	return (file != INVALID_HANDLE_VALUE);
}

bool FileReader::Read(void* ptr, uint size)
{
	ReadFile(file, ptr, size, &tmp, nullptr);
	return size == tmp;
}

void FileReader::ReadToString(string& s)
{
	DWORD size = GetFileSize(file, nullptr);
	s.resize(size);
	ReadFile(file, (char*)s.c_str(), size, &tmp, nullptr);
	assert(size == tmp);
}

void FileReader::Skip(uint bytes)
{
	SetFilePointer(file, bytes, nullptr, FILE_CURRENT);
}

bool FileReader::Ensure(uint size)
{

}

uint FileReader::GetSize() const
{
	return GetFileSize(file, nullptr);
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
