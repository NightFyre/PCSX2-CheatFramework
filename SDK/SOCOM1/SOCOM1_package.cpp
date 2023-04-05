#pragma once
#include "../../pch.h"

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
		//----------------------------------------------------------------------------------------------------
		//										OFFSETS
		// 	   Created By NightFyre
		// - Offsets
		//-----------------------------------------------------------------------------------

		///---------------------------------------------------------------------------------------------------
		//	[OFFSETS]
		//	OUTPUT
		// 0x00000000	0x00000000	:	<OffsetName>		// display like native PS2 Memory
		//NOTE: Ends with 2New Line
		std::string Offsets::LogData()
		{
			char data[0x1024];
			const char* data2 = "%llX	0x%08X	:	%s\n";
			std::string result;
			for (int i = 0; i < OffsetArray.size(); i++) {
				sprintf_s(data, data2, g_PS2Mem->GetAddr(OffsetArray[i]), g_PS2Mem->PS2Read<int>(g_PS2Mem->GetAddr(OffsetArray[i])), names[i].c_str());
				result += data;
			}
			return (result += "\n");
		}


		//----------------------------------------------------------------------------------------------------
		//										CCAMERA
		// 	   Created By NightFyre
		// - CCamera
		//-----------------------------------------------------------------------------------

		///---------------------------------------------------------------------------------------------------
		//	[OFFSETS]
		std::string CCamera::LogData()
		{
			return "";
		}


		//----------------------------------------------------------------------------------------------------
		//										CWEAPON
		// 	   Created By NightFyre
		// - CWeapon
		//-----------------------------------------------------------------------------------

		///---------------------------------------------------------------------------------------------------
		//	[CWEAPON]
		std::string CWeapon::LogData()
		{
			return "";
		}


		//----------------------------------------------------------------------------------------------------
		//										CPLAYERPHYSICS
		// 	   Created By NightFyre
		// - CPlayerPhysics
		//-----------------------------------------------------------------------------------

		///---------------------------------------------------------------------------------------------------
		//	[CPLAYERPHYSICS]
		std::string CPlayerPhysics::LogData()
		{
			return "";
		}


		//----------------------------------------------------------------------------------------------------
		//										CPLAYER
		// 	   Created By NightFyre
		// - CPlayer
		//-----------------------------------------------------------------------------------

		///---------------------------------------------------------------------------------------------------
		//	[CPLAYER]
		// Returns Class Pointer Status
		bool CPlayer::IsValid()
		{
			return ((unsigned int)this->pad_0000 == 0x000000 || this->NamePTR == NULL || this->CPlayerMovement == nullptr || this->TeamID == NULL) ? FALSE : TRUE;
		}

		///---------------------------------------------------------------------------------------------------
		//	[CPLAYER]
		// Returns Players Alive State
		bool CPlayer::IsAlive()
		{
			return (this->Health > 0.0f) ? TRUE : FALSE;
		}

		///---------------------------------------------------------------------------------------------------
		//	[CPLAYER]
		// Returns a Pointer to CPlayerPhysics Class
		CPlayerPhysics* CPlayer::PlayerPhysicsPtr()
		{
			return (CPlayerPhysics*)((unsigned int)this->CPlayerMovement + g_PS2Mem->BasePS2MemorySpace);
		}

		///---------------------------------------------------------------------------------------------------
		//	[CPLAYER]
		// Returns Player Name
		std::string CPlayer::GetPlayerName()
		{
			return (char*)(NamePTR + g_PS2Mem->BasePS2MemorySpace);
		}

		///---------------------------------------------------------------------------------------------------
		//	[CPLAYER]
		// Returns Player Team Name
		std::string CPlayer::GetTeamName()
		{
			switch (this->TeamID)
			{
			case (SEALS):		return "SEAL";
			case (TERRORIST):	return "TERRORIST";
			case (SPECTATOR):	return "SPECTATOR";
			case (SP_ABLE):		return	"SEALS - ABLE Team";
			case (SP_BRAVO):	return	"SEALS - BRAVO Team";
			case (SP_ENEMY_A):	return	"Hostile";
			case (SP_ENEMY_B):	return	"Hostile";
			case (SP_ENEMY_C):	return	"Hostile";
			case (SP_ENEMY_D):	return	"Hostile";
			case (SP_ENEMY_E):	return	"Hostile";
			case (SP_ENEMY_F):	return	"Hostile";
			case (SP_ENEMY_G):	return	"Hostile";
			case (SP_ENEMY_H):	return	"Hostile";
			case (SP_ENEMY_I):	return	"Hostile";
			}
			return std::to_string(this->TeamID);
		}

		///---------------------------------------------------------------------------------------------------
		//	[CPLAYER]
		// Returns Weapon Name
		std::string CPlayer::GetWeaponName(unsigned int Weapon)
		{
			switch (Weapon)
			{
			case(Primary::AR_552): return "552";
			case(Primary::SMG_9MM): return "9MM SUB";
			case(Secondary::PISTOL_9MM): return "9MM PISTOL";
			}

			char data[0x56];
			const char* data2 = "%08X";
			sprintf_s(data, data2, Weapon);
			return std::to_string(Weapon);
		}

		///---------------------------------------------------------------------------------------------------
		//	[CPLAYER]
		void CPlayer::GiveAmmo(int amount, int mags)
		{
			if (mags <= NULL || mags > 10)
				mags = 3;

			for (int i = NULL; i < mags; i++)
			{
				this->PrimaryMag[i] = amount;
				this->SecondaryMag[i] = amount;
			}
			this->EquipmentSlot1 = NULL;
			this->EquipmentSlot2 = NULL;
			this->EquipmentSlot3 = NULL;
		}

		///---------------------------------------------------------------------------------------------------
		//	[CPLAYER]
		// Replaces current weapon in slot with input Weapon
		void CPlayer::GiveWeapon(unsigned int Slot, unsigned int Weapon)
		{
			// Intended usage
			switch (Slot)
			{
			case (WeaponSlot::Primary):		this->PrimaryWeapon = Weapon;	break;
			case (WeaponSlot::Secondary):	this->SecondaryWeapon = Weapon;	break;
			case (WeaponSlot::EqSlot1):		this->EqSlot1 = Weapon;	break;
			case (WeaponSlot::EqSlot2):		this->EqSlot2 = Weapon;	break;
			case (WeaponSlot::EqSlot3):		this->EqSlot3 = Weapon;	break;
			}
		}

		///---------------------------------------------------------------------------------------------------
		//	[CPLAYER]
		void CPlayer::RemoveWeaponsandAmmo()
		{
			this->PrimaryWeapon = NULL;
			this->SecondaryWeapon = NULL;
			this->EqSlot1 = NULL;
			this->EqSlot2 = NULL;
			this->EqSlot3 = NULL;

			for (int i = NULL; i < 10; i++)
			{
				this->PrimaryMag[i] = NULL;
				this->SecondaryMag[i] = NULL;
			}
			this->EquipmentSlot1 = NULL;
			this->EquipmentSlot2 = NULL;
			this->EquipmentSlot3 = NULL;
		}

		///---------------------------------------------------------------------------------------------------
		//	[CPLAYER]
		// Teleports this entity to desired position
		void CPlayer::Teleport(Vector3 Pos)
		{
			unsigned int test = (unsigned int)this->CPlayerMovement;
			auto offset = (test + g_PS2Mem->BasePS2MemorySpace);
			auto Movement = (CPlayerPhysics*)offset;
			Movement->absPosition = Pos;
		}

		///---------------------------------------------------------------------------------------------------
		//	[CPLAYER]
		// generates a log containing important player data
		// NOTE: Ends with 2New Line
		std::string CPlayer::LogData()
		{
			Offsets offsets;
			auto SEAL = (CPlayer*)offsets.SEALPointer;
			float distance;
			if (SEAL != this)
				distance = g_PS2Mem->Tools->GetDistanceTo3DObject(SEAL->Position, this->Position);	//	@todo: Tools might be a nullptr here
			else
				distance = 0.0f;

			char data[0x256];
			const char* data2 = "PlayerObjectBase: %llX\nPlayerName: %s\nTeamName: %s\nPrimary Weapon: %s	|	PrimaryAmmo: %i\nSecondary Weapon: %s	|	SecondaryAmmo: %i\nHealth: %0.f\nPosition: { %f, %f, %f }\nDistance: { %f }\n\n";
			sprintf_s(data, data2, this, this->GetPlayerName().c_str(), this->GetTeamName().c_str(),
				GetWeaponName(this->PrimaryWeapon).c_str(), this->PrimaryMag[0],
				GetWeaponName(this->SecondaryWeapon).c_str(), this->SecondaryMag[0],
				(this->Health * 100.f),
				this->Position.x, this->Position.y, this->Position.z, distance);
			return data;
		}


		//----------------------------------------------------------------------------------------------------
		//										MATCHDATA
		// 	   Created By NightFyre
		// - MatchData
		//-----------------------------------------------------------------------------------

		///---------------------------------------------------------------------------------------------------
		//	[MATCHDATA]
		bool MatchData::isMatchEnded()
		{
			Offsets offset;
			return (g_PS2Mem->PS2Read<int>(offset.GameEndAddress) == NULL) ? TRUE : FALSE;
		}

		///---------------------------------------------------------------------------------------------------
		//	[MATCHDATA]
		void MatchData::ForceStartMatch()
		{
			//
		}

		///---------------------------------------------------------------------------------------------------
		//	[MATCHDATA]
		std::vector<CPlayer*> MatchData::GetPlayers()
		{
			int count = NULL;
			Offsets offsets;
			std::vector<CPlayer*> ents{};

			//   Grab the first entityObjectPointer in the array
			auto entArray = offsets.EntityArray;
			if (entArray == NULL) return ents;

			//   Store base address for later because the entity array loops back to the beginning
			auto baseEntity = entArray;

			size_t maxSize = 0x64;
			for (int i = 0; i <= maxSize; i++)
			{
				//	Cast entity object to CPlayer Class
				CPlayer* ent = (CPlayer*)((*(int32_t*)(entArray + 0x8)) + g_PS2Mem->BasePS2MemorySpace);
				if (ent == NULL)
					break;

				if (!ent->IsValid())
				{
					// Get next entity in the array
					entArray = *(int32_t*)entArray + g_PS2Mem->BasePS2MemorySpace;
					continue;
				}

				//	Push CPlayer Object into the vector array
				ents.push_back(ent);

				// Get next entity in the array
				entArray = *(int32_t*)entArray + g_PS2Mem->BasePS2MemorySpace;

				if (entArray == baseEntity)
					break;
			}

			//	return array of players
			return ents;
		}

		///---------------------------------------------------------------------------------------------------
		//	[MATCHDATA]
		std::string MatchData::LogData()
		{
			// Data
			// - Player Count
			// - Map Name
			// - Current Round
			// - Match Timer
			return "";
		}
	}
}
#pragma pack(pop)




