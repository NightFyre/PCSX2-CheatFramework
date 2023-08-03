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
        static class PS2*           g_PS2;
        static class PS2Memory*     g_Mem;
        static class PS2Tools*      g_Tools;
    };
    inline Engine* g_Engine;

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

    class PS2Memory
    {
    public:
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

    class PS2Tools
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
        static class GSRenderer**   g_gs_renderer;

        /*
            __int64 GSUpdateDisplayWindow() -> g_gs_device
            .text:00000001401508C5                 mov     rax, cs:g_gs_device
            AOB: 48 8B 05 D4 41 F8 03
        */
        static class GSDevice**     g_gs_device;

        /*
            __int64 __fastcall OpenGSDevice(char a1, unsigned int a2) -> g_emu_thread
            .text:000000014035C32F                 mov     rcx, cs:g_emu_thread
            AOB: 48 8B 0D A2 48 9D 0D
        */
        static class EmuThread**    g_emu_thread;
    
    };

    class GSRenderer
    {
    };

    class GSRendererHW
    {
    };

    class GSDevice
    {

    };

    /*
        GSDevice11 : DirectX11 Rendering API
        - 0x0 = vfTable
    */
    class GSDevice11 : public GSDevice
    {
        char                            pad_0008[56];	        //0x0000
        unsigned int                    m_window_info;	        //0x0040
        char                            pad_0044[12];	        //0x0044
        HWND                            wndw_hwnd;	            //0x0050
        char                            pad_0058[32];	        //0x0058
        class GSTexture11*              m_texture;	            //0x0078
        char                            pad_0080[80];	        //0x0080
        class IDXGIFactory*             m_dxgi_factory;	        //0x00D0	
        class ID3D11Device*             m_dev;	                //0x00D8
        class ID3D11DeviceContext*      m_ctx;                  //0x00E0
        char                            pad_00E8[8];	        //0x00E8
        class IDXGISwapChain*           m_swap_chain;	        //0x00F0	
        class ID3D11RenderTargetView*   m_swap_chain_rtv;	    //0x00F0	
        char                            pad_00F8[60];	        //0x00F8
        bool                            N00000BBA;	            //0x0134
        bool                            RequestedExclusiveFullscreenMode;	//0x0135
        bool                            N00000BBF;	            //0x0136
        bool                            m_is_exclusive_fullscreen;	//0x0137

    private:
        virtual void                    vf_CreateSurface();
        virtual void                    vf_Function1();
        virtual void                    vf_Function2();
        virtual void                    vf_DoFXAA();
        virtual void                    vf_Function4();
        virtual void                    vf_Function5();
        virtual void                    vf_Function6();
        virtual void                    vf_Create();
        virtual void                    vf_Function8();
        virtual void                    vf_Function9();
        virtual void                    vf_Function10();
        virtual void                    vf_Function11();
        virtual void                    vf_UpdateWindow();
        virtual void                    vf_ResizeWindow();
        virtual void                    vf_Function14();
        virtual void                    vf_Function15();
        virtual void                    vf_Function16();
        virtual void                    vf_Function17();
        virtual void                    vf_GetHostRefreshRate();
        virtual void                    vf_GetDriverInfo();
        virtual void                    vf_Function20();
        virtual void                    vf_Function21();
        virtual void                    vf_Function22();
        virtual void                    vf_Function23();
        virtual void                    vf_Function24();
        virtual void                    vf_Function25();
        virtual void                    vf_Function26();
        virtual void                    vf_Function27();
        virtual void                    vf_Function28();
        virtual void                    vf_Function29();
        virtual void                    vf_Function30();
        virtual void                    vf_Function31();
        virtual void                    vf_Function32();
        virtual void                    vf_RenderHW();
        virtual void                    vf_Function34();
    };  //Size: 0x0138

    class GSDevice12 : public GSDevice
    {
        char                        pad_0008[56];	        //0x0000
        __int64                     m_window_info;	        //0x0040
        char                        pad_0048[8];	        //0x0048
        __int64                     window_hwnd;	        //0x0050
        char                        pad_0058[1264];	        //0x0058
        class IDXGIFactory*         m_dxgi_factory;	        //0x0548
        class IDXGISwapChain*       m_swap_chain;	        //0x0550
        char                        pad_0558[40];	        //0x0558
    };	//Size: 0x055C


    class GSDeviceOGL : public GSDevice
    {

    };

    class GSDeviceVK : public GSDevice
    {

    };

}
#pragma pack(pop)