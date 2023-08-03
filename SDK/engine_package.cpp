#pragma once
#include "../pch.h"

/**
 * Name: PlayStation2 - PCSX2
 * Version: 0.0.1
 * Author: NightFyre
*/

#pragma pack(push, 0x01)
namespace PlayStation2
{

    GSRenderer**        CGlobals::g_gs_renderer;
    GSDevice**          CGlobals::g_gs_device;
    EmuThread**         CGlobals::g_emu_thread;
    //----------------------------------------------------------------------------------------------------
    //										CORE
    // 	   Created By NightFyre
    // CORE
    //-----------------------------------------------------------------------------------

    bool InitSDK(const std::string& moduleName, unsigned int gRenderer, unsigned int gDevice, unsigned int gEmu)
    {
        auto m_modBase = reinterpret_cast<__int64>(GetModuleHandleA(moduleName.c_str()));
        if (!m_modBase)
            return FALSE;

        CGlobals::g_gs_renderer = reinterpret_cast<GSRenderer**>(m_modBase + gRenderer);
        CGlobals::g_gs_device   = reinterpret_cast<GSDevice**>(m_modBase + gDevice);
        CGlobals::g_emu_thread  = reinterpret_cast<EmuThread**>(m_modBase + gEmu);

        // Initialize Core Class
        g_Engine = new Engine;

        // Resolve Signatures
        //	m_OnLeftDClick  = Signature("48 8B 05 ? ? ? ? 80 B8 ? ? ? ? ? 74 0C").Scan().As<uint64_t>();
        //  m_ResetEE       = Signature("80 3D ? ? ? ? ? 74 13 B8 ? ? ? ? 86").Scan().As<uint64_t>();	// 80 3D 09 18 E8 0C 00 74 13 B8 01 00 00 00 86 05 DF 0B E8 0C C6 05 F6 17 E8 0C 01
        //  g_gs_device     = 
        //  g_gs_renderer   =   
        //  g_emu_thread    =   
        return TRUE;
    }

    bool InitSDK() { return InitSDK("pcsx2-qt.exe", 0x40D4B98, 0x40D4AA0, 0xDD30BD8); }

    void ShutdownSDK()
    {

    }

    //----------------------------------------------------------------------------------------------------
	//										MEMORY
	// 	   Created By NightFyre
	// - Memory
	//-----------------------------------------------------------------------------------

    ///---------------------------------------------------------------------------------------------------
    //	[MEMORY]
    bool PS2Memory::isNullPtr()
    {
        return (this == nullptr) ? TRUE : FALSE;
    }

    ///---------------------------------------------------------------------------------------------------
    //	[MEMORY]
    // Converts shortened RAW PS2 format address to x64 address
    // EXAMPLE:
    // - Original RAW  : 2048D548  
    // - Shortened RAW : 0x48D548
    // - Base Memory   : 7FF660000000
    // - Result        : 7FF660000000 + 0x48D548 = 7FF66048D548
    uintptr_t PS2Memory::GetAddr(unsigned int RAW_PS2_OFFSET)
    {
        return (BasePS2MemorySpace + RAW_PS2_OFFSET);
    }

    ///---------------------------------------------------------------------------------------------------
    //	[MEMORY]
    // Assigns Shortened RAW PS2 Format Code to Class Pointer
    // <returns>ClassPointer</returns>
    // Note: Must be a base address
    // - CPlayer
    // - CCamera
    uintptr_t PS2Memory::GetClassPtr(unsigned int RAW_PS2_OFFSET)
    {
        return *(int32_t*)(RAW_PS2_OFFSET + BasePS2MemorySpace) + BasePS2MemorySpace;
    }

    ///---------------------------------------------------------------------------------------------------
    //	[MEMORY]
    // Resolves Pointer Chain from input Shorthand RAW PS2 Format Address
    // <returns></returns>
    uintptr_t PS2Memory::ResolvePtrChain(unsigned int RAW_PS2_OFFSET, std::vector<unsigned int> offsets)
    {
        //  --
        uintptr_t addr = (*(int32_t*)GetAddr(RAW_PS2_OFFSET)) + BasePS2MemorySpace;
        if (offsets.empty())
            return addr;

        // --- untested (possibly not needed anyways, should work)
        // GetClassPtr and relevent functions within each class will make this useless
        for (int i = 0; i < offsets.size(); i++)
        {
            addr = *(int32_t*)addr;
            addr += (offsets[i] + BasePS2MemorySpace);
        }
        return addr;
    }

    ///---------------------------------------------------------------------------------------------------
    //	[MEMORY]
    // Write Assembly patches to desired location
    // <returns>TRUE if operation is a success, otherwise result is FALSE</returns>
    // Note: resolve any offsets prior to running this function.
    bool PS2Memory::BytePatch(uintptr_t Address, BYTE* bytes, unsigned int size)
    {
        DWORD oldprotect;
        auto status = VirtualProtect(reinterpret_cast<void*>(Address), size, PAGE_EXECUTE_READWRITE, &oldprotect);
        if (!status)
            return FALSE;

        memcpy(reinterpret_cast<void*>(Address), bytes, size);
        VirtualProtect(reinterpret_cast<void*>(Address), size, oldprotect, &oldprotect);
        return TRUE;
    }

    ///---------------------------------------------------------------------------------------------------
    //	[MEMORY]
    // Write NOP opcodes for desired size at the set destination
    // <returns>TRUE if operation is a success, otherwise result is FALSE</returns>
    bool PS2Memory::Nop(uintptr_t Address, unsigned int size)
    {
        DWORD oldprotect;
        auto status = VirtualProtect(reinterpret_cast<void*>(Address), size, PAGE_EXECUTE_READWRITE, &oldprotect);
        if (!status)
            return FALSE;

        memset(reinterpret_cast<void*>(Address), 0x00, size);
        VirtualProtect(reinterpret_cast<void*>(Address), size, oldprotect, &oldprotect);
        return TRUE;
    }


    //----------------------------------------------------------------------------------------------------
	//										TOOLS
	// 	   Created By NightFyre
	// - Tools
	//-----------------------------------------------------------------------------------

    ///---------------------------------------------------------------------------------------------------
    //	[TOOL]
    //	Gets distance from Position A to Position B
    // <returns>Vector3</returns>
    float PS2Tools::GetDistanceTo3DObject(Vector3 POS, Vector3 POS2)
    {
        float x = (POS2.x - POS.x);
        float y = (POS2.y - POS.y);
        float z = (POS2.z - POS.z);
        float distance = std::sqrt(x * x + y * y + z * z);
        return (distance);
    }
}
#pragma pack(pop)
