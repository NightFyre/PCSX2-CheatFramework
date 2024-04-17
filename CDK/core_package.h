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
	bool InitCDK(const std::string& moduleName);
	bool InitCDK(const std::string& moduleName, unsigned int gRenderer);
	bool InitCDK();
	void ShutdownCDK();
}

#include "core_Structs.h"
#include "core_Classes.h"