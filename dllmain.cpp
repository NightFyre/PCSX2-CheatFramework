#pragma once
#include "pch.h"
using namespace std::chrono_literals;
static bool         g_Running = FALSE;

//---------------------------------------------------------------------------------------------------
//  Initialize Client
DWORD WINAPI Client(LPVOID hInstance)
{

#if _DEBUG

    MessageBoxA(NULL, "Initializing PCSX2 FrameWork", "DEBUG BUILD", MB_OK);

#endif    

    g_Running = PlayStation2::InitSDK();                                                //  Initialize PS2 Mini SDK
    
    if (g_Running)                                                                      //  Check the state of initialization
    {
        auto device         = *PlayStation2::CGlobals::g_gs_device;                     //  Obtains the current render api device instance [@TODO: create method of obtaining current api class instance (GSDevice11)]
        
        auto renderer       = *PlayStation2::CGlobals::g_gs_renderer;                   //  obtains Hardware renderer instance  
    
        auto emu_thread     = *PlayStation2::CGlobals::g_emu_thread;                    //  obtains main emu thread instance

        //  Render Loop
        do
        {
            //  Exit Key
            if (GetAsyncKeyState(VK_END) & 1)
                g_Running ^= 1;   //  flips the bit

        } while (g_Running);
        
        //  cleanup
        PlayStation2::ShutdownSDK();                        //  Shutdown the sdk
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
        
        HANDLE pHand = CreateThread(0, 0, Client, hModule, 0, 0);
    
        if (pHand)
            CloseHandle(pHand);
    
    }
    
    return TRUE;
}