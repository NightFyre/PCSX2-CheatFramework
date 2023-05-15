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
        // Obtain an instance of local player
        auto _managed_pointer = new SOCOM::Offsets();
        auto _localPlayer = reinterpret_cast<SOCOM::CZSealBody*>(_managed_pointer->SEALPointer);
        delete _managed_pointer;
        
        auto _managed_pointer_2 = new SOCOM::CZMatchData();
        if (!_managed_pointer_2->isMatchEnded())
        {
            // Get all entities in the match
            auto _entities = _managed_pointer_2->GetPlayers();

            // Verify Local Player is Valid
            if (_localPlayer != nullptr && _localPlayer->IsValid())
            {
                // Set Ammo to 1337
                for (auto& mag : _localPlayer->PrimaryMag)
                    mag = 1337;

                for (auto& mag : _localPlayer->SecondaryMag)
                    mag = 1337;

                // Manipulate Other Entities
                for (auto& ent : _entities)
                {
                    if (ent->GetPlayerName() == _localPlayer->GetPlayerName())
                        continue;

                    ent->Teleport(_localPlayer->Position);  //  Send ent to local player
                    ent->TeamID = _localPlayer->TeamID;     //  Set ent to same team as local player
                    ent->Health = .10f;                     //  Set ent to 1shot
                }
            }
        }
        delete _managed_pointer_2;

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


    // Get Local Player Instance
    auto _player = reinterpret_cast<SOCOM::CZSealBody*>(socom_offsets->SEALPointer);
    if (_player != nullptr && _player->IsValid())
        printf("%s", _player->LogData().c_str());           //  Print information about local player character

    //  Get Match Data Instance
    auto _match = new SOCOM::CZMatchData();
    
    //  Check if match is currently playing
    if (!_match->isMatchEnded())
    {
        //  Get Entities
        auto Entities = _match->GetPlayers();

        //  Log each entity data
        for (auto player : Entities)
            printf("%s", player->LogData().c_str());

        Entities.clear();                                   //  cleanup
    }
    delete socom_offsets;                                   //  . . .
    delete _match;                                          //  . . .

    //  Execute Client Loop
    Client();

    //   Exit
    ClientExit((HMODULE)hInstance);
}