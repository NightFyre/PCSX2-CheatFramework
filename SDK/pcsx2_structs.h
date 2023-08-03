#pragma once
#include "../SDK.h"

/**
 * Name: PlayStation2 - PCSX2
 * Version: 0.0.1
 * Author: NightFyre
*/

#pragma pack(push, 0x01)
namespace PlayStation2
{
	enum RenderAPI
	{
		None	= 0,
		D3D11	= 1,
		D3D12	= 2,
		OpenGL	= 4,
		Vulkan	= 3
	};
}
#pragma pack(pop)