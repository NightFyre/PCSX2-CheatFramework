#pragma once
#include "pch.h"

static bool g_running;

//---------------------------------------------------------------------------------------------------
//  Initialize Client
DWORD WINAPI Client(LPVOID hInstance)
{

#if _DEBUG

    MessageBoxA(NULL, "Initializing PCSX2 FrameWork", "DEBUG BUILD", MB_OK);

#endif    

    //  initialize pcsx2 cheat dev kit
    if (PlayStation2::InitSDK())
    {
        g_running = true;

        //  Get Debug Registers
        PlayStation2::PCSX2::g_cpuRegs = reinterpret_cast<PlayStation2::cpuRegisters*>((PlayStation2::Memory::GetPCSX2Addr(0x2EA8F2C) - 0x2AC));    //  c style cast to structure
        PlayStation2::PCSX2::g_psxRegs = reinterpret_cast<PlayStation2::psxRegisters*>((PlayStation2::Memory::GetPCSX2Addr(0x2EA809C) - 0x20C));    //  c style cast to structure
        PlayStation2::PCSX2::o_recResetEE = 0x2942C0;

        //  Render Loop
        do
        {
            //  Exit Key
            if (GetAsyncKeyState(VK_END) & 1)
                g_running = false;
            
            if (GetAsyncKeyState(VK_HOME) & 1)
                PlayStation2::PCSX2::ResetEE();

            if (PlayStation2::PCSX2::g_cpuRegs->pc == 0x1441C0)
                PlayStation2::Console::cLogMsg("[+] g_cpuRegs fn Compiled\n", PlayStation2::EConsoleColors::yellow);
            
            if (PlayStation2::PCSX2::g_psxRegs->pc == 0x1441C0)
                PlayStation2::Console::cLogMsg("[+] g_psxRegs fn Compiled\n", PlayStation2::EConsoleColors::yellow);

        } while (g_running);
        
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
        
        HANDLE pHand = CreateThread(0, 0, Client, hModule, 0, 0);
    
        if (pHand)
            CloseHandle(pHand);
    
    }
    
    return TRUE;
}