#pragma once

/**
 * Name: PlayStation2 - PCSX2
 * Version: 0.0.1
 * Author: NightFyre
*/

#pragma pack(push, 0x01)
namespace PlayStation2
{
    class PCSX2
    {	
    public:
        
        /*
            -   
        */
        static unsigned int o_gs_device;                                                //  global pointer to GSDevice  -> PCSX2 v1.7.5617: 0x3FA2728
        
        
        /*
            -
        */
        static unsigned int o_GSDevice_GetRenderAPI;                                    //  offset to function  //  GSDevice::vfIndex [9]
        typedef RenderAPI(__fastcall* GSDevice_GetRenderAPI_stub)(GSDevice*);           //  Returns the graphics API used by this device.

        /*
            -
        */
        static unsigned int o_GSUpdateWindow;                                           //  offset to function  //   GSDevice::vfIndex [12]
        typedef __int64(__fastcall* GSUpdateDisplayWindow_stub)();                      //  [ Nightly AOB: 48 83 EC 48 48 8B 0D ? ? ? ? 48 ] [ Soource AOB: 48 83 EC 48 48 8B 0D ? ? ? ? 48 8B ]

        /*
            -   
        */
        static unsigned int o_psxRecompileInstruction;                                  //  offset to function  ->  PCSX2 v1.7.5617: 0x269D80
        typedef void(__fastcall* psxRecompileNextInstruction_stub)(bool, bool);         //  [ Nightly AOB: E8 ? ? ? ? 8B 05 ? ? ? ? 8B 0D ? ? ? ? 85 ]  [ Source AOB: E8 ? ? ? ? 8B 15 ? ? ? ? 85 D2 75 ]
        
        /*
            -
        */
        static unsigned int o_recompileNextInstruction;                                 //  offset to function  ->  PCSX2 v1.7.5617: 0x291CA0
        typedef void(__fastcall* recompileNextInstruction_stub)(bool, bool);            //  AOB: [ Nightly AOB: E8 ? ? ? ? C7 44 24 ? ? ? ? ? 49 ]  [ Source AOB: ~ ] [ string: xref "Applying Dynamic Patch to address 0x%08X" ]
        
        /*
            -   
        */
        static unsigned int o_recResetEE;                                               //  offset to function -> PCSX2 v1.7.5617: 0x2942C0
        typedef void(__fastcall* recResetEE_stub)();                                    //  [ Nightly AOB: 80 3D ?? ?? ?? ?? ?? 75 30 C6 05 ?? ?? ?? ?? ?? C6 ]  [ Source AOB: 80 3D ? ? ? ? ? 74 3D 80 ]
        static void ResetEE();                                                          //  helper function utilizing the offsets and prototype fn

        /*

            //  Function: recompileNextInstruction
            //  AOB: [ Nightly AOB: E8 ? ? ? ? C7 44 24 ? ? ? ? ? 49 ]  [ Source AOB: ~ ]
            //  .text:000000014029313B                 mov     cs:dword_142EA8F2C, eax
            //  dword_142EA8F2C = v157;;           // cpuRegs.code // 0x2AC
        */
        static unsigned int o_cpuRegs;      //      offset  ->  PCSX2 v1.7.5617: 0x2EA8F2C
        static cpuRegisters* g_cpuRegs;     //      iR5900
        static __int32 g_cpupc;             //      offset  ->  PCSX2 v1.7.5617: 0      //  @TODO:have not determined a method for obtaining


        /*

            //  Function: psxRecompileNextInstruction
            //  AOB: [ Nightly AOB: E8 ? ? ? ? 8B 05 ? ? ? ? 8B 0D ? ? ? ? 85 ]  [ Source AOB: E8 ? ? ? ? 8B 15 ? ? ? ? 85 D2 75 ]
            //  .text:0000000140269D96                 mov     r15d, cs:dword_142EA809C
            //  v3 = dword_142EA809C;           // psxRegs.code // 0x20C
        */
        static unsigned int o_psxRegs;      //      offset  ->  PCSX2 v1.7.5617: 0x2EA809C
        static psxRegisters* g_psxRegs;     //      iR3000A
        static __int32 g_psxpc;             //      offset  ->  PCSX2 v1.7.5617: 0      //  @TODO:have not determined a method for obtaining




        //  DEPRECATED: pcsx2 1.6
    private:    //  ~
        uintptr_t hk_OnLeftDClick   = NULL; //  Signature("48 8B 05 ? ? ? ? 80 B8 ? ? ? ? ? 74 0C").Scan().As<uint64_t>();
        uintptr_t hk_ResetEE        = NULL; //  Signature("80 3D ? ? ? ? ? 74 13 B8 ? ? ? ? 86").Scan().As<uint64_t>();	// 80 3D ? ? ? ? ? 75 30 C6 05 ? ? ? ? ? C6
    };

    class GSRenderer
    {
    };

    /*
    
        -
    */
    class GSRendererHW
    {
    };
    
    /*
        
        -   
    */
    class GSDevice
    {
    private:
        //  - No member variables can be placed in this class due to spacing.
        //  -- instead functions will be used to obtain variables as this does not change the size of the class or inherited classes.

    public:
        static RenderAPI                GetRenderAPI();         //  Obtain the current render api. Should be used to get the current class instance of GDevice


    private:
        virtual void                    vf_CreateSurface();
        virtual void                    vf_Function1();
        virtual void                    vf_Function2();
        virtual void                    vf_DoFXAA();
        virtual void                    vf_Function4();
        virtual void                    vf_Function5();
        virtual void                    vf_Function6();
        virtual void                    vf_Create();
        virtual void                    vf_Destroy();
        virtual void                    vf_GetRenderAPI();
        virtual void                    vf_HasSurface();
        virtual void                    vf_Function11();
        virtual void                    vf_UpdateWindow();
        virtual void                    vf_ResizeWindow();
        virtual void                    vf_SupportsExclusiveFullscreen();
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
    };

    /*
        GSDevice11 : DirectX11 Rendering API
        - 0x0 = vfTable
    */
    class GSDevice11 : public GSDevice
    {
    private:
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

    public:
        bool                            isValidSwapChain();
        IDXGISwapChain*                 GetSwapChain();
        ID3D11Device*                   GetDevice();
        DXGI_SWAP_CHAIN_DESC            GetDesc();
    };  //Size: 0x0138

    /*
        GSDevice12 : DirectX12 Rendering API
        - 0x0 = vfTable
    */
    class GSDevice12 : public GSDevice
    {
    private:
        char                            pad_0008[56];	        //0x0000
        __int64                         m_window_info;	        //0x0040
        char                            pad_0048[8];	        //0x0048
        __int64                         window_hwnd;	        //0x0050
        char                            pad_0058[1264];	        //0x0058
        class IDXGIFactory*             m_dxgi_factory;	        //0x0548
        class IDXGISwapChain*           m_swap_chain;	        //0x0550
        char                            pad_0558[40];	        //0x0558

    public:
        bool isValidSwapChain();
        IDXGISwapChain* GetSwapChain();
    };	//Size: 0x055C


    /*
        GSDeviceOGL : OpenGL Rendering API
        - 0x0 = vfTable
    */
    class GSDeviceOGL : public GSDevice
    {

    };

    /*
        GSDeviceVK : Vulkan Rendering API
        - 0x0 = vfTable
    */
    class GSDeviceVK : public GSDevice
    {

    };

}
#pragma pack(pop)