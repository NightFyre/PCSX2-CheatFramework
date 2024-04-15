#pragma once
#include "pch.h"

static bool g_running;

#define fn_start 0x0        //  should be function start address 

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
        //  PlayStation2::PCSX2::o_cpuRegs = 0x0;
        //  PlayStation2::PCSX2::g_cpuRegs = reinterpret_cast<PlayStation2::cpuRegisters*>((PlayStation2::Memory::GetAddr(PlayStation2::PCSX2::o_cpuRegs) - 0x2AC));    //  [0x2AC is g_cpuRegs.code] The offset for cpuRegs found in recompileNextInstruction is displaced to the code offset in the structure

        //  PlayStation2::PCSX2::o_psxRegs = 0x0;
        //  PlayStation2::PCSX2::g_psxRegs = reinterpret_cast<PlayStation2::psxRegisters*>((PlayStation2::Memory::GetAddr(PlayStation2::PCSX2::o_psxRegs) - 0x20C));   //    similar to cpuRegs, the found offset is displaced and must be brought back to origin to access the data.
        
        /// Reset Recompiler
        //  PlayStation2::PCSX2::o_recResetEE = 0x0;    
        //  PlayStation2::PCSX2::ResetEE();             //  Reset EE so that we can re/capture events

        //  Render Loop
        do
        {
            //  Exit Key
            if (GetAsyncKeyState(VK_END) & 1)
                g_running = false;
         
            if (PlayStation2::PCSX2::o_recResetEE <= 0)
                continue;

            //  Recompile EE , can be used to capture a functions compilation. May need to trigger an event of sorts in game. YMMV
            if (GetAsyncKeyState(VK_HOME) & 1)
                PlayStation2::PCSX2::ResetEE();

            if (fn_start <= 0)
                continue;

            //  Capture EE Function Compilation
            if (PlayStation2::PCSX2::g_cpuRegs->pc == fn_start)
            {
                using namespace PlayStation2;

                //  Capture Register Data
                const auto pc = PCSX2::g_cpuRegs->pc;           //  program counter
                const auto code = PCSX2::g_cpuRegs->code;       //  ~
                const auto GPR = PCSX2::g_cpuRegs->GPR;         //  registers

                //  Log Data
                Console::cLogMsg("[+] PCSX2::PS2::EE::fnName()\npc:\t%d\ncode:\t%d\nGPR.sp:\t%d\n\n",
                    EConsoleColors::yellow,
                    pc,                         //  
                    code,                       //  
                    GPR.sp.n[0]                 //  
                );
            }

            //  Capture IOP Function Compilation
            if (PlayStation2::PCSX2::g_psxRegs->pc == fn_start)
            {
                using namespace PlayStation2;

                //  Capture Register Data
                const auto pc = PCSX2::g_psxRegs->pc;           //  program counter
                const auto code = PCSX2::g_psxRegs->code;       //  ~
                const auto GPR = PCSX2::g_psxRegs->GPR;         //  registers

                Console::cLogMsg("[+] PCSX2::PS2::IOP::fnName()\npc:\t%d\ncode:\t%d\nGPR.sp:\t%d\n\n", 
                    EConsoleColors::yellow,
                    pc,                         //  
                    code,                       //  
                    GPR.sp                      //  
                );
            }

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