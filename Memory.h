#pragma once
#include <Windows.h>
#include <memoryapi.h>
#include <TlHelp32.h>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

typedef unsigned __int8 __i8;		//	1Byte
typedef unsigned __int16 __i16;		//	2Bytes
typedef unsigned __int32 __i32;		//	4Bytes
typedef unsigned __int64 __i64;		//	8Bytes

struct PCSX2PROCESSINFO64
{
	HANDLE hProc{ INVALID_HANDLE_VALUE };		//	handle to process
	DWORD dwProcID{ 0 };						//	process id
	__i64 uBaseAddress{ 0 };					//	process module base address -> IMAGE_DOS_HEADER
	__i64 uEEmem{ 0 };							//	pcsx2 game module base address
}; typedef PCSX2PROCESSINFO64 pcsx2_t;
inline pcsx2_t g_pcsx;	//	static pointer to process information

//	External Memory Class
class PCSX2Memory
{
private:
	bool ResolveProcess(const std::string& pName, pcsx2_t& pInfo);

public:
	bool bValidInstance{ false };

public:

	//	returns the virtual address of an exported name from the export address table
	static __i64 GetProcAddressEx(const pcsx2_t& moduleInfo, const std::string& name, __i64& result);
	
	//	returns eemem + offset
	static __i64 GetAddr(const __i32& offset);

	//	reads a continguous string in memory
	static bool ReadString(const __i64& addr, std::string& result);

	//	reads a chain of offsets and returns the result
	static bool ReadPointerChain(const __i64& addr, const std::vector<__i32>& offsets, __i64& result);

	//	template static methods
public:
	template<typename T>
	T Read(const __i64& addr) noexcept
	{
		if (g_pcsx.hProc == INVALID_HANDLE_VALUE)
			return T{};

		T result{};
		ReadProcessMemory(g_pcsx.hProc, (void*)addr, &result, sizeof(T), nullptr);
		return result;
	}

	template<typename T>
	bool Write(const __i64& addr, const T& value)
	{
		if (g_pcsx.hProc == INVALID_HANDLE_VALUE)
			return false;

		SIZE_T bytesWritten;
		return WriteProcessMemory(g_pcsx.hProc, (void*)addr, &value, sizeof(T), &bytesWritten) && bytesWritten == sizeof(T);
	}

	//	constructors
public:
	explicit PCSX2Memory();
	PCSX2Memory(const std::string& processName);
	~PCSX2Memory();
};