# PCSX2 Menu Trainer Development Kit  
## CODE EXAMPLES
```cpp
/// PCSX2 -> Hook DirectX11 Present
PlayStation2::GSDevice* device = *PlayStation2::CGlobals::g_gs_device;
auto d3d11 = reinterpret_cast<PlayStation2::GSDevice11*>(device);
pSwapChain = d3d11->GetSwapChain();
if (pSwapChain)
{
    //    Get swapchain description
    DXGI_SWAP_CHAIN_DESC desc;
    pSwapChain->GetDesc(&desc);
    printf("[+] PCSX2::GSDevice11::SwapChainDescription -> Obtained!\nWindowHandle: 0x%llX\n\n", desc.OutputWindow);

    //  Hook present (the PCSX2 class itself contains a template hook for accessibility)
    PlayStation2::hkVFunction(pSwapChain, 8, oIDXGISwapChainPresent, hkPresent);
    printf("[+] PCSX2::GSDevice11::Present -> Hooked!\nOriginal: 0x%llX\nDetour: 0x%llX\n\n",
        oIDXGISwapChainPresent, hkPresent
    );
}
```

```cpp
/// SOCOM 1 -> Access SealObject & Modify Weapon Ammo
{
    // Create an instance of the Offsets Class
    SOCOM1::Offsets offsets;               // Initializing the class will auto resolve any of our offsets placed in the header
    auto base = (SOCOM1::CPlayer*)offsets.SEALPointer;  //  Get SEALObject Class
    if (base->isValid())            // Check if our player object is valid, generally the value will be NULL if not in a game   
        base.PrimaryMag1 = 1337;    // Set Ammo Count in MAG1
}
```

| ![image](https://github.com/NightFyre/PCSX2-CheatFrameWork/assets/80198020/7417d7e7-603c-465a-807f-62abf88179eb) | ![image](https://user-images.githubusercontent.com/80198020/201475737-21591eb0-8858-4575-8ee9-5d9d2e07d1dc.png) | ![image](https://user-images.githubusercontent.com/80198020/200977619-e22fe7a5-b914-4906-9e3c-50f929ea3ebf.png) | 
| :---: | :---: |  :---: |

## References & Credits
- [Accessing PCSX2 Memory](https://nightfyre.github.io/PCSX2_Trainer/)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [Minhook Hooking Library](https://github.com/TsudaKageyu/minhook)
- [Sly Cooper Modding Community](https://discord.com/invite/2GSXcEzPJA) 
- [SOCOM Modding Community](https://discord.com/invite/PCJGrwMdUS) 
