#pragma once

//-----------------------------------------------------------------------------
typedef void* HANDLE;
const HANDLE INVALID_HANDLE_VALUE = (HANDLE)(IntPointer)-1;

//-----------------------------------------------------------------------------
class FileReader
{
public:
	FileReader() : file(INVALID_HANDLE_VALUE), own_handle(false)
	{
	}

	explicit FileReader(HANDLE file) : file(file), own_handle(false)
	{
	}

	explicit FileReader(cstring filename) : own_handle(true)
	{
		Open(filename);
	}

	~FileReader();

	bool Open(cstring filename);
	bool Read(void* ptr, uint size);

	bool IsOpen() const
	{
		return file != INVALID_HANDLE_VALUE;
	}

	operator bool() const
	{
		return file != INVALID_HANDLE_VALUE;
	}

	template<typename T>
	bool operator >> (T& a)
	{
		return Read(&a, sizeof(a));
	}

	template<typename T>
	T Read()
	{
		T a;
		Read(&a, sizeof(T));
		return a;
	}

	template<typename T>
	bool Read(T& a)
	{
		return Read(&a, sizeof(a));
	}

	template<typename T, typename T2>
	bool ReadCasted(T2& a)
	{
		T b;
		if(!Read<T>(b))
			return false;
		a = (T2)b;
		return true;
	}

	bool ReadStringBUF()
	{
		byte len = Read<byte>();
		if(len == 0)
		{
			BUF[0] = 0;
			return true;
		}
		else
		{
			BUF[len] = 0;
			return Read(BUF, len);
		}
	}

	template<typename T>
	void Skip()
	{
		SetFilePointer(file, sizeof(T), nullptr, FILE_CURRENT);
	}

	void Skip(int bytes)
	{
		SetFilePointer(file, bytes, nullptr, FILE_CURRENT);
	}

	bool ReadString1(string& s)
	{
		byte len;
		if(!Read(len))
			return false;
		s.resize(len);
		return Read((char*)s.c_str(), len);
	}

	bool ReadString2(string& s)
	{
		word len;
		if(!Read(len))
			return false;
		s.resize(len);
		return Read((char*)s.c_str(), len);
	}

	bool operator >> (string& s)
	{
		return ReadString1(s);
	}

	void ReadToString(string& s)
	{
		DWORD size = GetFileSize(file, nullptr);
		s.resize(size);
		ReadFile(file, (char*)s.c_str(), size, &tmp, nullptr);
		assert(size == tmp);
	}

	template<typename T>
	void ReadVector1(vector<T>& v)
	{
		byte count;
		Read(count);
		v.resize(count);
		if(count)
			Read(&v[0], sizeof(T)*count);
	}

	template<typename T>
	void ReadVector2(vector<T>& v)
	{
		word count;
		Read(count);
		v.resize(count);
		if(count)
			Read(&v[0], sizeof(T)*count);
	}

	uint GetSize() const
	{
		return GetFileSize(file, nullptr);
	}

	HANDLE file;
	bool own_handle;
};

//-----------------------------------------------------------------------------
class FileWriter
{
public:
	FileWriter() : file(INVALID_HANDLE_VALUE), own_handle(true)
	{
	}

	explicit FileWriter(HANDLE file) : file(file), own_handle(false)
	{
	}

	explicit FileWriter(cstring filename) : own_handle(true)
	{
		Open(filename);
	}

	~FileWriter()
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
		file = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		return (file != INVALID_HANDLE_VALUE);
	}

	bool IsOpen() const
	{
		return file != INVALID_HANDLE_VALUE;
	}

	void Write(const void* ptr, uint size)
	{
		WriteFile(file, ptr, size, &tmp, nullptr);
		assert(size == tmp);
	}

	template<typename T>
	void operator << (const T& a)
	{
		Write(&a, sizeof(a));
	}

	template<typename T>
	void Write(const T& a)
	{
		Write(&a, sizeof(a));
	}

	template<typename T, typename T2>
	void WriteCasted(const T2& a)
	{
		Write(&a, sizeof(T));
	}

	void WriteString1(const string& s)
	{
		int length = s.length();
		assert(length < 256);
		WriteCasted<byte>(length);
		if(length)
			Write(s.c_str(), length);
	}

	void WriteString1(cstring str)
	{
		assert(str);
		int length = strlen(str);
		assert(length < 256);
		WriteCasted<byte>(length);
		if(length)
			Write(str, length);
	}

	void WriteString2(const string& s)
	{
		int length = s.length();
		assert(length < 256 * 256);
		WriteCasted<word>(length);
		if(length)
			Write(s.c_str(), length);
	}

	void WriteString2(cstring str)
	{
		assert(str);
		int length = strlen(str);
		assert(length < 256 * 256);
		Write<word>(length);
		if(length)
			Write(str, length);
	}

	void operator << (const string& s)
	{
		WriteString1(s);
	}

	void operator << (cstring str)
	{
		assert(str);
		WriteString1(str);
	}

	operator bool() const
	{
		return file != INVALID_HANDLE_VALUE;
	}

	void Write0()
	{
		WriteCasted<byte>(0);
	}

	template<typename T>
	void WriteVector1(const vector<T>& v)
	{
		WriteCasted<byte>(v.size());
		if(!v.empty())
			Write(&v[0], sizeof(T)*v.size());
	}

	template<typename T>
	void WriteVector2(const vector<T>& v)
	{
		WriteCasted<word>(v.size());
		if(!v.empty())
			Write(&v[0], sizeof(T)*v.size());
	}

	void Flush()
	{
		FlushFileBuffers(file);
	}

	uint GetSize() const
	{
		return GetFileSize(file, nullptr);
	}

	HANDLE file;
	bool own_handle;
};
