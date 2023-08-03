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

    //----------------------------------------------------------------------------------------------------
    //										PCSX2
    //-----------------------------------------------------------------------------------

    void PCSX2::recResetEE(uintptr_t Address)
    {
        typedef void(__cdecl* pFunctionAddress)();
        pFunctionAddress pResetEE = (pFunctionAddress)((Address));
        pResetEE();
    }

    RenderAPI GSDevice::GetRenderAPI()
    {
        //  swapchain will be null when a rendering device is not being used
        //  thankfully each swapchain member variable is located at a different offset per each rendering api
        //  we can cast our current instance to each rendering api class instance and check whcih swapchain is non nullptr and return the enum

        if (reinterpret_cast<GSDevice11*>(this)->isValidSwapChain())
            return RenderAPI::D3D11;
          
        if (reinterpret_cast<GSDevice12*>(this)->isValidSwapChain())
            return RenderAPI::D3D12;

        return RenderAPI::None;
    }

    //----------------------------------------------------------------------------------------------------
    //										DirectX 11
    //-----------------------------------------------------------------------------------
    bool GSDevice11::isValidSwapChain() { return m_swap_chain != nullptr; }
    IDXGISwapChain* GSDevice11::GetSwapChain() { return m_swap_chain; }
    ID3D11Device* GSDevice11::GetDevice() { return m_dev; }
    DXGI_SWAP_CHAIN_DESC GSDevice11::GetDesc()
    {
        DXGI_SWAP_CHAIN_DESC desc;
        if (this->m_swap_chain != nullptr)
            this->m_swap_chain->GetDesc(&desc);     //  Directly call d3d11.dll method
        return desc;
    }

    //----------------------------------------------------------------------------------------------------
    //										DirectX 12
    //-----------------------------------------------------------------------------------
    bool GSDevice12::isValidSwapChain() { return m_swap_chain != nullptr; }
    IDXGISwapChain* GSDevice12::GetSwapChain() { return m_swap_chain; }

}
#pragma pack(pop)
