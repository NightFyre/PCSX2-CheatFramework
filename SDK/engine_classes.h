#pragma once

/**
 * Name: PlayStation2 - PCSX2
 * Version: 0.0.1
 * Author: NightFyre
*/

#pragma pack(push, 0x01)
namespace PlayStation2
{
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
    
    public: //  METHODS
        static Memory*                  GetDefaultInstance();
        static uintptr_t                GetBasePS2Address();   //  returns the module base of the game
        static bool                     ObtainProcessInfo(ProcessInfo& pInfo);
        static uintptr_t                GetPS2Address(unsigned int RAW_PS2_OFFSET);
        static uintptr_t                DereferencePtr(unsigned int RAW_PS2_OFFSET);
        static uintptr_t                ResolvePtrChain(unsigned int RAW_PS2_OFFSET, std::vector<unsigned int> offsets = {});
        static bool                     BytePatch(uintptr_t Address, BYTE* bytes, unsigned int size);
        static bool                     NopBytes(uintptr_t Address, unsigned int size);

        ///---------------------------------------------------------------------------------------------------
        //	[MEMORY]
        // Takes Full Address
        // Make sure to resolve any offsets prior to running this function
        // NOTE: only reads last 4bytes
        template<typename T> 
        static inline T PS2Read(uintptr_t Address)
        {
            /// USING FRAMERATE AS AN EXAMPLE
            //0x7FF6B048CF60 0000001E0000001E   //  8   Bytes
            //0x7FF6B048CF60 0000001E           //  4   Bytes
            // Reformat Value from designated address
            unsigned int format = *(int32_t*)Address;

            T A{};
            A = (T)format;
            return A;
        }

        ///---------------------------------------------------------------------------------------------------
        //	[MEMORY]
        // Takes Full Address
        // Make sure to resolve any offsets prior to running this function
        // NOTE: only writes last 4bytes
        template<typename T> 
        static inline void PS2Write(uintptr_t Address, T Patch)
        {
            /// USING FRAMERATE AS AN EXAMPLE
            //0x7FF6B048CF60 0000001E0000001E   //  8   Bytes
            //0x7FF6B048CF60 0000001E           //  4   Bytes
            // Reformat Value from designated address
            *(T*)Address = Patch;
        }

    public:
        Memory();
        ~Memory();

    private:
        static bool                     m_isInitialized;                                //  Set on first constructor call
        static uintptr_t                dwGameBase;                                     //  Process Module Base
        static uintptr_t                dwEEMem;                                        //  EEMem Pointer
        static uintptr_t                BasePS2MemorySpace;                             //  EEMem Base Address
        static ProcessInfo              Process;                                        //  Process Information Struct
        static Memory*                  m_instance;                     //  static class instance
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
            
            AOB: 48 8B 3D ? ? ? ? 80 7F
        */
        static class EmuThread*         g_emu_thread;

        //  
        static class Console*           g_console;
        static class Engine*            g_engine;
        static class Memory*            g_memory;
    
    };

}
#pragma pack(pop)