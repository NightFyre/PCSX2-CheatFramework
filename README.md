# PCSX2 Menu Trainer Development Kit  
PCSX2 Menu Trainer Development Kit is a C++ library designed to simplify the process of creating cheats for the PCSX2 emulator. 
It aims to bridge the gap between modern cheat development practices and the classic era of cheat codes.
This framework provides tools to port older codes to a more modern approach, using additional libraries such as dear imgui and minhook.

| ![image](https://github.com/NightFyre/PCSX2-CheatFrameWork/assets/80198020/7417d7e7-603c-465a-807f-62abf88179eb) | ![image](https://user-images.githubusercontent.com/80198020/201475737-21591eb0-8858-4575-8ee9-5d9d2e07d1dc.png) | ![image](https://user-images.githubusercontent.com/80198020/200977619-e22fe7a5-b914-4906-9e3c-50f929ea3ebf.png) | 
| :---: | :---: |  :---: |

## GENERAL USAGE
1. include the SDK header as well as any required packages.  
*NOTE: Engine_Package.h is a required package and needs to be manually included.*
 - `SDK.h`                  //  Including the SDK header will provide access to all the most necessary features.
 - `Engine_Package.cpp`     //  Core Package and must be included for basic usage 
 - `PCSX2_Package.cpp`      //  Contains PCSX2 specific members and functions. Like the rendering API's for instance

2. Initialize the SDK
 - `PlayStation2::InitSDK();`

 3. Do Stuff
 https://github.com/NightFyre/PCSX2-CheatFrameWork/blob/1cdaa7f5a22868f86a9a7beeb7a637c6f58a8f8e/dllmain.cpp#L8-L44

## CODE EXAMPLES

- Get Physical Address
```cpp
//  -   Example Pnach: Save Anywhere by Codejunkies ( Kingdom Hearts (NTSC-UC) )
//  -   patch=1,EE,204865E0,extended,00114288
//  - Raw Address   -> 204865E0

// Extract offset from raw address (remove "0x20")
unsigned int offset = 0x4865E0;

//  Get Physical Address
auto addr = PlayStation2::Engine::g_Mem->GetPS2Address(offset);
```

- Read Memory
```cpp
// Read Memory
//  -   Example Offset: Tidus Health - FFX
//  -   0x10DF34C

//  Get Offset Physical Address
auto addr = PlayStation2::Engine::g_Mem->GetPS2Address(0x10DF34C);

//  Read Memory
auto result = PlayStation2::Engine::g_Mem->PS2Read<int>(addr);
```

- Write Memory
```cpp
//  Write Memory
//  -   Example Pnach: Infinite Health by Codejunkies ( Star Wars Battlefront )
//  -   patch=1,EE,20519060,extended,00000001
//  - Raw Address   -> 20519060
//  - Patch         -> 00000001

//  Get Physical Address
auto addr = PlayStation2::Engine::g_Mem->GetPS2Address(0x519060);

//  Write Memory
PlayStation2::Engine::g_Mem->PS2Write<int>(addr, 1);
```

## ADVANCED TECHNIQUES
- Access Class Members via GamePackages 
```cpp
/// SOCOM 1 -> Access SealObject & Modify Weapon Ammo
{
    // Create an instance of the Offsets Class
    SOCOM1::Offsets offsets;                            // Initializing the class will auto resolve all offsets placed in the class constructor
    auto base = (SOCOM1::CPlayer*)offsets.SEALPointer;  //  Get SEALObject Class
    if (base->isValid())                                // Check if player object is valid, generally the value will be NULL if not in a game   
        base.PrimaryMag1 = 1337;                        // Set Ammo Count in MAG1
}
```

- Hook Rendering API
```cpp
/// PCSX2 -> Hook DirectX11 Present
PlayStation2::GSDevice* device = *PlayStation2::CGlobals::g_gs_device;              //  Obtains the current render api device instance 
auto d3d11 = reinterpret_cast<PlayStation2::GSDevice11*>(device);                   //  Cast D3D11 Rendering Device
pSwapChain = d3d11->GetSwapChain();                                                 //  Obtain SwapChain Pointer
if (pSwapChain)
    PlayStation2::hkVFunction(pSwapChain, 8, oIDXGISwapChainPresent, hkPresent);    //  Hook present 
```

## References & Credits
- [PCSX2](https://github.com/PCSX2/pcsx2)
- [GameHacking.org](https://gamehacking.org/system/ps2)
- [Sly Cooper Modding Community](https://discord.com/invite/2GSXcEzPJA) 
- [SOCOM Modding Community](https://discord.com/invite/PCJGrwMdUS) 
- [Dear ImGui](https://github.com/ocornut/imgui)
- [Minhook Hooking Library](https://github.com/TsudaKageyu/minhook)

## License
This framework is distributed under the MIT License.

## Disclaimer
1. *This framework is intended for educational and single-player use.*  
2. *Use cheats responsibly and respect the terms of use of the games you are modifying.*