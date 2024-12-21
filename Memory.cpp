#include "Memory.h"

PCSX2Memory::PCSX2Memory() { bValidInstance = ResolveProcess("pcsx2-qt.exe", g_pcsx); }

PCSX2Memory::PCSX2Memory(const std::string& name) { bValidInstance = ResolveProcess(name, g_pcsx); }

PCSX2Memory::~PCSX2Memory() {}

bool PCSX2Memory::ResolveProcess(const std::string& pName, pcsx2_t& pInfo)
{
	pInfo = pcsx2_t();

	std::wstring key = std::wstring(pName.begin(), pName.end());
	std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return std::tolower(c); });
	if (!key.size())
		return false;

	///	GET PROCESS ID
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
		return false;

	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	if (!Process32First(hSnap, &pEntry))
	{
		CloseHandle(hSnap);
		return false;
	}

	bool bFound{ false };
	do
	{
		auto name = std::wstring(pEntry.szExeFile);
		std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {return std::tolower(c); });
		if (name != key)
			continue;

		bFound = true;
		pInfo.dwProcID = pEntry.th32ProcessID;

	} while (Process32Next(hSnap, &pEntry));
	CloseHandle(hSnap);

	if (!bFound || !pInfo.dwProcID)
	{
		pInfo = pcsx2_t();
		return false;
	}

	///	GET PROCESS MODULE BASE
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pInfo.dwProcID);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		pInfo = pcsx2_t();
		return false;
	}

	MODULEENTRY32 me32;
	me32.dwSize = sizeof(me32);
	if (!Module32First(hSnap, &me32))
	{
		pInfo = pcsx2_t();
		CloseHandle(hSnap);
		return false;
	}
	CloseHandle(hSnap);	//	first module is PCSX2

	pInfo.uBaseAddress = (long long)me32.modBaseAddr;	//	module base address

	///	OBTAIN HANDLE TO PROCESS
	pInfo.hProc = OpenProcess(PROCESS_ALL_ACCESS, false, g_pcsx.dwProcID);
	if (pInfo.hProc == INVALID_HANDLE_VALUE)
	{
		pInfo = pcsx2_t();
		return false;
	}

	return true;

	//	GET EE MEMORY
	//	return GetProcAddressEx(pInfo, "EEmem", pInfo.uEEmem) > 0;
}

__i64 PCS2Memory::GetProcAddressEx(const pcsx2_t& pInfo, const std::string& name, __i64& result)
{
	result = 0;	//	set invalid pointer

	auto dosHeader = Read<IMAGE_DOS_HEADER>(pInfo.uBaseAddress);
	if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE)
		return false;

	auto ntHeader = Read<IMAGE_NT_HEADERS>(pInfo.uBaseAddress + dosHeader.e_lfanew);
	for (IMAGE_DATA_DIRECTORY& directory : ntHeader.OptionalHeader.DataDirectory)
	{
		__i64 exportRVA = pInfo.uBaseAddress + directory.VirtualAddress;
		if (!exportRVA)
			continue;

		auto exports = Read<IMAGE_EXPORT_DIRECTORY>(exportRVA);
		auto namesRVA = pInfo.uBaseAddress + exports.AddressOfNames;
		auto functionsRVA = pInfo.uBaseAddress + exports.AddressOfFunctions;
		for (int i = 0; i < exports.NumberOfNames; i++)
		{
			auto nameOffset = Read<__i32>(namesRVA + i * sizeof(DWORD));
			auto fnOffset = Read<__i32>(functionsRVA + i * sizeof(DWORD));

			std::string name;
			if (!ReadString(pInfo.uBaseAddress + nameOffset, name))
				continue;

			if (name != name.c_str())	//	@TODO: case sensitive
				continue;

			result = Read<__i64>(pInfo.uBaseAddress + fnOffset);
			break;
		}

		if (result)
			break;
	}

	return result;
}

__i64 PCSX2Memory::GetAddr(const __i32& offset)
{
	if (g_pcsx.hProc == INVALID_HANDLE_VALUE)
		return false;

	return g_pcsx.uEEmem + offset;
}

bool PCSX2Memory::ReadString(const __i64& addr, std::string& result)
{
	if (g_pcsx.hProc == INVALID_HANDLE_VALUE)
		return false;

	char buff[MAX_PATH];
	if (!ReadProcessMemory(g_pcsx.hProc, (void*)addr, &buff, MAX_PATH, nullptr))
		return false;

	result = std::string(buff);	//	@TODO zero terminate? lol

	return true;
}

bool PCSX2Memory::ReadPointerChain(const __i64& addr, const std::vector<__i32>& offsets, __i64& result)
{
	if (g_pcsx.hProc == INVALID_HANDLE_VALUE)
		return false;

	//	@TODO : 

	return false;
}