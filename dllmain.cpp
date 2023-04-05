#pragma once
#include "pch.h"
#include "example.hpp"

//---------------------------------------------------------------------------------------------------
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwCallReason, LPVOID lpReserved)
{
    switch (dwCallReason) 
    {
        case (DLL_PROCESS_ATTACH):      CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientInitialize, hModule, NULL, NULL);    break;
        case (DLL_PROCESS_DETACH):      g_Running ^= 1;                                                                             break;
    }
    return TRUE;
}