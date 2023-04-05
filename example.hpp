#pragma once
#include "pch.h"
using namespace std::chrono_literals;
using namespace PlayStation2;

//---------------------------------------------------------------------------------------------------
// GLOBALS
static BOOL         g_Running = TRUE;

//---------------------------------------------------------------------------------------------------
DWORD WINAPI Client()
{
    // Main Hack Thread
    while (g_Running)
    {
        //  Wait for Exit Key
        if (GetAsyncKeyState(VK_END) & 1)
            g_Running ^= 1;
    }

    // Hack Thread Ended
    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------------------------------
void ClientExit(HMODULE hModule)
{
    ShutdownSDK();
    FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
}

//---------------------------------------------------------------------------------------------------
void ClientInitialize(LPVOID hInstance)
{
#if _DEBUG
    MessageBoxA(NULL, "Initializing PCSX2 FrameWork", "DEBUG BUILD", MB_OK);
#endif

    // Initialize PS2 Mini SDK
    InitSDK();

    // Verify Game Module -- this is entirely dependent on the game that is running via PCSX2, Provided as a courtesy example
    // - results will vary in regards to the following
    //  - Game Region
    //  - Game Version
    //  - Game Patch Version
    auto socom_offsets = new SOCOM::Offsets();
    printf("%s", socom_offsets->LogData().c_str());         //  Verifies ability to Read & Write Process Memory as well as obtain class pointers
    delete socom_offsets;                                   //  Cleanup created class

    //  Execute Client Loop
    Client();

    //   Exit
    ClientExit((HMODULE)hInstance);
}