#pragma once

/**
 * Name: PlayStation2 - PCSX2
 * Version: 0.0.1
 * Author: NightFyre
*/

#pragma pack(push, 0x01)
namespace PlayStation2
{
	// --------------------------------------------------
	// # Forwards
	// --------------------------------------------------

	// --------------------------------------------------
	// # Global functions
	// --------------------------------------------------
	bool InitSDK(const std::string& moduleName, unsigned int gRenderer, unsigned int gDevice, unsigned int gEmu);
	bool InitSDK();
	void ShutdownSDK();

	unsigned int GetVtblOffset(void* czInstance, const char* dwModule = NULL);

	int GetVtblIndex(void* fncPtr, void* vTblAddr);

	template<typename fnc>
	fnc GetVFunction(const void* czInstance, size_t vfIndex)
	{
		auto vfTbl = *static_cast<const void***>(const_cast<void*>(czInstance));
		return reinterpret_cast<fnc>(const_cast<void (*)>(vfTbl[vfIndex]));
	}

	template<typename retType, typename... Args>
	retType CallVFunction(const void* czInstance, size_t vfIndex, Args&&... args)
	{
		const auto FUNCTION = GetVFunction<retType(*)(const void*, Args...)>(czInstance, vfIndex);
		return FUNCTION(czInstance, std::forward<Args>(args)...);
	}

	template<typename OrigFunc>
	void hkVFunction(void* instance, int index, OrigFunc& originalFunc, void* newFunc)
	{
		DWORD old;
		auto vTable		= *reinterpret_cast<__int64**>(instance);
		originalFunc	= reinterpret_cast<OrigFunc>(vTable[index]);
		VirtualProtect(&vTable[index], sizeof(void*), PAGE_EXECUTE_READWRITE, &old);
		vTable[index]	= reinterpret_cast<__int64>(newFunc);
		VirtualProtect(&vTable[index], sizeof(void*), old, &old);
	}

	template<typename OrigFunc>
	void hkRestoreVFunction(void* instance, int index, OrigFunc originalFunc)
	{
		DWORD old;
		auto vTable		= *reinterpret_cast<__int64**>(instance);
		VirtualProtect(&vTable[index], sizeof(void*), PAGE_EXECUTE_READWRITE, &old);
		vTable[index]	= reinterpret_cast<__int64>(originalFunc);
		VirtualProtect(&vTable[index], sizeof(void*), old, &old);
	}
}
#pragma pack(pop)

#include "engine_Structs.h"
#include "engine_Classes.h"