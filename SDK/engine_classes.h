#pragma once

/**
 * Name: PlayStation2 - PCSX2
 * Version: 0.0.1
 * Author: NightFyre
*/

#pragma pack(push, 0x01)
namespace PlayStation2
{
    class Engine
    {
    public:
        static class PCSX2*             g_PCSX2;                                        //  global accessor for PCSX2 member classes , members & methods
        static class Memory*            g_Mem;                                          //  memory helpers
        static class Tools*             g_Tools;                                        //  all other helpers

    public:
        void                            CreateConsole(const char* title, bool bShow = true); //  creates a console instance for debug output
        void                            DestroyConsole();                               //  destroys the created console instance
        bool                            isConsolePresent();                             //  

        
    private:
	    FILE*			                p_fin_stream;		                            //  Console Text Input Stream
	    FILE*			                p_fout_stream;		                            //  Console Text Output Stream
	    FILE*			                p_ferr_stream;		                            //  Console Text Error Stream
	    HANDLE			                p_console_handle;			                    //  Handle to Console
	    HWND			                p_wndw_handle;			                        //  Window Handle to Console
	    HANDLE			                p_pipe_handle;				                    //	Handle to Pipe
        bool                            p_bConsole{ 0 };                                //  flag for console creation
        bool                            p_bShowWindow{ 0 };                             //  flag for displaying console window
    };
    inline std::unique_ptr<Engine> g_Engine;

    class Memory
    {
    public:
        ProcessInfo                     Process;                                        //  Process Information Struct

    public:
        bool                            ObtainProcessInfo(ProcessInfo& pInfo);
        uintptr_t                       GetPS2Address(unsigned int RAW_PS2_OFFSET);
        uintptr_t                       DereferencePtr(unsigned int RAW_PS2_OFFSET);
        uintptr_t                       ResolvePtrChain(unsigned int RAW_PS2_OFFSET, std::vector<unsigned int> offsets = {});
        bool                            BytePatch(uintptr_t Address, BYTE* bytes, unsigned int size);
        bool                            NopBytes(uintptr_t Address, unsigned int size);
        Memory();
        ~Memory();
        
        ///---------------------------------------------------------------------------------------------------
        //	[MEMORY]
        // Takes Full Address
        // Make sure to resolve any offsets prior to running this function
        // NOTE: only reads last 4bytes
        template<typename T> inline T PS2Read(uintptr_t Address)
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
        template<typename T> inline void PS2Write(uintptr_t Address, T Patch)
        {
            /// USING FRAMERATE AS AN EXAMPLE
            //0x7FF6B048CF60 0000001E0000001E   //  8   Bytes
            //0x7FF6B048CF60 0000001E           //  4   Bytes
            // Reformat Value from designated address
            *(T*)Address = Patch;
        }


    private:
        uintptr_t                       dwGameBase;                             //  Process Module Base
        uintptr_t                       dwEEMem;                                //  EEMem Pointer
        uintptr_t                       BasePS2MemorySpace;                     //  EEMem Base Address
    };

    class Tools
    {
    public:
        float GetDistanceTo3DObject(Vector3 POS, Vector3 POS2);
    };
   
    class CGlobals
    {
    public:

        /*
            __int64 GSUpdateDisplayWindow() -> g_gs_renderer
            .text:000000014015095F                 mov     rcx, cs:g_gs_renderer
            AOB: 48 8B 0D 32 42 F8 03
        */
        static class GSRenderer**       g_gs_renderer;

        /*
            __int64 GSUpdateDisplayWindow() -> g_gs_device
            .text:00000001401508C5                 mov     rax, cs:g_gs_device
            AOB: 48 8B 05 D4 41 F8 03
        */
        static class GSDevice**         g_gs_device;

        /*
            __int64 __fastcall OpenGSDevice(char a1, unsigned int a2) -> g_emu_thread
            .text:000000014035C32F                 mov     rcx, cs:g_emu_thread
            AOB: 48 8B 0D A2 48 9D 0D
        */
        static class EmuThread**        g_emu_thread;
    
    };

}
#pragma pack(pop)