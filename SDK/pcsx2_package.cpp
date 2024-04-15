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
    //  Class Offsets
    unsigned int PCSX2::o_gs_device;
    
    //  Fn Offsets
    unsigned int PCSX2::o_GSDevice_GetRenderAPI;
    unsigned int PCSX2::o_GSUpdateWindow;
    unsigned int PCSX2::o_psxRecompileInstruction;
    unsigned int PCSX2::o_recResetEE;

    //  Structures
    cpuRegisters* PCSX2::g_cpuRegs;
    __int32 PCSX2::g_cpupc;
    psxRegisters* PCSX2::g_psxRegs;
    __int32 PCSX2::g_psxpc;

    //----------------------------------------------------------------------------------------------------
    //										PCSX2
    //-----------------------------------------------------------------------------------

    //-----------------------------------------------------------------------------------
    void PCSX2::ResetEE()
    {
        if (!o_recResetEE)
        {
            // @TODO: print error
            return;
        }

        static recResetEE_stub fn = reinterpret_cast<recResetEE_stub>(Memory::GetPCSX2Addr(o_recResetEE)); //  @TODO: provide method for obtaining function pointer

        fn();
    }

    //----------------------------------------------------------------------------------------------------
    //										GSDevice
    //-----------------------------------------------------------------------------------

    //-----------------------------------------------------------------------------------
    RenderAPI GSDevice::GetRenderAPI()
    {
        return CallVFunction<RenderAPI>(CGlobals::g_gs_device, 9);
    }

    //----------------------------------------------------------------------------------------------------
    //									GSDevice :: DirectX 11
    //-----------------------------------------------------------------------------------

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

    //----------------------------------------------------------------------------------------------------
    //									GSDevice :: DirectX 12
    //-----------------------------------------------------------------------------------
    bool GSDevice12::isValidSwapChain() { return m_swap_chain != nullptr; }
    IDXGISwapChain* GSDevice12::GetSwapChain() { return m_swap_chain; }

}
#pragma pack(pop)
