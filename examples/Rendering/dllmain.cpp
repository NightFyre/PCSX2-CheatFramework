#pragma once
#include <CDK.h>
static bool g_running;

//---------------------------------------------------------------------------------------------------
//  fwd declare rendering helpers
void InitImGui();
void RenderImGui();
LRESULT WndProc(const HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

//---------------------------------------------------------------------------------------------------
//  Initialize Client Thread
DWORD WINAPI MainThread(LPVOID hInstance)
{

#if _DEBUG

    MessageBoxA(NULL, "Initializing PCSX2 Cheat Device", "DEBUG BUILD", MB_OK | MB_ICONINFORMATION);

#endif    

    //  initialize pcsx2 cheat dev kit
    if (PlayStation2::InitCDK("pcsx2-qt.exe", true))
    {
        //  Obtain global instance of GSDevice
        PlayStation2::PCSX2::g_gs_device = reinterpret_cast<PlayStation2::GSDevice*>(*(__int64*)(PlayStation2::Memory::GetAddr(PlayStation2::PCSX2::o_gs_device))); //  offset because have not implemented reading assembly op code for variable , must be shrunk
        if (PlayStation2::PCSX2::g_gs_device)
        {
            //  Get RenderAPI in use
            if (PlayStation2::GSDevice::GetRenderAPI() == PlayStation2::RenderAPI::D3D11)
            {
                // Get GSDevice11*
                auto d3d11 = static_cast<PlayStation2::GSDevice11*>(PlayStation2::PCSX2::g_gs_device);

                if (d3d11)
                {
                    //  Get SwapChain & Hook
                    PlayStation2::PCSX2::pSwapChain = d3d11->GetSwapChain();

                    if (PlayStation2::PCSX2::pSwapChain)
                    {
                    
                        PlayStation2::Memory::hkVFunction(PlayStation2::PCSX2::pSwapChain, PlayStation2::DXGI::IDXGI_PRESENT, PlayStation2::PCSX2::oDXGISwapChainPresent, PlayStation2::PCSX2::hkPresent);
                        PlayStation2::Memory::hkVFunction(PlayStation2::PCSX2::pSwapChain, PlayStation2::DXGI::IDXGI_RESIZE_BUFFERS, PlayStation2::PCSX2::oIDXGIResizeBuffers, PlayStation2::PCSX2::hkResizeBuffers);
                        InitImGui();

                    }
                }
            }
            else
                MessageBoxA(NULL, "[ERROR] D3D11 NOT PRESENT", "Couldn't detect D3D11 as the active renderer.\nPlease switch to D3D11 before proceeding.", MB_OK | MB_ICONEXCLAMATION);
        }

        g_running = true;

        //  Loop
        do
        {
            //  Exit Module
            if (PlayStation2::Tools::GetKeyState(VK_END, 500))
                g_running = false;

        } while (g_running);
        
        //  Remove Hook
        if (PlayStation2::PCSX2::pSwapChain)
        {
            //  Restore WndProc
            SetWindowLongPtrA(PlayStation2::PCSX2::RenderWindow, GWLP_WNDPROC, (LONG_PTR)PlayStation2::PCSX2::oWndProc);

            //  Restore Hooks
            PlayStation2::Memory::hkRestoreVFunction(PlayStation2::PCSX2::pSwapChain, PlayStation2::DXGI::IDXGI_PRESENT, PlayStation2::PCSX2::oDXGISwapChainPresent);
            PlayStation2::Memory::hkRestoreVFunction(PlayStation2::PCSX2::pSwapChain, PlayStation2::DXGI::IDXGI_RESIZE_BUFFERS, PlayStation2::PCSX2::oIDXGIResizeBuffers);
        }

        //  cleanup
        PlayStation2::ShutdownCDK(); 
    }

    //  Exit
    FreeLibraryAndExitThread((HMODULE)hInstance, EXIT_SUCCESS);
    
    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------------------------------
//  EntryPoint
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwCallReason, LPVOID x)
{
    UNREFERENCED_PARAMETER(x);

    if (dwCallReason == DLL_PROCESS_ATTACH)
    {

        DisableThreadLibraryCalls(hModule);

        HANDLE pHand = CreateThread(0, 0, MainThread, hModule, 0, 0);

        if (pHand)
            CloseHandle(pHand);

    }

    return TRUE;
}

//---------------------------------------------------------------------------------------------------
//  window message handler
LRESULT WndProc(const HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{

    //  Handle window message events when the window is shown
    if (PlayStation2::PCSX2::isMenuShown)
    {
        ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);

        return true;
    }

    return CallWindowProcA(PlayStation2::PCSX2::oWndProc, hwnd, msg, wparam, lparam);
}

//---------------------------------------------------------------------------------------------------
//  initialize dear imgui
void InitImGui()
{
    if (SUCCEEDED(PlayStation2::PCSX2::pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&PlayStation2::PCSX2::D3D11Device)))
    {
        PlayStation2::PCSX2::D3D11Device->GetImmediateContext(&PlayStation2::PCSX2::D11DeviceCtx);
        DXGI_SWAP_CHAIN_DESC sd;
        PlayStation2::PCSX2::pSwapChain->GetDesc(&sd);

        PlayStation2::PCSX2::RenderWindow = sd.OutputWindow;

        ID3D11Texture2D* buffer;
        PlayStation2::PCSX2::pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&buffer);
        PlayStation2::PCSX2::D3D11Device->CreateRenderTargetView(buffer, NULL, &PlayStation2::PCSX2::D11RenderTarget);

        buffer->Release();
        PlayStation2::PCSX2::oWndProc = (WNDPROC)SetWindowLongPtrA(PlayStation2::PCSX2::RenderWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

        ImGui::CreateContext();

        auto& io = ImGui::GetIO();
        io.LogFilename = nullptr;
        io.IniFilename = nullptr;

        ImGui_ImplWin32_Init(PlayStation2::PCSX2::RenderWindow);
        ImGui_ImplDX11_Init(PlayStation2::PCSX2::D3D11Device, PlayStation2::PCSX2::D11DeviceCtx);

        PlayStation2::PCSX2::isImGuiInit = true;

        PlayStation2::PCSX2::RegisterRenderFunction(RenderImGui);
    }
}

//---------------------------------------------------------------------------------------------------
//  render gui window
void RenderImGui()
{

    //  Listen for Menu Show Key
    if (PlayStation2::Tools::GetKeyState(VK_INSERT, 500))
        PlayStation2::PCSX2::isMenuShown ^= 1;

    //  Early exit if the menu is not meant to be rendered
    if (!PlayStation2::PCSX2::isMenuShown)
        return;

    //   Render ImGui
    if (ImGui::Begin("[ PCSX2 ] Cheat Device", nullptr, 96))
    {
        ImGui::Text("Hello World!");

        //  Display PCSX2 Process Information

        //  Display PS2 EE Mem Address

        ImGui::End();
    }
}