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

    g_Running = PlayStation2::InitSDK();                                        //  Initialize PS2 Mini SDK
    if (g_Running)                                                              //  Check the state of initialization
    {
        PlayStation2::g_Engine->CreateConsole(" PCSX2 Framework Console ");     //  Manually spawn a console instance ( this will be an sdk initialization option at some point in the future )
        auto device         = *PlayStation2::CGlobals::g_gs_device;             //  Obtains the current render api device instance [@TODO: create method of obtaining current api class instance (GSDevice11)]
        auto renderer       = *PlayStation2::CGlobals::g_gs_renderer;           //  obtains Hardware renderer instance  
        auto emu_thread     = *PlayStation2::CGlobals::g_emu_thread;            //  obtains main emu thread instance
        
        printf("[+] PCSX2 Framework Client Initialized!\n- device\t-> 0x%llX\n- renderer\t-> 0x%llX\n- thread\t-> 0x%llX\n"
            ". . . PRESS [END] TO EXIT . . .\n\n",
            device,
            renderer,
            emu_thread
        );

        //  Render Loop
        do
        {
            //  Exit Key
            if (GetAsyncKeyState(VK_END) & 1)
                g_Running ^= 1;   //  flips the bit

        } while (g_Running);
        
        //  cleanup
        if (PlayStation2::g_Engine->isConsolePresent())     //  check if console is present ( this will be included in SDK initialization later on )
            PlayStation2::g_Engine->DestroyConsole();       //  Destroy console instance ( otherwise it would linger and all references to it would be lost without manually obtaining the instance )
        PlayStation2::ShutdownSDK();                        //  Shutdown the sdk
    }

    //  Exit
    FreeLibraryAndExitThread((HMODULE)hInstance, EXIT_SUCCESS);
    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------------------------------
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwCallReason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwCallReason == DLL_PROCESS_ATTACH)
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Client, hModule, NULL, NULL); 
    
    return TRUE;
}