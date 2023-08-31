#pragma once
#include "../pch.h"

/**
 * Name: PlayStation2 - PCSX2
 * Version: 0.0.1
 * Author: NightFyre
*/

#pragma pack(push, 0x01)
namespace PlayStation2
{

    // --------------------------------------------------
    // # Forwards
    // --------------------------------------------------

    GSRenderer**        CGlobals::g_gs_renderer;
    GSDevice**          CGlobals::g_gs_device;
    EmuThread**         CGlobals::g_emu_thread;

    //----------------------------------------------------------------------------------------------------
    //										CORE
    //-----------------------------------------------------------------------------------

    ///---------------------------------------------------------------------------------------------------
    bool InitSDK(const std::string& moduleName, unsigned int gRenderer, unsigned int gDevice, unsigned int gEmu)
    {
        auto m_modBase = reinterpret_cast<__int64>(GetModuleHandleA(moduleName.c_str()));
        if (!m_modBase)
            return FALSE;

        g_Engine = std::make_unique<Engine>();                      // Initialize Core Class
        Memory::Memory();                                           //  Init Memory 

        //  Initialize PCSX2 Classes
        CGlobals::g_gs_renderer = reinterpret_cast<GSRenderer**>(m_modBase + gRenderer);
        CGlobals::g_gs_device   = reinterpret_cast<GSDevice**>(m_modBase + gDevice);
        CGlobals::g_emu_thread  = reinterpret_cast<EmuThread**>(m_modBase + gEmu);

        return TRUE;
    }

    ///---------------------------------------------------------------------------------------------------
    // Template Initialization function
    //  NOTE: offsets will not always be correct
    bool InitSDK() { return InitSDK("pcsx2-qt.exe", 0x40D4B98, 0x40D4AA0, 0xDD30BD8); }

    ///---------------------------------------------------------------------------------------------------
    void ShutdownSDK()
    {
        // clear pointers   (effectively freeing the memory)
        CGlobals::g_gs_renderer = nullptr;     
        CGlobals::g_gs_device   = nullptr;  
        CGlobals::g_emu_thread  = nullptr;  
        
        //  cleanup console if not handled by the user
        if (g_Engine->isConsolePresent())
            g_Engine->DestroyConsole();
    }

    ///---------------------------------------------------------------------------------------------------
	unsigned int GetVtblOffset(void* czInstance, const char* dwModule)
	{
		uintptr_t moduleBase = (uintptr_t)GetModuleHandleA(NULL);
		return ((*(unsigned int*)czInstance) - moduleBase);
	}

    ///---------------------------------------------------------------------------------------------------
	int GetVtblIndex(void* fncPtr, void* vTblAddr) { return (((__int64)fncPtr - (__int64)vTblAddr) / sizeof(void*)) - 1; }

    //----------------------------------------------------------------------------------------------------
    //										ENGINE
    //-----------------------------------------------------------------------------------

