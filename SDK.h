#pragma once
// --------------------------------------- \\
//      SDK Created By ( NightFyre )       \\
// --------------------------------------- \\
// Name: PlayStation2 - PCSX2, Version: 0.0.1

// Common Headers required for SDK files
#include <windows.h>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>

//	RENDERING APIS
#include <d3d11.h>

//	Include SDK Files
#include "SDK/engine_package.h"	//	Core PS2 Helper Classes, Structs & Functions
#include "SDK/pcsx2_package.h"	//	Core PCSX2 Classes , Structs & Functions

// Custom Game Packages 
// - review readme and examples for better understanding with including a game package.
//	#include "SDK/GamePackage/SOCOM1/SOCOM1_package.h"
//	#include "SDK/GamePackage/SlyCooper1/SlyCooper_Package.h"