#pragma once
#include "pch.h"
using namespace std::chrono_literals;

//---------------------------------------------------------------------------------------------------
// GLOBALS
static BOOL         g_Running = TRUE;

//---------------------------------------------------------------------------------------------------
// Main Cheat Thread
DWORD WINAPI Client()
{

    auto device     = *PlayStation2::CGlobals::g_gs_device;             //  Obtains the current render api device instance [@TODO: create method of obtaining current api class instance (GSDevice11)]
    auto renderer   = *PlayStation2::CGlobals::g_gs_renderer;           //  obtains Hardware renderer instance  
    auto emu_thread = *PlayStation2::CGlobals::g_emu_thread;            //  obtains main emu thread instance

    printf("[+] PCSX2 Framework Client Initialized!\n- device -> 0x%llX\n- renderer -> 0x%llX\n- thread -> 0x%llX\n\n",
        device,
        renderer,
        emu_thread
    );

    while (g_Running)
    {
        if (GetAsyncKeyState(VK_END) & 1)
            g_Running = FALSE;
    }

    // Thread Ended
    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------------------------------
void ClientExit(HMODULE hModule)
{
    PlayStation2::ShutdownSDK();
    FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
}

//---------------------------------------------------------------------------------------------------
void ClientInitialize(LPVOID hInstance)
{
#if _DEBUG
    MessageBoxA(NULL, "Initializing PCSX2 FrameWork", "DEBUG BUILD", MB_OK);
#endif    
    PlayStation2::InitSDK();        //  Initialize PS2 Mini SDK
    Client();                       //  Execute Client Loop
    ClientExit((HMODULE)hInstance); //  Exit
}