#pragma once

/**
 * Name: PlayStation2 - PCSX2
 * Version: 0.0.1
 * Author: NightFyre
*/

#pragma pack(push, 0x01)
namespace PlayStation2
{
	// --------------------------------------------------
	// # Forwards
	// --------------------------------------------------

	// --------------------------------------------------
	// # Global functions
	// --------------------------------------------------
	bool InitSDK(const std::string& moduleName, unsigned int gRenderer, unsigned int gDevice, unsigned int gEmu);
	bool InitSDK();
	void ShutdownSDK();
}
#pragma pack(pop)

#include "engine_Structs.h"
#include "engine_Classes.h"