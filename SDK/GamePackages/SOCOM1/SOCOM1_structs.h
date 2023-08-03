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
		enum EWeaponSlot : uint32_t
		{
			Primary,		// = 0x60C,
			Secondary,		// = 0x610,
			EqSlot1,		// = 0x614,
			EqSlot2,		// = 0x618,
			EqSlot3,		// = 0x61C,
		};
		static const char* AvailSlots[5] = 
		{ 
			"Primary", 
			"Secondary", 
			"Equipment 1", 
			"Equipment 2", 
			"Equipment 3" 
		};

		enum EPrimaryWeapon : uint32_t
		{
			M4A1_SD		= 0xCC3F80,			
			AR_552		= 0xCC4550,			
			SMG_9MM		= 0x00CC61F0,		
			abs1		= 0xCC6D90,			
		};
		static const char* PrimaryWeapons[4] = 
		{ 
			"M4A1 SD",						//	0
			"552", 							//	1
			"9mm Sub",						//	2
			"abs" 							//	3
		};

		enum ESecondaryWeapon : uint32_t
		{
			PISTOL_9MM	= 0xCC2B60,		
			_abs		= 13402528,		
			_abs2		= 13401072,		
			_abs3		= 13395872,		
			_abs4		= 13377488,		
		};
		static const char* SecondaryWeapons[5] = 
		{ 
			"9MM Pistol",					//	0
			"abs", 							//	1
			"abs2", 						//	2
			"abs3", 						//	3
			"abs4" 							//	4
		};

		enum EEquipment : uint32_t
		{
			Flashbang	= 0x0,
		};
		static const char* EquipmentWeapons[1] = 
		{ 
			"Flashbang"						//	0
		};

		enum EAmmoType : uint32_t
		{

		};
		static const char* AmmoTypes[1] = 
		{ 
			"NULL"							//	0
		};

		enum ETeam : uint32_t
		{
			SEALS		= 0x40000001,		//	Seal
			TERRORIST	= 0x80000100,		//	Terrorist
			TURRET		= 0x48000000,		//	Turret
			SPECTATOR	= 0x00010000,		//	Spectator

			// CAMPAIGN
			SP_ABLE		= 0x84000006,		//	Alpha Team
			SP_BRAVO	= 0x8400000A,		//	Bravo Team
			SP_ENEMY_A	= 0x40000050,		//	Iron Brother / Iron Leader
			SP_ENEMY_B	= 0x40000100,		//	
			SP_ENEMY_C	= 0x40000210,		//	
			SP_ENEMY_D	= 0x40000410,		//	
			SP_ENEMY_E	= 0x40000810,		//	
			SP_ENEMY_F	= 0x40001010,		//	
			SP_ENEMY_G	= 0x40002010,		//	
			SP_ENEMY_H	= 0x40004010,		//	
			SP_ENEMY_I	= 0x40021010,		//	
		};
		static const char* TeamNames[5] =
		{
			"SEALS",			//	0
			"TERRORISTS",		//	1
			"SPECTATOR",		//	2
			"ABLE",				//	3
			"BRAVO",			//	4
		};

		enum EMap : uint32_t
		{

		};
		static const char* MapNames[1] =
		{
			"NULL"
		};

		enum CameraViewType : uint32_t
		{

		};
	}

}
#pragma pack(pop)