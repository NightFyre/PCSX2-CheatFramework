#pragma once
#include "pch.h"

//---------------------------------------------------------------------------------------------------
//  Initialize Client
DWORD WINAPI Client(LPVOID hInstance)
{

#if _DEBUG

    MessageBoxA(NULL, "Initializing PCSX2 FrameWork", "DEBUG BUILD", MB_OK);

#endif    

    if (PlayStation2::InitSDK())
    {

        //  Render Loop
        do
        {
            //  Exit Key
            if (GetAsyncKeyState(VK_END) & 1)
                g_running ^= 1;   //  flips the bit

        } while (g_running);
        
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