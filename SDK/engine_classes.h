#pragma once

/**
 * Name: PlayStation2 - PCSX2
 * Version: 0.0.1
 * Author: NightFyre
*/

#pragma pack(push, 0x01)
namespace PlayStation2
{
    //  @TODO: hook pcsx2 console update function for thread context & use pcsx2 console for output.
    //  bool (__fastcall* ConsoleWriteLine)(ConsoleColor, const char*, ...);
    class Console
    {
    public:
        static Console*                 GetDefaultInstance();
        static void                     LogMsgEx(FILE* stream, HANDLE pHand, const char* msg, EConsoleColors color, va_list args);      //  Logs message to the console
        static void                     LogMsg(const char* msg, ...);                                                                   //  Easily logs message to the console
        static void                     cLogMsg(const char* msg, EConsoleColors color, ...);                                            //  Logs a color message to the console
        static void                     ToggleViewState(bool isVisible);                                                                //  
        static void                     DestroyConsole();               // Should be called at the end of program as part of cleanup , decontructor with static methods would just constantly deallocate the console. 

    public:
        Console();
        Console(const char* title);

    private:
        static FILE*                    m_pInStream;		            //  Console Text Input Stream
        static FILE*                    m_pOutStream;		            //  Console Text Output Stream
        static FILE*                    m_pErrStream;		            //  Console Text Error Stream
        static HANDLE                   m_pHandle;	                    //  Handle to Console
        static HWND	                    m_pWndw;		                //  Window Handle to Console
        static HANDLE                   m_pPipe;		                //	Handle to Pipe
        static bool                     m_isConsoleAllocated;           //  flag for console creation
        static bool                     m_isVisible;                    //  flag for displaying console window
        static std::mutex               m_mutex;                        //  lock to ensure thread safety
        static Console*                 m_instance;                     //  static class instance
    };

    class Engine
    {
    public:
        typedef HRESULT(APIENTRY* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
    
    public:
        static Engine*                 GetDefaultInstance();
        static bool                    D3D11HookPresent(IDXGISwapChain* p, void* ofnc, void* nFnc);                             //
        static void                    D3D11UnHookPresent(IDXGISwapChain* p, void* ofnc);                                       //

    private:
        static Engine*                 m_instance;                     //  static class instance
    };

    class Memory
    {
    public:
        template <typename T>
        static inline T                 ReadMemoryEx(__int64 addr) 
        { 
            if (!addr)
                return false;

            return *(T*)addr; 
        }
        
        template<typename T>
        static inline bool              WriteMemoryEx(__int64 addr, T patch)
        { 
            if (!addr)
                return false;

            *(T*)addr = patch; 

            return true;
        }

    public:
        static Memory*                  GetDefaultInstance();
        static __int64                  GetModuleBase();                            //  returns the module base of PCSX2 Process
        static __int64                  GetAddr(unsigned int offset);               //  returns address offset from PCSX2 module base
        static bool                     ObtainProcessInfo(ProcessInfo& pInfo);      //  resolves information on a windows process ( PID, Handle, BaseAddr & Base Game Window Information )
        static __int64                  ResolvePtrChain(__int64 addr, std::vector<unsigned int> offsets = {});
        static bool                     BytePatch(__int64 Address, BYTE* bytes, unsigned int size);
        static bool                     NopBytes(__int64 Address, unsigned int size);

    public:
        Memory();
        ~Memory();
        
        friend class PS2Memory;
    
    private:
        static bool                     m_isInitialized;                                //  Set on first constructor call
        static __int64                  dwGameBase;                                     //  Process Module Base
        static __int64                  dwEEMem;                                        //  EEMem Pointer
        static __int64                  BasePS2MemorySpace;                             //  EEMem Base Address
        static ProcessInfo              Process;                                        //  Process Information Struct
        static Memory*                  m_instance;                                     //  static class instance
    };

    class PS2Memory
    {
    public:
        ///---------------------------------------------------------------------------------------------------
        //	[MEMORY]
        // Takes Full Address
        // Make sure to resolve any offsets prior to running this function
        // NOTE: only reads last 4bytes
        template<typename T>
        static inline T                 Read(__int64 addr)
        {
            //  4 byte alignment
            unsigned int format = *(__int32*)addr;

            //  cast to type
            return (T)(format);
        }

        ///---------------------------------------------------------------------------------------------------
        //	[MEMORY]
        // Takes Full Address
        // Make sure to resolve any offsets prior to running this function
        // NOTE: only writes last 4bytes
        template<typename T>
        static inline bool              Write(__int64 addr, T Patch)
        { 
            if (!addr)
                return false;

            *(T*)addr = Patch;
        
            return true;
        }

        template <typename T>
        static inline T                 EzRead(__int32 offset) { return Read<T>(GetAddr(offset)); }

        template <typename T>
        static inline bool              EzWrite(__int32 offset, T patch) { return Write(GetAddr(offset), patch); }


    public:
        static __int64                  GetModuleBase();                    //  returns the module base of the game
        static __int64                  GetAddr(__int32 offset);            //  returns address offset from PS2 EE module base        
        static __int64                  GetPtr(__int32 offset);             //  
        static __int64                  ResolvePtrChain(__int32 base_offset, std::vector<__int32> offsets);

    };

    class Tools
    {
    public:
        static float GetDistanceTo3DObject(Vector3 POS, Vector3 POS2);
    };
   
    class CGlobals
    {
    public:

        /*
            AOB: 
        */
        static class GSRenderer*        g_gs_renderer;

        /*
            
            AOB: 48 8B 0D ? ? ? ? 48 85 C9 74 20
        */
        static class GSDevice*          g_gs_device;

        /*
        */
        static class EmuThread*         g_emu_thread;

        //  
        static class Console*           g_console;
        static class Engine*            g_engine;
        static class Memory*            g_memory;
    };

}
#pragma pack(pop)