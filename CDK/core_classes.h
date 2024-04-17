#pragma once

/**
 * Name: PlayStation2 - PCSX2
 * Version: 1.0.0
 * Author: NightFyre
*/

namespace PlayStation2
{
    class CGlobals
    {
    public: 
        static class Console*           g_console;
        static class Engine*            g_engine;
        static class Memory*            g_memory;
    };

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
    
    public:
        static Engine*                 GetDefaultInstance();

    private:
        static Engine*                 m_instance;                     //  static class instance
    };

    class Memory
    {
    public:
        template <typename T>
        static inline T                 ReadMemoryEx(__int64 addr) { return *(T*)addr; }
        
        template<typename T>
        static inline void              WriteMemoryEx(__int64 addr, T patch) { *(T*)addr = patch; }

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

        /*
            Summary: takes an input long address and reads its value
            Notes:
                -   'long' in this sense would be the physical address in memory such as '00007FF6C194DC00'

            Example Usage: Get float Position of Object
                -   PS2Memory::ReadLong<float>(address);
            
            Example Usage: Get Vector3 Position of Object
                -   PS2Memory::ReadLong<Vector3>(address)
        */
        template<typename T>
        static inline T                 ReadLong(__int64 addr) { return *(T*)(addr); }


        /*
            Summary: takes an input long address and writes a new value
            Notes:
                -   'short' in this sense would be the game offset such as'0x4899F0'

            Example Usage: Write new float Position of Object
                -   PS2Memory::WriteLong<float>(offset, patch);

            Example Usage: Write new Vector3 Position of Object
                -   PS2Memory::WriteLong<Vector3>(offset, patch)
        */
        template<typename T>
        static inline void              WriteLong(__int64 addr, T Patch) { *(T*)addr = Patch; }
        
        /*
            Summary: takes an input short offset , transforms it to an address in memory and reads its value
            Notes:
                -   'short' in this sense would be the game offset such as'0x4899F0'

            Example Usage: Get float Position of Object
                -   PS2Memory::ReadShort<float>(offset);

            Example Usage: Get Vector3 Position of Object
                -   PS2Memory::ReadShort<Vector3>(offset)
        */
        template <typename T>
        static inline T                 ReadShort(__int32 offset)  { return ReadLong<T>(GetAddr(offset)); }

        /*
            Summary: takes an input short offset , transforms it to an address in memory and writes a new value
            Notes:
                -   'short' in this sense would be the game offset such as'0x4899F0'

            Example Usage: write new float Position of Object
                -   PS2Memory::WriteShort<float>(offset, patch);

            Example Usage: Write new Vector3 Position of Object
                -   PS2Memory::WriteShort<Vector3>(offset, patch)
        */
        template <typename T>
        static inline bool              WriteShort(__int32 offset, T patch) { return WriteLong<T>(GetAddr(offset), patch); }

        //  takes an input long address , reads the value and casts it to a class pointer
        template <typename T>
        static inline T                GetPtrLong(long long address)
        {
            return reinterpret_cast<T>(GetModuleBase() + ReadLong<__int32>(address));
        }

        //  takes an input short offset , transforms it to an address in memory, reads its value and casts it to a class pointer 
        template <typename T>
        static inline T                GetPtrShort(__int32 offset)
        {
            return reinterpret_cast<T>(GetModuleBase() + ReadShort<__int32>(offset));
        }


    public:
        static __int64                  GetModuleBase();                    //  obtain PS2 EE memory base address. 00007FF6C0000000 
        static __int64                  GetAddr(__int32 offset);            //  transform offset to physical address. 0x44D648 -> 00007FF6C44D648      
        static __int64                  ResolvePtrChain(__int32 base_offset, std::vector<__int32> offsets);
    };

    class Tools
    {
    public:
        
        static bool                     GetKeyState(WORD v, SHORT delta);
        static bool                     GamepadGetButtonState(WORD button);


        class CPUTimer
        {
        public:
            //  Enumeration for specifying different timing units
            enum class ETimings
            {
                ETiming_MS,     //  millisecond
                ETiming_S,      //  second
                ETiming_M,      //  minute
                ETiming_HR,     //  hour
            };

        public:
            void                        Start();            //  Start the timer
            void                        Stop();             //  Stop the timer
            void                        Reset();            //  Reset the timer to start again
            double                      Stop(ETimings t);   // Stop the timer and 
            double                      GetElapsedTime(clock_t time, ETimings t = ETimings::ETiming_MS) const;  //  return the elapsed time in the specified unit //  NOTE: timer is not stopped

        public:
            CPUTimer();                                     // Constructor - starts the timer

        private:
            clock_t                     m_start;            // Start time of the timer
            clock_t                     m_end{ 0 };         // End time of the timer
        };

        class Math3D
        {
            static float GetDistanceTo3DObject(Vector3 POS, Vector3 POS2);
        };

    };

}