    ///---------------------------------------------------------------------------------------------------
    void Engine::CreateConsole(const char* title, bool bShow)
    {
        AllocConsole();														//  Allocate console for output
        p_bShowWindow = bShow;                                              //  set visible flag
        p_wndw_handle = GetConsoleWindow();									//  Store WindowHandle to console
        p_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);					//  Store handle to console
        freopen_s(&p_fin_stream, "CONIN$", "r", stdout);	                //  Establish input stream
        freopen_s(&p_fout_stream, "CONOUT$", "w", stdout);	                //  Establish ouput stream
        freopen_s(&p_ferr_stream, "CONOUT$", "w", stdout);	                //  Establish error stream
        SetConsoleTitleA(title);					                        //  Set console title
        ShowWindow(p_wndw_handle, p_bShowWindow ? SW_SHOW : SW_HIDE);		//	Show console window based on visible state
        p_bConsole = TRUE;
    }   

    ///---------------------------------------------------------------------------------------------------
    void Engine::DestroyConsole()
    {
        if (!p_bConsole || !p_fout_stream)
            return;

        p_bConsole      = false;                                            //
        p_bShowWindow   = false;                                            //
        fclose(p_fout_stream);                                              //  
        fclose(p_fin_stream);                                               //  
        fclose(p_ferr_stream);                                              //  
        DestroyWindow(p_wndw_handle);                                       //  
        FreeConsole();                                                      //  
    }

    ///---------------------------------------------------------------------------------------------------
    bool Engine::isConsolePresent() { return this->p_bConsole; }

    ///---------------------------------------------------------------------------------------------------
    //  D3D Template Hook
    void Engine::D3D11HookPresent()
    {
        //  Get Device Context
        auto device = *CGlobals::g_gs_device;
        if (!device)
            return;

        // Get GS Device
        auto d3d11 = reinterpret_cast<GSDevice11*>(device);
        if (!d3d11)
            return;

        this->m_pSwapChain = d3d11->GetSwapChain();
        if (!this->m_pSwapChain)
            return;

        // Hook
        hkVFunction(this->m_pSwapChain, 8, this->fnc_oIDXGISwapChainPresent, this->hkPresent);
    }
    void Engine::D3D11UnHookPresent()
    {
        if (!this->m_pSwapChain)
            return;

        hkRestoreVFunction(this->m_pSwapChain, 8, this->fnc_oIDXGISwapChainPresent);
        this->m_pSwapChain = nullptr;
        this->fnc_oIDXGISwapChainPresent = NULL;
    }
    HRESULT APIENTRY Engine::hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
    {
        //  Additional logic goes here

        // detour exit
        return g_Engine->fnc_oIDXGISwapChainPresent(pSwapChain, SyncInterval, Flags);
    }

    //----------------------------------------------------------------------------------------------------
	//										MEMORY
	//-----------------------------------------------------------------------------------

    //  Constructors
    Memory::Memory()
    {
        if (ObtainProcessInfo(Process))
        {
            Memory::dwGameBase          = (uintptr_t)Process.m_ModuleBase;
            Memory::dwEEMem             = (uintptr_t)GetProcAddress((HMODULE)Process.m_ModuleHandle, "EEmem");
            Memory::BasePS2MemorySpace  = *(uintptr_t*)dwEEMem;
        }
    }
    Memory::~Memory() {}

    ///---------------------------------------------------------------------------------------------------
    //	[MEMORY]
    //  
    bool Memory::ObtainProcessInfo(ProcessInfo& pInfo)
    {
        // Get initial process info
        pInfo.m_ModulePID       = GetCurrentProcessId();
        pInfo.m_ModuleHandle    = GetModuleHandle(NULL);
        pInfo.m_ModuleBase      = (uintptr_t)GetModuleHandle(NULL);

        // Get the path of the module
        char szModulePath[MAX_PATH];
        if (!GetModuleFileNameA((HMODULE)pInfo.m_ModuleHandle, szModulePath, MAX_PATH))
        {
            CloseHandle(pInfo.m_ModuleHandle);
            return FALSE;
        }

        // Get the main window of the current process
        HWND hWnd = GetForegroundWindow();
        if (!hWnd)
        {
            CloseHandle(pInfo.m_ModuleHandle);
            return FALSE;
        }

        // Get the window title and class name of the main window
        char szWindowTitle[MAX_PATH];
        char szClassName[MAX_PATH];
        GetWindowTextA(hWnd, szWindowTitle, MAX_PATH);
        GetClassNameA(hWnd, szClassName, MAX_PATH);

        // Get the size of the main window
        RECT rect;
        GetWindowRect(hWnd, &rect);
        int nWidth = rect.right - rect.left;
        int nHeight = rect.bottom - rect.top;

        // Set the fields of the ProcessInfo struct
        pInfo.m_GameWindow      = hWnd;
        pInfo.m_WindowTitle     = szWindowTitle;
        pInfo.m_ClassName       = szClassName;
        pInfo.m_ModulePath      = szModulePath;
        pInfo.m_WindowWidth     = nWidth;
        pInfo.m_WindowHeight    = nHeight;
        pInfo.m_WindowSize      = Vector2((float)nWidth, (float)nHeight);
        return TRUE;
    }

    ///---------------------------------------------------------------------------------------------------
    //	[MEMORY]
    // Converts shortened RAW PS2 format address to x64 address
    // EXAMPLE:
    // - Original RAW  : 2048D548  
    // - Shortened RAW : 0x48D548
    // - Base Memory   : 7FF660000000
    // - Result        : 7FF660000000 + 0x48D548 = 7FF66048D548
    uintptr_t Memory::GetPS2Address(unsigned int RAW_PS2_OFFSET) { return (Memory::BasePS2MemorySpace + RAW_PS2_OFFSET); }

    ///---------------------------------------------------------------------------------------------------
    //	[MEMORY]
    // Assigns Shortened RAW PS2 Format Code to Class Pointer
    // <returns>ClassPointer</returns>
    // Note: Must be a base address
    // - CPlayer
    // - CCamera
    uintptr_t Memory::DereferencePtr(unsigned int RAW_PS2_OFFSET) { return *(int32_t*)(RAW_PS2_OFFSET + Memory::BasePS2MemorySpace) + Memory::BasePS2MemorySpace; }

    ///---------------------------------------------------------------------------------------------------
    //	[MEMORY]
    // Resolves Pointer Chain from input Shorthand RAW PS2 Format Address
    // <returns></returns>
    uintptr_t Memory::ResolvePtrChain(unsigned int RAW_PS2_OFFSET, std::vector<unsigned int> offsets)
    {
        //  --
        uintptr_t addr = (*(int32_t*)GetPS2Address(RAW_PS2_OFFSET)) + BasePS2MemorySpace;
        if (offsets.empty())
            return addr;

        // --- untested (possibly not needed anyways, should work)
        // GetClassPtr and relevent functions within each class will make this useless
        for (int i = 0; i < offsets.size(); i++)
        {
            addr = *(int32_t*)addr;
            addr += (offsets[i] + BasePS2MemorySpace);
        }
        return addr;
    }

    ///---------------------------------------------------------------------------------------------------
    //	[MEMORY]
    // Write Assembly patches to desired location
    // <returns>TRUE if operation is a success, otherwise result is FALSE</returns>
    // Note: resolve any offsets prior to running this function.
    bool Memory::BytePatch(uintptr_t Address, BYTE* bytes, unsigned int size)
    {
        DWORD oldprotect;
        auto status = VirtualProtect(reinterpret_cast<void*>(Address), size, PAGE_EXECUTE_READWRITE, &oldprotect);
        if (!status)
            return FALSE;

        memcpy(reinterpret_cast<void*>(Address), bytes, size);
        VirtualProtect(reinterpret_cast<void*>(Address), size, oldprotect, &oldprotect);
        return TRUE;
    }

    ///---------------------------------------------------------------------------------------------------
    //	[MEMORY]
    // Write NOP opcodes for desired size at the set destination
    // <returns>TRUE if operation is a success, otherwise result is FALSE</returns>
    bool Memory::NopBytes(uintptr_t Address, unsigned int size)
    {
        DWORD oldprotect;
        auto status = VirtualProtect(reinterpret_cast<void*>(Address), size, PAGE_EXECUTE_READWRITE, &oldprotect);
        if (!status)
            return FALSE;

        memset(reinterpret_cast<void*>(Address), 0x00, size);
        VirtualProtect(reinterpret_cast<void*>(Address), size, oldprotect, &oldprotect);
        return TRUE;
    }


    //----------------------------------------------------------------------------------------------------
	//										TOOLS
	//-----------------------------------------------------------------------------------

    ///---------------------------------------------------------------------------------------------------
    //	[TOOL]
    //	Gets distance from Position A to Position B
    // <returns>Vector3</returns>
    float Tools::GetDistanceTo3DObject(Vector3 POS, Vector3 POS2)
    {
        float x = (POS2.x - POS.x);
        float y = (POS2.y - POS.y);
        float z = (POS2.z - POS.z);
        float distance = std::sqrt(x * x + y * y + z * z);
        return (distance);
    }
}
#pragma pack(pop)
