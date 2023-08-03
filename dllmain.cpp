#pragma once
#include "pch.h"
using namespace std::chrono_literals;
static bool         g_Running = FALSE;

//---------------------------------------------------------------------------------------------------
// Main Cheat Thread
void MainThread(bool& ref)
{
    do
    {
        if (GetAsyncKeyState(VK_END) & 1)
            ref ^= 1;

    } while (ref);
}

//---------------------------------------------------------------------------------------------------
//  Initialize Client
DWORD WINAPI Client(LPVOID hInstance)
{

#if _DEBUG
    MessageBoxA(NULL, "Initializing PCSX2 FrameWork", "DEBUG BUILD", MB_OK);
#endif    

    g_Running = PlayStation2::InitSDK();        //  Initialize PS2 Mini SDK
    if (g_Running)
    {
        PlayStation2::g_Engine->CreateConsole(" PCSX2 Framework Console ");     //  Manually spawn a console instance
        auto device         = *PlayStation2::CGlobals::g_gs_device;             //  Obtains the current render api device instance [@TODO: create method of obtaining current api class instance (GSDevice11)]
        auto renderer       = *PlayStation2::CGlobals::g_gs_renderer;           //  obtains Hardware renderer instance  
        auto emu_thread     = *PlayStation2::CGlobals::g_emu_thread;            //  obtains main emu thread instance
        
        printf("[+] PCSX2 Framework Client Initialized!\n- device\t-> 0x%llX\n- renderer\t-> 0x%llX\n- thread\t-> 0x%llX\n"
            ". . . PRESS [END] TO EXIT . . .\n\n",
            device,
            renderer,
            emu_thread
        );

        MainThread(g_Running);   //  <-- exectutes until g_Running = FALSE

        // Thread Ended
        PlayStation2::g_Engine->DestroyConsole();
        PlayStation2::ShutdownSDK();
    }
    FreeLibraryAndExitThread((HMODULE)hInstance, EXIT_SUCCESS);
    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------------------------------
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwCallReason, LPVOID lpReserved)
{
    switch (dwCallReason) 
    {
        case (DLL_PROCESS_ATTACH):      CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Client, hModule, NULL, NULL);    break;
        case (DLL_PROCESS_DETACH):      g_Running ^= 1;                                                                             break;
    }
    return TRUE;
}