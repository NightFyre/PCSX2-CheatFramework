#pragma once

/**
 * Name: PlayStation2 - PCSX2 :: SOCOM U.S NAVY SEALs
 * Version: 0.0.1
 * Author: NightFyre
*/

#pragma pack(push, 0x01)
namespace PlayStation2
{
	namespace SOCOM
	{
		enum WeaponSlot : unsigned int
		{
			Primary,	// = 0x60C,
			Secondary,	// = 0x610,
			EqSlot1,	// = 0x614,
			EqSlot2,	// = 0x618,
			EqSlot3,	// = 0x61C,
		};

		enum Primary : unsigned int
		{
			M4A1_SD = 0xCC3F80,
			AR_552 = 0xCC4550,
			SMG_9MM = 0x00CC61F0,
			abs1 = 0xCC6D90,
		};

		enum Secondary : unsigned int
		{
			PISTOL_9MM = 0xCC2B60,
			_abs = 13402528,
			_abs2 = 13401072,
			_abs3 = 13395872,
			_abs4 = 13377488,
		};

		enum Equipment : unsigned int
		{
			Flashbang = 0x0,
		};

		enum Teams : unsigned int
		{
			SEALS = 0x40000001,			//	Seal
			TERRORIST = 0x80000100,		//	Terrorist
			TURRET = 0x48000000,		//	Turret
			SPECTATOR = 0x00010000,		//	Spectator

			// CAMPAIGN
			SP_ABLE = 0x84000006,			//	Alpha Team
			SP_BRAVO = 0x8400000A,			//	Bravo Team
			SP_ENEMY_A = 0x40000050,		//	Iron Brother / Iron Leader
			SP_ENEMY_B = 0x40000100,		//	
			SP_ENEMY_C = 0x40000210,		//	
			SP_ENEMY_D = 0x40000410,		//	
			SP_ENEMY_E = 0x40000810,		//	
			SP_ENEMY_F = 0x40001010,		//	
			SP_ENEMY_G = 0x40002010,		//	
			SP_ENEMY_H = 0x40004010,		//	
			SP_ENEMY_I = 0x40021010,		//	
		};
	}
}
#pragma pack(pop)