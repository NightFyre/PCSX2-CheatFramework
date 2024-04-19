#pragma once
#include <CDK.h>
static bool g_running;

//---------------------------------------------------------------------------------------------------
//  fwd declare rendering helpers
void HUD();
void MENU();
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

        ImGuiIO& io = ImGui::GetIO(); 
        //  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     // Enable GamePad Controls
        io.LogFilename = nullptr;
        io.IniFilename = nullptr;

        ImGui::StyleColorsClassic();

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

    //  Render GUI Menu
    if (PlayStation2::PCSX2::isMenuShown)
        MENU();

    //  Render Heads Up Display
    //  HUD();

}


//---------------------------------------------------------------------------------------------------
//  render gui window
void MENU()
{

    //   Render ImGui
    if (!ImGui::Begin("[ PCSX2 ] Cheat Device", nullptr))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("Hello World!");

    //  Display PCSX2 Process Information
    ImGui::Text("PCSX2ModuleBase: 0x%llX", PlayStation2::Memory::GetModuleBase());
    ImGui::Text("PS2ModuleBase: 0x%llX", PlayStation2::PS2Memory::GetModuleBase());
    ImGui::Text("GSDevice: 0x%llX", PlayStation2::PCSX2::g_gs_device);
    ImGui::Text("D3D11 SwapChain: 0x%llX", PlayStation2::PCSX2::pSwapChain);

    static int BytesPerLine{ 4 };
    static int MemReadSize{ 0x1000 };
    //  ImGui::InputInt("Memory Read Size:", &MemReadSize, 1, 1);
    ImGui::BeginChild("MemoryScanner", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border);
    {
        int pad{ 0 };
        for (int i = 0; i < MemReadSize; i += BytesPerLine)
        {
            static __int64 base = PlayStation2::PS2Memory::GetModuleBase();
            if (!base)
            {
                ImGui::Text("Failed to get module base address."); 
                break;
            }

            ImGui::Text("%p: ", (unsigned long long)(base + i));
            ImGui::SameLine();
            for (int j = 0; j < BytesPerLine ; j++)
            {
                ImGui::Text("0x%02X ", PlayStation2::PS2Memory::ReadLong<unsigned __int8>(base + i + j));
                ImGui::SameLine();
            }
            ImGui::NewLine();
        }
    }
    ImGui::EndChild();
    ImGui::End();

}

//---------------------------------------------------------------------------------------------------
//  render gui window
void HUD()
{

    auto io = ImGui::GetIO();
    ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetWindowSize(ImGui::GetMainViewport()->Size);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, NULL);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 1.00f, 0.00f));
    if (!ImGui::Begin("[ PCSX2 ] Cheat Device Heads Up Display", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs))
    {
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();
        ImGui::End();
        return;
    }
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar();

    auto ImDraw = ImGui::GetBackgroundDrawList();
    auto draw_size = ImGui::GetMainViewport()->Size;
    auto center = ImVec2({ draw_size.x * .5f, draw_size.y * .5f });
    auto top_center = ImVec2({ draw_size.x * .5f, draw_size.y * 0.0f });

    const char* text = "[ PCSX2 ] CHEAT DEVICE";
    float textSize = ImGui::CalcTextSize(text).x;
    ImVec2 textPosition = ImVec2(top_center.x - (textSize * 0.5f), top_center.y);
    ImDraw->AddText(ImGui::GetFont(), 16.f, textPosition, IM_COL32_WHITE, text, text + strlen(text), 800, 0);

    ImGui::End();

}