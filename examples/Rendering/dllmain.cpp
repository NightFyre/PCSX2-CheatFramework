#pragma once
#include <CDK.h>

static bool g_running;

//---------------------------------------------------------------------------------------------------
//  Declare Hook Variables
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
//  Initialize Client Thread
DWORD WINAPI MainThread(LPVOID hInstance)
{

#if _DEBUG

    MessageBoxA(NULL, "Initializing PCSX2 Cheat Device", "DEBUG BUILD", MB_OK | MB_ICONINFORMATION);

#endif    

    //  initialize pcsx2 cheat dev kit
    if (PlayStation2::InitCDK("pcsx2-qt.exe", true))
    {
        PlayStation2::Console::ToggleViewState(true);
        PlayStation2::PCSX2::g_gs_device = reinterpret_cast<PlayStation2::GSDevice*>(*(__int64*)(PlayStation2::Memory::GetAddr(PlayStation2::PCSX2::o_gs_device))); //  offset because have not implemented reading assembly op code for variable , must be shrunk
        if (PlayStation2::PCSX2::g_gs_device)
        {

            if (PlayStation2::GSDevice::GetRenderAPI() != PlayStation2::RenderAPI::D3D11)
                MessageBoxA(NULL, "[ERROR] D3D11 NOT PRESENT", "Couldn't detect D3D11 as the active renderer.\nPlease switch to D3D11 before proceeding.", MB_OK | MB_ICONEXCLAMATION);


            // Get GS Device
            auto d3d11 = static_cast<PlayStation2::GSDevice11*>(PlayStation2::PCSX2::g_gs_device);
            if (d3d11)
            {
                //  Get SwapChain & Hook
                pSwapChain = d3d11->GetSwapChain();
                if (pSwapChain)
                    PlayStation2::Memory::hkVFunction(pSwapChain, 8, oIDXGISwapChainPresent, hkPresent);
            }
        }

        g_running = true;

        //  Loop
        do
        {
            //  Exit Module
            if (PlayStation2::Tools::GetKeyState(VK_END, 500))
                g_running = false;

        } while (g_running);
        
        //  Remove Hook
        if (pSwapChain)
            PlayStation2::Memory::hkRestoreVFunction(pSwapChain, 8, oIDXGISwapChainPresent);

        //  cleanup
        PlayStation2::ShutdownCDK(); 
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