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
	enum class RenderAPI
	{
		None,
		D3D11,
		Metal,
		D3D12,
		Vulkan,
		OpenGL
	};
}
#pragma pack(pop)