#pragma once

/**
 * Name: PlayStation2 - PCSX2
 * Version: 1.0.0
 * Author: NightFyre
*/

namespace PlayStation2
{
	// --------------------------------------------------
	// # Global functions
	// --------------------------------------------------
	bool InitSDK(const std::string& moduleName, unsigned int gRenderer);
	bool InitSDK();
	void ShutdownSDK();
}

#include "engine_Structs.h"
#include "engine_Classes.h"