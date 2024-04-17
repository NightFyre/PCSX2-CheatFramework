#pragma once
#include "../pch.h"

/**
 * Name: PlayStation2 - PCSX2
 * Version: 1.0.0
 * Author: NightFyre
*/

namespace PlayStation2
{

    //----------------------------------------------------------------------------------------------------
    //										CORE
    //-----------------------------------------------------------------------------------

    ///---------------------------------------------------------------------------------------------------
    unsigned int GetVtblOffset(void* czInstance, const char* dwModule) { return ((*(unsigned int*)czInstance) - Memory::GetModuleBase()); }

    ///---------------------------------------------------------------------------------------------------
    int GetVtblIndex(void* fncPtr, void* vTblAddr) { return (((__int64)fncPtr - (__int64)vTblAddr) / sizeof(void*)) - 1; }


    //----------------------------------------------------------------------------------------------------
    //										PCSX2
    //-----------------------------------------------------------------------------------

#pragma region  //  PCSX2

    //----------------------------------------------------------------------------------------------------
    //  STATICS
    //  Class Offsets
    EmuThread*                  PCSX2::g_emu_thread;
    GSRenderer*                 PCSX2::g_gs_renderer;
    unsigned int                PCSX2::o_gs_device{ 0x3FA2728 };
    unsigned int                PCSX2::o_GSUpdateWindow;
    unsigned int                PCSX2::o_GSDevice_GetRenderAPI;
    GSDevice*                   PCSX2::g_gs_device;
    unsigned int                PCSX2::o_psxRecompileInstruction{ 0x269D80 };
    unsigned int                PCSX2::o_recompileNextInstruction{ 0x291CA0 };
    unsigned int                PCSX2::o_recResetEE{ 0x2942C0 };
    cpuRegisters*               PCSX2::g_cpuRegs;
    __int32                     PCSX2::g_cpupc{ 0x0 };
    unsigned int                PCSX2::o_cpuRegs{ 0x2EA8F2C };
    psxRegisters*               PCSX2::g_psxRegs;
    __int32                     PCSX2::g_psxpc{ 0x0 };
    unsigned int                PCSX2::o_psxRegs{ 0x2EA809C };

    //-----------------------------------------------------------------------------------
    void PCSX2::ResetEE()
    {
        if (!o_recResetEE)
        {
            Console::cLogMsg("[!] Failed to Reset EE/iR5900 Recompiler ->\t'o_recResetEE was nullptr'\n", EConsoleColors::dark_red);
            return;
        }

        static recResetEE_stub fn = reinterpret_cast<recResetEE_stub>(Memory::GetAddr(o_recResetEE)); //  @TODO: provide method for obtaining function pointer

        fn();
    
        Console::cLogMsg("[+] EE/iR5900 Recompiler Reset\n", EConsoleColors::dark_gray);
    }

#pragma endregion

    //----------------------------------------------------------------------------------------------------
    //										GSDevice
    //-----------------------------------------------------------------------------------

#pragma region  //  GSDevice

    //-----------------------------------------------------------------------------------
    RenderAPI GSDevice::GetRenderAPI()
    {
        return CallVFunction<RenderAPI>(PCSX2::g_gs_device, 9);
    }

#pragma endregion

    //----------------------------------------------------------------------------------------------------
    //									GSDevice::GSDevice11
    //-----------------------------------------------------------------------------------

#pragma region  //  GSDevice::GSDevice11

    bool GSDevice11::isValidSwapChain() { return m_swap_chain != nullptr; }
    
    IDXGISwapChain* GSDevice11::GetSwapChain() { return m_swap_chain; }
    
    ID3D11Device* GSDevice11::GetDevice() { return m_dev; }

    //-----------------------------------------------------------------------------------
    DXGI_SWAP_CHAIN_DESC GSDevice11::GetDesc()
    {
        DXGI_SWAP_CHAIN_DESC desc;
        if (this->m_swap_chain != nullptr)
            this->m_swap_chain->GetDesc(&desc);     //  Directly call d3d11.dll method
        return desc;
    }

#pragma endregion

    //----------------------------------------------------------------------------------------------------
    //									GSDevice::GSDevice12
    //-----------------------------------------------------------------------------------

#pragma region  //  GSDevice::GSDevice11

    bool GSDevice12::isValidSwapChain() { return m_swap_chain != nullptr; }

    IDXGISwapChain* GSDevice12::GetSwapChain() { return m_swap_chain; }

#pragma endregion

}