///	EXAMPLE ImGui Project

	/*
	//	Debug Weapon Manager
	//	Will not work in this scope!
	//	Does not work in current state, enum values need to be passed but instead is passing enum positions
	CWeapon WeaponData;
	void WeaponManager(CPlayer* player)
	{
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::TextCentered("[WEAPON MANAGER]", TRUE, ImColor(0, 255, 255, 200));
		ImGui::Separator();
		ImGui::SetNextItemWidth(100);
		ImGui::Combo("##jfhgjdflkhklsdh", (int*)&WeaponData.e_selectedSlot, WeaponData.AvailSlots, IM_ARRAYSIZE(WeaponData.AvailSlots));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		switch (WeaponData.e_selectedSlot)
		{
			case(0): ImGui::Combo("##jfhgjdflkhklsdh1", (int*)&WeaponData.e_selected_Primary, WeaponData.PrimaryWeapons, IM_ARRAYSIZE(WeaponData.PrimaryWeapons)); break;
			case(1): ImGui::Combo("##kdgdsazhljfna", (int*)&WeaponData.e_selected_Secondary, WeaponData.SecondaryWeapons, IM_ARRAYSIZE(WeaponData.SecondaryWeapons)); break;
			case(2): ImGui::Combo("##zjdfhbadeh", (int*)&WeaponData.e_selected_Equipment, WeaponData.EquipmentWeapon, IM_ARRAYSIZE(WeaponData.EquipmentWeapon)); break;
			case(3): ImGui::Combo("##hsdfgjsytkiet", (int*)&WeaponData.e_selected_Equipment, WeaponData.EquipmentWeapon, IM_ARRAYSIZE(WeaponData.EquipmentWeapon)); break;
			case(4): ImGui::Combo("##gjnkdtoythmyuk", (int*)&WeaponData.e_selected_Equipment, WeaponData.EquipmentWeapon, IM_ARRAYSIZE(WeaponData.EquipmentWeapon)); break;
		}
		if (ImGui::Button("GIVE WEAPON", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			switch (WeaponData.e_selectedSlot)
			{
				case 0:        player->GiveWeapon(WeaponData.e_selectedSlot, WeaponData.e_selected_Primary); break;
				case 1:        player->GiveWeapon(WeaponData.e_selectedSlot, WeaponData.e_selected_Secondary); break;
				case 2:        player->GiveWeapon(WeaponData.e_selectedSlot, WeaponData.e_selected_Equipment); break;
				case 3:        player->GiveWeapon(WeaponData.e_selectedSlot, WeaponData.e_selected_Equipment); break;
				case 4:        player->GiveWeapon(WeaponData.e_selectedSlot, WeaponData.e_selected_Equipment); break;
			}
		}
	}

	void test_debug_menu()
	{
		SOCOM1::Offsets offsets;
		SOCOM1::CPlayer* PLAYER = (CPlayer*)offsets.SEALPointer;
		SOCOM1::MatchData MATCH;

		if (ImGui::Button("Test Read Memory", ImVec2(ImGui::GetWindowContentRegionWidth(), 20)))
		{
			auto address = offsets.FrameRate1;

			auto value = g_Engine->PS2Read<int>(offsets.FrameRate1);

			g_Console->printdbg("Test Read Memory:\nFrameRate1: [%08X]\nVALUE: [%i]\n\n", Console::Colors::blue, address, value);
		}

		if (ImGui::Button("Test Write Memory", ImVec2(ImGui::GetWindowContentRegionWidth(), 20)))
		{
			g_Engine->PS2Write<int>(offsets.FrameRate1, 30);

			g_Engine->PS2Write<int>(offsets.FrameRate2, 30);

			g_Console->printdbg("Test Write Memory:\nFrameRate1: [%i]\nFrameRate2: [%i]\n\n", Console::Colors::blue, g_Engine->PS2Read<int>(offsets.FrameRate1), g_Engine->PS2Read<int>(offsets.FrameRate2));

		}

		if (ImGui::Button("Test ResolvePointer", ImVec2(ImGui::GetWindowContentRegionWidth(), 20)))
		{
			if (PLAYER->IsValid()) {

				CPlayer::CPlayerPhysics* move = PLAYER->PlayerPhysicsPtr();

				g_Console->printdbg("%s", Console::Colors::DEFAULT, PLAYER->LogData().c_str());
			}
			else
				g_Console->printdbg("[!] INVALID PLAYER OBJECT\n", Console::Colors::red);
		}

		if (ImGui::Button("Display Offsets Data", ImVec2(ImGui::GetWindowContentRegionWidth(), 20)))
			g_Console->printdbg("Dumping Offset Data:\n%s", Console::Colors::DEFAULT, offsets.LogData().c_str());

		if (ImGui::Button("Check isValid", ImVec2(ImGui::GetWindowContentRegionWidth(), 20)))
		{
			bool result = PLAYER->IsValid();

			switch (result) {

			case (TRUE):	g_Console->printdbg("isValid: [%i]\n", Console::Colors::yellow, result); break;

			case (FALSE):	g_Console->printdbg("isValid: [%i]\n", Console::Colors::red, result); break;
			}
		}

		if (ImGui::Button("Dump Player Data", ImVec2(ImGui::GetWindowContentRegionWidth(), 20)))
		{
			if (PLAYER->IsValid())
				g_Console->printdbg("%s", Console::Colors::DEFAULT, PLAYER->LogData().c_str());
			else
				g_Console->printdbg("[!] INVALID PLAYER OBJECT\n", Console::Colors::red);
		}

		if (ImGui::Button("Give 552", ImVec2(ImGui::GetWindowContentRegionWidth(), 20)))
		{
			if (PLAYER->IsValid()) {
				//player->PrimaryWeapon = CPlayer::CWeapon::AR_552;
				PLAYER->PrimaryWeapon = CWeapon::AR_552;
				for (int i = 0; i < 10; i++)
					PLAYER->PrimaryMag[i] = 1337;
			}
			else
				g_Console->printdbg("[!] INVALID PLAYER OBJECT\n", Console::Colors::red);
		}

		if (ImGui::Button("Dump Entity Array", ImVec2(ImGui::GetWindowContentRegionWidth(), 20)))
		{
			std::vector<SOCOM1::CPlayer*> ent = MATCH.GetPlayers();

			if (!ent.empty()) {

				for (int i = 0; i < ent.size(); i++)
					g_Console->printdbg("%s", Console::Colors::DEFAULT, ent[i]->LogData().c_str());

			}

		}

		if (ImGui::Button("Telpeort All Entities", ImVec2(ImGui::GetWindowContentRegionWidth(), 20)))
		{
			if (PLAYER->IsValid())
			{
				std::vector<SOCOM1::CPlayer*> ent = MATCH.GetPlayers();

				if (!ent.empty()) {

					for (int i = 0; i < ent.size(); i++) {

						float distance = PLAYER->GetDistanceTo3DObject(PLAYER->Position, ent[i]->Position);
						g_Console->printdbg("Teleporting Entity:\n- Name: %s\n - Position: { %0.2f, %0.2f, %0.2f }\n - Distance: %0.0fm\n\n", Console::Colors::DEFAULT, ent[i]->GetPlayerName().c_str(), ent[i]->Position.x, ent[i]->Position.y, ent[i]->Position.z, distance);
						ent[i]->Teleport(PLAYER->Position);
					}
				}
			}
		}

		if (ImGui::Button("Dump Match Data", ImVec2(ImGui::GetWindowContentRegionWidth(), 20)))
			g_Console->printdbg("IsMatchEnded: [%i]\n\n", Console::Colors::yellow, MATCH.isMatchEnded());

		if (PLAYER->IsValid())
			WeaponManager(PLAYER);
	}
	*/