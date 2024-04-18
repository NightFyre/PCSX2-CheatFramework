# PCSX2 Cheat Development Kit  
PCSX2 Menu Trainer Development Kit is a C++ library designed to simplify the process of creating cheats for the PCSX2 emulator. 
It aims to bridge the gap between modern cheat development practices and the classic era of cheat codes.
This framework provides tools to port older codes to a more modern approach, using additional libraries such as dear imgui and minhook.

| ![image](https://github.com/NightFyre/PCSX2-CheatFrameWork/assets/80198020/7417d7e7-603c-465a-807f-62abf88179eb) | ![image](https://user-images.githubusercontent.com/80198020/201475737-21591eb0-8858-4575-8ee9-5d9d2e07d1dc.png) | ![image](https://user-images.githubusercontent.com/80198020/200977619-e22fe7a5-b914-4906-9e3c-50f929ea3ebf.png) | 
| :---: | :---: |  :---: |

## FEATURES
- Easily Read & Write to virtual PS2 Memory
- PCSX2 Emulator Process Virtual Method Hooking
- Rendering API Helpers
- Access EE & IOP Registers during execution

## USAGE
1. include the CDK header as well as any required packages.  
*NOTE: Engine_Package.h is a required package and needs to be manually included.*
 - `CDK.h`                  //  Including the SDK header will provide access to all the most necessary features.
 - `CDK.cpp`                //  make sure to include the source file to your project , otherwise attempting to use any methods will result in an "unresolved external error"

2. Initialize the Cheat Device
https://github.com/NightFyre/PCSX2-CheatFramework/blob/4bccdd509105c2238ac33f16fbafd11f3883dc44/CDK.h#L73-L74

3. Do Stuff
https://github.com/NightFyre/PCSX2-CheatFramework/blob/70dcf7dc51747df014f5d537a3188b8de1fe95a2/dllmain.cpp#L18-L29

4. (optional) include game package for additional controls

## USAGE EXAMPLES

- Access EE Memory
https://github.com/NightFyre/PCSX2-CheatFramework/blob/b2ae5a44d7c1452256920c4af81f745cdf0df2f3/examples/GameMemory/dllmain.cpp#L33-L34

- Get Virtual Address
https://github.com/NightFyre/PCSX2-CheatFramework/blob/b2ae5a44d7c1452256920c4af81f745cdf0df2f3/examples/GameMemory/dllmain.cpp#L37-L38

- Read Virtual Memory
https://github.com/NightFyre/PCSX2-CheatFramework/blob/b2ae5a44d7c1452256920c4af81f745cdf0df2f3/examples/GameMemory/dllmain.cpp#L41-L47

- Access Class Members via GamePackages 
```cpp
/// SOCOM 1 -> Access SealObject & Modify Weapon Ammo
{
    // Create an instance of the Offsets Class
    SOCOM1::CZSeal* pLocalSeal = SOCOM1::CZSeal::GetDefaultInstance();
    if (pLocalSeal)
        Console::cLogMsg("[+][PCSX2-CDK][SOCOM] LocalSeal Name: %s\n", EConsoleColors::green, pLocalSeal->GetName().c_str());
}
```

- Hook Rendering API
https://github.com/NightFyre/PCSX2-CheatFramework/blob/7c786cdd3e7f8c0855bc7d1f83df687092424604/examples/Rendering/dllmain.cpp#L44-L51


### References & Credits
- [PCSX2](https://github.com/PCSX2/pcsx2)
- [GameHacking.org](https://gamehacking.org/system/ps2)
- [Sly Cooper Modding Community](https://discord.com/invite/2GSXcEzPJA) 
- [SOCOM Modding Community](https://discord.com/invite/PCJGrwMdUS) 
- [A General Guide for Making Cheats & Trainers for PCSX2](https://www.unknowncheats.me/forum/general-programming-and-reversing/569991-pcsx2-guide-cheats-trainers.html)

### License
This framework is distributed under the MIT License.

### Disclaimer
1. *This framework is intended for educational and single-player use.*  
2. *Use cheats responsibly and respect the terms of use of the games you are modifying.*
