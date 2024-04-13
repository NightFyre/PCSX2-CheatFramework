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
        bool result{ false };

        auto m_modBase = reinterpret_cast<__int64>(GetModuleHandleA(moduleName.c_str()));
        if (!m_modBase)
            return result;

        //  Initialize PCSX2 Classes
        CGlobals::g_gs_renderer = reinterpret_cast<GSRenderer**>(m_modBase + gRenderer);
        CGlobals::g_gs_device   = reinterpret_cast<GSDevice**>(m_modBase + gDevice);
        CGlobals::g_emu_thread  = reinterpret_cast<EmuThread**>(m_modBase + gEmu);

        result = (CGlobals::g_gs_renderer > 0 && CGlobals::g_gs_device > 0 && CGlobals::g_emu_thread > 0);
        if (result)
        {
            Console::LogMsg("[+] PCSX2 Framework Client Initialized!\n"
                "gs_Renderer:\t0x%llX\n"
                "gs_Device:\t0x%llX\n"
                "gs_Emu:\t0x%llX\n"
                "EEMem:\t0x%llX\n",
                CGlobals::g_gs_renderer,
                CGlobals::g_gs_device,
                CGlobals::g_emu_thread,
                Memory::GetBasePS2Address()
            );
        }
        return result;
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
    //										CONSOLE
    //-----------------------------------------------------------------------------------

    ///---------------------------------------------------------------------------------------------------
    //  STATICS
    FILE*                   Console::m_pInStream;
    FILE*                   Console::m_pOutStream;
    FILE*                   Console::m_pErrStream;
    HANDLE                  Console::m_pHandle;
    HWND	                Console::m_pWndw;
    HANDLE                  Console::m_pPipe;
    bool                    Console::m_isConsoleAllocated{ false };
    bool                    Console::m_isVisible{ true };

    ///---------------------------------------------------------------------------------------------------
    Console::Console()
    {
        if (m_isConsoleAllocated)
            return;

        AllocConsole();														//  Allocate console for output
        m_pHandle = GetStdHandle(STD_OUTPUT_HANDLE);					    //  Store handle to console
        m_pWndw = GetConsoleWindow();									    //  Store WindowHandle to console
        freopen_s(&m_pInStream, "CONIN$", "r", stdout);	                    //  Establish input stream
        freopen_s(&m_pOutStream, "CONOUT$", "w", stdout);	                //  Establish ouput stream
        freopen_s(&m_pErrStream, "CONOUT$", "w", stdout);	                //  Establish error stream
        ShowWindow(m_pWndw, m_isVisible ? SW_SHOW : SW_HIDE);		        //	Show console window based on visible state
        m_isConsoleAllocated = true;
    }

    ///---------------------------------------------------------------------------------------------------
    Console::Console(const char* title)
    {
        if (m_isConsoleAllocated)
            return;

        AllocConsole();														//  Allocate console for output
        m_pHandle = GetStdHandle(STD_OUTPUT_HANDLE);					    //  Store handle to console
        m_pWndw = GetConsoleWindow();									    //  Store WindowHandle to console
        freopen_s(&m_pInStream, "CONIN$", "r", stdout);	                    //  Establish input stream
        freopen_s(&m_pOutStream, "CONOUT$", "w", stdout);	                //  Establish ouput stream
        freopen_s(&m_pErrStream, "CONOUT$", "w", stdout);	                //  Establish error stream
        ShowWindow(m_pWndw, m_isVisible ? SW_SHOW : SW_HIDE);		        //	Show console window based on visible state
        SetConsoleTitleA(title);                                            //  Set console window title
        m_isConsoleAllocated = true;
    }

    ///---------------------------------------------------------------------------------------------------
    void Console::DestroyConsole()
    {
        if (m_isConsoleAllocated)
            return;

        m_isConsoleAllocated = false;                                       //
        m_isVisible = true;                                                 //
        fclose(m_pInStream);                                                //  
        fclose(m_pOutStream);                                               //  
        fclose(m_pErrStream);                                               //  
        DestroyWindow(m_pWndw);                                             //  
        FreeConsole();
    }	
    
    //---------------------------------------------------------------------------------------------------
    void Console::LogMsgEx(FILE* stream, HANDLE pHand, const char* msg, EConsoleColors color, va_list args)
    {
        if (!m_isConsoleAllocated)
            return;

        SetConsoleTextAttribute(pHand, color);					            //	Set output text color
        vfprintf(stream, msg, args);								        //	print
        SetConsoleTextAttribute(pHand, EConsoleColors::DEFAULT);	        //	Restore output text color to default
    }

    //---------------------------------------------------------------------------------------------------
    void Console::LogMsg(const char* text, ...)
    {
        if (!m_isConsoleAllocated)
            return;
        
        va_list args;												        //	declare arguments
        va_start(args, text);										        //	get arguments
        LogMsgEx(m_pOutStream, m_pHandle, text, EConsoleColors::DEFAULT, args);//	print
        va_end(args);												        //	clear arguments
    }

    //---------------------------------------------------------------------------------------------------
    void Console::cLogMsg(const char* text, EConsoleColors color, ...)
    {
        if (!m_isConsoleAllocated)
            return;

        va_list args;												        //	declare arguments
        va_start(args, text);										        //	get arguments
        LogMsgEx(m_pOutStream, m_pHandle, text, color, args);		        //	print
        va_end(args);												        //	clear arguments
    }

    //----------------------------------------------------------------------------------------------------
    //										ENGINE
    //-----------------------------------------------------------------------------------

    ///---------------------------------------------------------------------------------------------------
    //  D3D Template Hook
    void Engine::D3D11HookPresent(IDXGISwapChain* p, IDXGISwapChainPresent ofnc, void* nFnc)
    {
        //  Get Device Context
        auto device = *CGlobals::g_gs_device;
        if (!device)
            return;

        // Get GS Device
        auto d3d11 = reinterpret_cast<GSDevice11*>(device);
        if (!d3d11)
            return;

        //  Get SwapChain
        p = d3d11->GetSwapChain();
        if (!p)
            return;

        // Hook
        hkVFunction(p, 8, ofnc, nFnc);
    }

    ///---------------------------------------------------------------------------------------------------
    void Engine::D3D11UnHookPresent(IDXGISwapChain* p, IDXGISwapChainPresent ofnc)
    {
        if (!p)
            return;

        hkRestoreVFunction(p, 8, ofnc);
        p = nullptr;
        ofnc = NULL;
    }

    //----------------------------------------------------------------------------------------------------
	//										MEMORY
	//-----------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------
    //  STATICS
    uintptr_t                Memory::dwGameBase;                                     
    uintptr_t                Memory::dwEEMem;                                        
    uintptr_t                Memory::BasePS2MemorySpace;                             
    ProcessInfo              Memory::Process;                                        
    bool                     Memory::m_isInitialized;                

    //----------------------------------------------------------------------------------------------------
    //  CONSTRUCTORS
    Memory::Memory()
    {
        if (m_isInitialized)
            return;

        if (ObtainProcessInfo(Process))
        {
            Memory::dwGameBase          = (uintptr_t)Process.m_ModuleBase;
            Memory::dwEEMem             = (uintptr_t)GetProcAddress((HMODULE)Process.m_ModuleHandle, "EEmem");
            Memory::BasePS2MemorySpace  = *(uintptr_t*)dwEEMem;
            m_isInitialized = true;
        }
    }
    Memory::~Memory() {}

    ///---------------------------------------------------------------------------------------------------
    //	[MEMORY]


    ///---------------------------------------------------------------------------------------------------
    uintptr_t Memory::GetBasePS2Address() { return BasePS2MemorySpace; }

    ///---------------------------------------------------------------------------------------------------
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
