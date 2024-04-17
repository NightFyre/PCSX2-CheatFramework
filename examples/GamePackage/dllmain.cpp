#pragma once
#include "pch.h"

#include "lib/PlayStation2-Game-SDKs/SOCOM U.S Navy Seals/SOCOM1_package.h"

static bool g_running;

//---------------------------------------------------------------------------------------------------
//  Initialize Client Thread
DWORD WINAPI MainThread(LPVOID hInstance)
{

#if _DEBUG

    MessageBoxA(NULL, "Initializing PCSX2 Cheat Device", "DEBUG BUILD", MB_OK | MB_ICONINFORMATION);

#endif    

    //  initialize pcsx2 cheat dev kit
    if (PlayStation2::InitCDK())
    {                
        using namespace PlayStation2;

        g_running = true;

        //  Loop
        do
        {
            //  Exit Module
            if (GetAsyncKeyState(VK_END) & 0x8000)
                g_running = false;

            
            SOCOM::CZSeal* pLocalPlayer = SOCOM::CZSeal::GetDefaultInstance();
            if (pLocalPlayer)
            {

                if (Tools::GetKeyState(VK_HOME, 500))
                {
                    Tools::CPUTimer t;

                    std::vector<SOCOM::CZSeal*> players;
                    SOCOM::CZMatchData::GetPlayers(&players);
                    DWORD i = 0;
                    for (auto ent : players)
                    {
                        i++;
                        if (!ent->IsValid())
                            continue;

                        Console::LogMsg("[+][%.3f][SOCOM::GetPlayers][%d/%d][HP: %.0f] %s\n", t.GetElapsedTime(clock()), i, players.size(), ent->GetHealth(), ent->GetName().c_str());
                    }
                    Console::LogMsg("[+][%.3f][SOCOM::GetPlayers] DONE\n\n", t.Stop(Tools::CPUTimer::ETimings::ETiming_MS));
                }

            }

        } while (g_running);
        
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