# PCSX2 Menu Trainer Development Kit  
## CODE EXAMPLE
```cpp
/// Access SealObject & Modify Weapon Ammo
{
    // Create an instance of the Offsets Class
    SOCOM1::Offsets offsets;               // Initializing the class will auto resolve any of our offsets placed in the header
    auto base = (SOCOM1::CPlayer*)offsets.SEALPointer;  //  Get SEALObject Class
    if (base->isValid())            // Check if our player object is valid, generally the value will be NULL if not in a game   
        base.PrimaryMag1 = 1337;    // Set Ammo Count in MAG1
}
```
| ![image](https://user-images.githubusercontent.com/80198020/200977503-e4737e4e-9d2d-4b2e-9554-3b8484872e38.png) | ![image](https://user-images.githubusercontent.com/80198020/201475737-21591eb0-8858-4575-8ee9-5d9d2e07d1dc.png) | ![image](https://user-images.githubusercontent.com/80198020/200977619-e22fe7a5-b914-4906-9e3c-50f929ea3ebf.png) |  
| :---: | :---: |  :---: |

## References & Credits
- [Accessing PCSX2 Memory](https://nightfyre.github.io/PCSX2_Trainer/)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [Minhook Hooking Library](https://github.com/TsudaKageyu/minhook)
- [Sly Cooper Modding Community](https://discord.com/invite/2GSXcEzPJA) 
