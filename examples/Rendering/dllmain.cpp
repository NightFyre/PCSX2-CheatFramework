#pragma once
#include "pch.h"

static bool g_running;

//---------------------------------------------------------------------------------------------------
//  Initialize Client
static bool IsPrint{ false };
IDXGISwapChain* pSwapChain;
typedef HRESULT(APIENTRY* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
IDXGISwapChainPresent oIDXGISwapChainPresent = NULL;
HRESULT APIENTRY hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (!IsPrint)
    {
        PlayStation2::Console::LogMsg("[+][hkPresent] SwapChain: 0x%llX\n", pSwapChain);
        IsPrint = true;
    }

    return oIDXGISwapChainPresent(pSwapChain, SyncInterval, Flags);
}

//---------------------------------------------------------------------------------------------------
//  Initialize Client
DWORD WINAPI MainThread(LPVOID hInstance)
{

#if _DEBUG

    MessageBoxA(NULL, "Initializing PCSX2 FrameWork", "DEBUG BUILD", MB_OK | MB_ICONINFORMATION);

#endif    

    //  initialize pcsx2 cheat dev kit
    if (PlayStation2::InitSDK("pcsx2-qt.exe", PlayStation2::PCSX2::o_gs_device))
    {


        if (PlayStation2::GSDevice::GetRenderAPI() != PlayStation2::RenderAPI::D3D11)
            MessageBoxA(NULL, "[ERROR] D3D11 NOT PRESENT", "Couldn't detect D3D11 as the active renderer.\nPlease switch to D3D11 before proceeding.", MB_OK | MB_ICONEXCLAMATION);


        // Get GS Device
        auto d3d11 = static_cast<PlayStation2::GSDevice11*>(PlayStation2::CGlobals::g_gs_device);
        if (d3d11)
        {
            //  Get SwapChain & Hook
            pSwapChain = d3d11->GetSwapChain();
            if (pSwapChain)            
                PlayStation2::hkVFunction(pSwapChain, 8, oIDXGISwapChainPresent, hkPresent);
        }

        g_running = true;

        //  Render Loop
        do
        {
            //  Exit Key
            if (GetAsyncKeyState(VK_END) & 1)
                g_running = false;

        } while (g_running);
        
        //  Remove Hook
        if (pSwapChain)
            PlayStation2::hkRestoreVFunction(pSwapChain, 8, oIDXGISwapChainPresent);

        //  cleanup
        PlayStation2::ShutdownSDK(); 
    }

    //  Exit
    FreeLibraryAndExitThread((HMODULE)hInstance, EXIT_SUCCESS);
    
    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------------------------------
//  EntryPoint
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwCallReason, LPVOID x)
{
    UNREFERENCED_PARAMETER(x);

    if (dwCallReason == DLL_PROCESS_ATTACH)
    {
        
        DisableThreadLibraryCalls(hModule);
        
        HANDLE pHand = CreateThread(0, 0, MainThread, hModule, 0, 0);
    
        if (pHand)
            CloseHandle(pHand);
    
    }
    
    return TRUE;
}