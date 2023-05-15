#pragma once

/**
 * Name: PlayStation2 - PCSX2
 * Version: 0.0.1
 * Author: NightFyre
*/

#pragma pack(push, 0x01)
namespace PlayStation2
{
    class PS2
    {	
    public:
        // Class Initialize
        PS2()
        {
            HWND g_chWnd = GetConsoleWindow();
            if (g_chWnd == NULL)
            {
                AllocConsole();														//  Allocate console for output
                freopen_s(&output_stream, "CONOUT$", "w", stdout);	                //  Establish stream for output to console
                SetConsoleTitleA("PCSX2 FrameWork by NightFyre");					//  Set console title
                g_chWnd = GetConsoleWindow();										//  Store WindowHandle to console
            }
            g_cHandle = GetStdHandle(STD_OUTPUT_HANDLE);							//  Store handle to console
            ShowWindow(GetConsoleWindow(), SW_SHOW);								//	Show console window
            printf("[+] PCSX2-FrameWork::Initialized\n\n");         				//	Print text to our newly created window
        }

        //  Cleanup
        ~PS2()
        {
            if (output_stream == NULL)
                return;

            fclose(output_stream);
            DestroyWindow(g_chWnd);
            FreeConsole();
        }

    private:    //  Nothing should ever need access to these things
	    FILE*			    output_stream;		                                    //  Console Text Output Stream
	    HANDLE			    g_cHandle;			                                    //  Handle to Console
	    HWND			    g_chWnd{};			                                    //  Window Handle to Console
	    HANDLE			    _pipe;				                                    //	Handle to Pipe
    };
    inline std::unique_ptr<PS2> g_PS2;

    class PS2Memory
    {
    public:
        class PS2Tools*                     Tools;                                  //  
        ProcessInfo                         Process;                                //  Process Information Struct
        uintptr_t                           dwGameBase;                             //  Process Module Base
        uintptr_t                           dwEEMem;                                //  EEMem Pointer
        uintptr_t                           BasePS2MemorySpace;                     //  EEMem Base Address

    public:
        bool                                isNullPtr();
        uintptr_t                           GetAddr(unsigned int RAW_PS2_OFFSET);
        uintptr_t                           GetClassPtr(unsigned int RAW_PS2_OFFSET);
        uintptr_t                           ResolvePtrChain(unsigned int RAW_PS2_OFFSET, std::vector<unsigned int> offsets = {});
        bool                                BytePatch(uintptr_t Address, BYTE* bytes, unsigned int size);
        bool                                Nop(uintptr_t Address, unsigned int size);
        
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


    public:
        PS2Memory() 
        {
            Process.Initialize();
            dwGameBase          = (uintptr_t)Process.m_ModuleBase;
            dwEEMem             = (uintptr_t)GetProcAddress((HMODULE)Process.m_ModuleHandle, "EEmem");
            BasePS2MemorySpace  = *(uintptr_t*)dwEEMem;
        }
        ~PS2Memory() {}
    };
    inline std::unique_ptr<PS2Memory> g_PS2Mem;

    class PS2Tools
    {
    public:
        float GetDistanceTo3DObject(Vector3 POS, Vector3 POS2);
    };
}
#pragma pack(pop)