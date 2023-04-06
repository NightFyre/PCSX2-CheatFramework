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
		class Offsets
		{
		public:
			// Match
			int gameEndAddr		= 0x5D708C;
			int forceMatch		= 0x1F66F4;

			// Character
			int PlayerPointer	= 0x48D548;
			int entArray		= 0x4D46A4;

			// Environment
			int fog				= 0x1E5AC0;
			int fps1			= 0x48CF60;
			int fps2			= 0x48CF64;


			std::vector<int> OffsetArray =
			{
				gameEndAddr,
				forceMatch,
				PlayerPointer,
				entArray,
				fog,
				fps1,
				fps2
			};

			std::vector<std::string> names =
			{
				"GameEndedAddress",
				"ForceStartMatch",
				"SEALPointer",
				"EntityArrayPointer",
				"FOG",
				"Framerate",
				"Framerate"
			};

			// Auto Resolve List
		public:
			uintptr_t SEALPointer		= g_PS2Mem->GetClassPtr(PlayerPointer);
			uintptr_t EntityArray		= g_PS2Mem->GetAddr(entArray);
			uintptr_t GameEndAddress	= g_PS2Mem->GetAddr(gameEndAddr);
			uintptr_t FrameRate1		= g_PS2Mem->GetAddr(fps1);
			uintptr_t FrameRate2		= g_PS2Mem->GetAddr(fps2);

			//	Functions
		public:
			std::string LogData();
		};

		class CCamera
		{

			//	OFFSETS
		public:
			char			pad_0000[80];	//0x0000
			Vector3			Position;		//0x0050
			char			pad_005C[132];	//0x005C

			//	FUNCTIONS
		public:
			std::string		LogData();
		};

		class CWeapon
		{

			/*

				- Core
				-- GiveAmmo
				-- GiveWeapon
				-- GetLoadoutData | Primary, Secondary, Equipment & Ammo

				- Systems
				-- Weapon Manager | Slot -> Type -> Weapon : Send

			*/

		public:
			int e_selectedSlot = Primary;
			const char* AvailSlots[5] = { "Primary", "Secondary", "Equipment 1", "Equipment 2", "Equipment 3" };

			unsigned int e_selected_Primary = Primary::AR_552;
			const char* PrimaryWeapons[4] = { "M4A1 SD", "552", "9mm Sub", "abs" };

			unsigned int e_selected_Secondary = Secondary::PISTOL_9MM;
			const char* SecondaryWeapons[5] = { "9MM Pistol", "abs", "abs2", "abs3", "abs4" };

			unsigned int e_selected_Equipment = Equipment::Flashbang;
			const char* EquipmentWeapon[1] = { "Flashbang" };

		public:
			std::string LogData();
		};

		class CPlayerPhysics
		{
		public:
			char						pad_0000[48];	//0x0000
			Vector3						absPosition;	//0x0030
			char						pad_003C[348];	//0x003C

			//	FUNCTIONS
		public:
			std::string LogData();

		};	//Size: 0x0198

		class CPlayer
		{
		public:	// NATIVE OFFSETS (DO NOT DISRUPT)
			char						pad_0000[20];		//0x0000
			int							NamePTR;			//0x0014
			char						pad_0018[4];		//0x0018
			Vector3						Position;			//0x001C
			class CPlayerPhysics*		CPlayerMovement;	//0x0028
			char						pad_0030[148];		//0x0030
			int							TeamID;				//0x00C4
			char						pad_00C8[1348];		//0x00C8
			unsigned int				PrimaryWeapon;		//0x060C
			unsigned int				SecondaryWeapon;	//0x0610
			unsigned int				EqSlot1;			//0x0614
			unsigned int				EqSlot2;			//0x0618
			unsigned int				EqSlot3;			//0x061C
			char						pad_0620[220];		//0x0620
			int							PrimaryMag[10];		//0x06FC
			int							SecondaryMag[10];	//0x0724
			int							EquipmentSlot1;		//0x074C
			char						pad_0750[36];		//0x0750
			int							EquipmentSlot2;		//0x0774
			char						pad_0778[36];		//0x0778
			int							EquipmentSlot3;		//0x079C
			char						pad_07A0[1840];		//0x07A0
			float						Health;				//0x0ED0

			//	FUNCTIONS
		public:
			bool						IsValid();
			bool						IsAlive();
			CPlayerPhysics*				PlayerPhysicsPtr();
			std::string					GetPlayerName();
			std::string					GetTeamName();
			std::string					GetWeaponName(unsigned int Weapon);
			void						GiveAmmo(int amount, int mags = {});
			void						GiveWeapon(unsigned int Slot, unsigned int Weapon);
			void						RemoveWeaponsandAmmo();
			void						Teleport(Vector3 Pos);
			std::string					LogData();

		};	//Size: 0x0ED4

		class MatchData
		{
			/*
			*
				- ForceHost
				- ForceStartMatch
				- ForceEndMatch
				- MatchNeverEnds
				- PlayerArray
				- GetLobbyInfo	| RoomName, MapName, Mode, #Players(Organized by team), Rounds Won/Current Round/Timer

			*/
			// NOTES
			// Host
			// - Force Start
			// - 
			// Match Info
			// - Map
			// - Round
			// - Time
			// - Players
			// - Score

			// SUB CLASSES
		public:

			//	OFFSETS
		public:

			//	FUNCTIONS
		public:
			bool						isMatchEnded();
			void						ForceStartMatch();
			std::vector<CPlayer*>		GetPlayers();
			std::string					LogData();
		};
	}
}
#pragma pack(pop)