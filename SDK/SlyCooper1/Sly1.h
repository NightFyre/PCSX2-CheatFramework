#pragma once
#include "engine.h"

/// RESOURCES & CREDITS: Sly Cooper Modding Server
/// <summary>
/// - https://github.com/zzamizz/weed-sheet
/// - https://docs.google.com/spreadsheets/d/1ISxw587iICRDdaLJfLaTvJUaYkjGBReH4NY-yKN-Ip0/edit#gid=0
/// - https://discord.com/invite/2GSXcEzPJA
/// </summary>

namespace Sly1 {

	class Offsets : public PS2
	{
	public:

		//	
	public:
		std::string _data;
		
		//	Pointers
		int o_GS = 0x2623C0;
		int o_WS = 0x2623C4;
		int o_LS = 0x2623C8;
		int o_JT = 0x262E10;
		int o_CC = 0x126A64;
		int o_EOC = 0x2623E0;

		std::vector<int> OffsetArray = {
			o_GS,		//	GameState
			o_WS,       //  WorldState
			o_LS,       //  LevelState
			o_JT,       //  SlyPointer
			o_CC,		//	CustomColors
			o_EOC,		//	EntityOutlineColor
		};

		std::vector<std::string> names = {
			"GameState",
			"WorldState",
			"LevelState",
			"SlyPointer",
			"CustomColorAddr",
			"EntityOutlineColorAddr"
		};

		// Auto Resolve List
	public:
		uintptr_t slyPointer = GetClassPtr(o_JT);
		uintptr_t gsPointer = GetClassPtr(o_GS);
		uintptr_t wsPointer = GetClassPtr(o_WS);
		uintptr_t lsPointer = GetClassPtr(o_LS);
		uintptr_t CustomColors = GetAddr(o_CC);
		uintptr_t EntityOutlineColors = GetAddr(o_EOC);


		//	Functions
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		//	OUTPUT
		// 0x00000000	0x00000000	:	<OffsetName>		// display like native PS2 Memory
		//NOTE: Ends with 2New Line
		std::string LogData()
		{
			char data[0x1024];
			const char* data2 = "%llX	0x%08X	:	%s\n";
			std::string _data;
			for (int i = 0; i < OffsetArray.size(); i++) {
				sprintf_s(data, data2, GetAddr(OffsetArray[i]), PS2Read<int>(GetAddr(OffsetArray[i])), names[i].c_str());
				_data += data;
			}
			return (_data += "\n");
		}
	};

	class JT : public PS2
	{
		//	ENUMS
	public:
		enum JTStates : int {
			JT_Stand = 0,
			JT_Run = 1,
			JT_Jump = 2,
			JT_Hang = 3,
			JT_Zap = 4,
			JT_SideStep = 5,
			JT_Hide = 6,
			JT_Pipe = 7,
			JT_Edge = 8,
			JT_Celebrate = 9,
			JT_Rush = 10,
			JT_Puppet = 11,
			JT_Peek = 12,
			JT_Ski = 13,
			JT_Ball = 14,
		};

		enum JTBodyStates : int {
			Jump_Init = 0,
			Jump_Rise = 1,
			Jump_Fall = 2,
			Jump_Boost = 3,
			Jump_In = 4,
			Jump_Out = 5,
			Jump_Thrown = 6,
			Jump_Smash = 7,
			Jump_Dive = 8,
			Jump_Target = 9,
			Jump_Spire = 10,
			Jump_Rail = 11,
			Jump_Hshape = 12,
			Jump_Hpnt = 13,
			Hide_Stand = 14,
			Hide_Sidestep = 15,
			Hide_PeekLeft = 16,
			Hide_PeekRight = 17,
			Hide_Run = 18,
			Hide_Spin = 19,
			Pipe_Init = 20,
			Pipe_Stay = 21,
			Pipe_Up = 22,
			Pipe_Down = 23,
			Pipe_Spin = 24,
			Celebrate_Key = 25,
			Celebrate_Timed = 26,
			Celebrate_UseKey = 27,
			Rush_Attack = 28,
			Rush_Bounce = 29,
			Peek_Enter = 30,
			Peek_Peek = 31,
			Peek_Exit = 32,
			Zap_Blunt = 33,
			Zap_Electric = 34,
			Zap_Fire = 35,
			Zap_Water = 36,
			Zap_Crush = 37,
			Zap_Pit = 38,
			Zap_Thrown = 39,
			Zap_Dead = 40,
			Zap_DeadFront = 41,
			Zap_DeadInWater = 42,
		};

		enum JTCaneStates : int {
			Reach = 0,
			Hang = 1,
			Pipe = 2,
			SweepInit = 3,
			SweepFore = 4,
			SweepBack = 5,
			StandingSweepFore = 6,
			StandingSweepBack = 7,
			Max = 8,
		};

		//	NATIVE OFFSETS
	public:
		//int objectBase;			//0x0000
		//char pad_0004[16];		//0x0004
		//__int64 SWPointer;		//0x0014
		//char pad_001C[228];		//0x001C
		char pad_0000[256];			//0x0000
		Vector3 Position;			//0x0100
		char pad_010C[68];			//0x010C
		Vector3 Velocity;			//0x0150
		char pad_015C[508];			//0x015C
		float Gravity;				//0x0358
		char pad_035C[732];			//0x035C
		float FacingAngle;			//0x0638
		char pad_063C[108];			//0x063C
		float JumpHeight;			//0x06A8
		char pad_06AC[7028];		//0x06AC
		int JTState;				//0x2220
		int JTBodyState;			//0x2224
		int JTCaneState;			//0x2228
		char pad_222C[60];			//0x222C
		int MomentumHalting;		//0x2268
		char pad_226C[556];			//0x226C
		int JumpAddress2;			//0x2498
		char pad_249C[367884];		//0x249C
		byte CaneHandleOutline[4];	//0x5C1A8
		char pad_5C1AC[3420];		//0x5C1AC
		byte CaneHookHandle[4];		//0x5CF08
		char pad_5CF0C[192076];		//0x5CF0C
		byte LeftHandOutlineA[4];	//0x8BD58
		char pad_8BD5C[230044];		//0x8BD5C
		byte HeadOutline[4];		//0xC3FF8
		char pad_C3FFC[23868];		//0xC3FFC
		byte EarsOutline[4];		//0xC9D38
		char pad_C9D3C[23212];		//0xC9D3C
		byte HatOutline[4];			//0xCF7E8
		char pad_CF7EC[38844];		//0xCF7EC
		byte BackpackOutline[4];	//0xD8FA8
		//Size: 0xD8FAC

		//	FUNCTIONS
	public:

		/// <summary>
		/// Returns Class Pointer Status
		/// </summary>
		/// <returns>BOOL</returns>
		bool IsValid()
		{
			return ((unsigned int)this->pad_0000 == 0x000000) ? FALSE : TRUE;
		}


		void SyncOutlines(float value)
		{
			Offsets offsets;
			auto address = offsets.CustomColors;
			if (g_Engine->PS2Read<int>(address) != NULL)
				g_Engine->PS2Write<int>(address, NULL);
				
			//	Change all Slys outline colors
			//this->CaneHandleOutline		= value;
			//this->CaneHookHandle		= value;	
			//this->LeftHandOutlineA		= value;
			//this->HeadOutline			= value;
			//this->HatOutline			= value;
			//this->BackpackOutline		= value;
		}

		void RestoreOutlines()
		{
			Offsets offsets;
			auto address = offsets.CustomColors;
			if (g_Engine->PS2Read<int>(address) == NULL)
				g_Engine->PS2Write<int>(address, 0xAC910268);
		}
	};

	//	DEBUG MENU
	/*
		static const ImVec4 defaultColor = ImVec4(16.0f / 255.0f, 16.0f / 255.0f, 16.0f / 255.0f, 128.0f / 255.0f);					//	[ 0x10, 0x10, 0x10, 0x80 ]

		static bool bGeneralOutline = FALSE;
		static bool bRGB_SLY = FALSE;
		static ImVec4 main_color_picker = ImVec4(16.0f / 255.0f, 16.0f / 255.0f, 16.0f / 255.0f, 128.0f / 255.0f);				//	[ 0x10, 0x10, 0x10, 0x80 ]

		static bool bCaneHandle_outline = FALSE;
		static ImVec4 CaneHandle_picker = ImVec4(16.0f / 255.0f, 16.0f / 255.0f, 16.0f / 255.0f, 128.0f / 255.0f);				//	[ 0x10, 0x10, 0x10, 0x80 ]

		static bool bCaneHookHandle_outline = FALSE;
		static ImVec4 CaneHookHandle_picker = ImVec4(16.0f / 255.0f, 16.0f / 255.0f, 16.0f / 255.0f, 128.0f / 255.0f);			//	[ 0x10, 0x10, 0x10, 0x80 ]

		static bool bLeftHandOutline_outline = FALSE;
		static ImVec4 LeftHandOutline_picker = ImVec4(0.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f);			//	[ 0x00, 0xFF, 0xFF, 0x00 ]

		static bool bHeadOutline_outline = FALSE;
		static ImVec4 HeadOutline_picker = ImVec4(46.0f / 255.0f, 144.0f / 255.0f, 96.0f / 255.0f, 62.0f / 255.0f);				//	[ 0x2E, 0x90, 0x60, 0x3E ]

		static bool bEarsOutline_outline = FALSE;
		static ImVec4 EarsOutline_picker = ImVec4(16.0f / 255.0f, 16.0f / 255.0f, 16.0f / 255.0f, 128.0f / 255.0f);				//	[ 0xDD, 0x24, 0x06, 0x3F ]

		static bool bHatOutline_outline = FALSE;
		static ImVec4 HatOutline_picker = ImVec4(16.0f / 255.0f, 16.0f / 255.0f, 16.0f / 255.0f, 128.0f / 255.0f);				//	[ 0x01, 0x00, 0x00, 0x00 ]

		static bool bBackpackOutline_outline = FALSE;
		static ImVec4 BackpackOutline_picker = ImVec4(16.0f / 255.0f, 16.0f / 255.0f, 16.0f / 255.0f, 128.0f / 255.0f);			//	[ 0x00, 0x00, 0x7A, 0x44 ]


		void ChangeBaseOutlineColor(uintptr_t address, ImVec4 Color)
		{
			float floats[4] = { (Color.x * 255), (Color.y * 255), (Color.z * 255), (Color.w * 255) };

			Offsets offsets;
			auto permCheckAddr = offsets.CustomColors;
			if (g_Engine->PS2Read<int>(permCheckAddr) == NULL)
				g_Engine->PS2Write<int>(permCheckAddr, 0xAC910268);

			for (int i = 0; i < 4; i++) {
				g_Engine->PS2Write<int8_t>(address, floats[i]);
				address++;
			}
		}

		void ChangeOutlineColorEx(ImVec4 Color)
		{
			// Convert ImVec4
			float test[4] = { (Color.x * 255), (Color.y * 255), (Color.z * 255), (Color.w * 255) };

			Offsets offsets;
			auto address = offsets.CustomColors;
			g_Engine->PS2Write<int>(address, NULL);

			auto colorAddress = offsets.EntityOutlineColors;
			for (int i = 0; i < 4; i++) {
				g_Engine->PS2Write<int8_t>(colorAddress, test[i]);
				colorAddress++;
			}

			//g_Engine->PS2Write<int>(address, 0xAC910268);
		}

		void RGBSLY()
		{
			Offsets offsets;
			auto permCheckAddr = offsets.CustomColors;
			if (g_Engine->PS2Read<int>(permCheckAddr) == NULL)
				g_Engine->PS2Write<int>(permCheckAddr, 0xAC910268);

			// Convert ImVec4
			float test[4] = { (g_Menu->dbg_RAINBOW.Value.x * 255), (g_Menu->dbg_RAINBOW.Value.y * 255), (g_Menu->dbg_RAINBOW.Value.z * 255), (g_Menu->dbg_RAINBOW.Value.w * 255) };
			auto colorAddress = offsets.EntityOutlineColors;
			for (int i = 0; i < 4; i++) {
				g_Engine->PS2Write<int8_t>(colorAddress, test[i]);
				colorAddress++;
			}
		}
		*/
		
		
		/*

		///
		Sly1::Offsets offsets;
		Sly1::JT* PLAYER = (Sly1::JT*)offsets.slyPointer;

		ImGui::TextCentered("[ SLY COLOR OUTLINES ]", TRUE, ImColor(0, 255, 255, 200));
		ImGui::Spacing();
		ImGui::Separator();

		{

			ImGui::Toggle(" General ", &bGeneralOutline);
			ImGui::SameLine();
			window->DC.CursorPos.x = (window->DC.CursorPos.x + 20);
			ToolLocation.x = window->DC.CursorPos.x;
			ImGui::ColorEdit4("##gdskhjaoergea", (float*)&main_color_picker, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoInputs);
			if (bGeneralOutline)															//
				ChangeBaseOutlineColor(offsets.EntityOutlineColors, main_color_picker);			//
			ImGui::SameLine();																//
			if (ImGui::Button("RESET##0"))	//
			{
				bGeneralOutline = FALSE;
				main_color_picker = defaultColor;
				ChangeBaseOutlineColor(offsets.EntityOutlineColors, defaultColor);				//
			}

			//---------------------------------------------------------------------------------------------------

			ImGui::Toggle(" Cane ", &bCaneHandle_outline);
			ImGui::SameLine();
			window->DC.CursorPos.x = ToolLocation.x;
			ImGui::ColorEdit4("##ajartyarasrgfhsr", (float*)&CaneHandle_picker, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoInputs);
			if (bCaneHandle_outline) {
				float cane_color[4] = { (CaneHandle_picker.x * 255), (CaneHandle_picker.y * 255), (CaneHandle_picker.z * 255), (CaneHandle_picker.w * 255) };
				g_Engine->PS2Write<int>(offsets.CustomColors, NULL);
				for (int i = 0; i < 4; i++) {
					PLAYER->CaneHandleOutline[i] = cane_color[i];
					PLAYER->CaneHookHandle[i] = cane_color[i];
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("RESET##1"))
			{
				bCaneHandle_outline = FALSE;
				CaneHandle_picker = defaultColor;
				g_Engine->PS2Write<int>(offsets.CustomColors, 0xAC910268);

			}

			//---------------------------------------------------------------------------------------------------

			
			ImGui::Toggle("CaneHook");
			window->DC.CursorPos.x = ToolLocation.x;
			ImGui::SameLine();
			ImGui::ColorEdit4("##adfharjhrthbsh", (float*)&CaneHandle_picker, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			if (ImGui::Button("RESET##2", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{

			}

			//---------------------------------------------------------------------------------------------------

			ImGui::Toggle("LeftHand");
			window->DC.CursorPos.x = ToolLocation.x;
			ImGui::SameLine();
			ImGui::ColorEdit4("##advbarthhbthbtr", (float*)&CaneHandle_picker, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			if (ImGui::Button("RESET##3", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{

			}

			//---------------------------------------------------------------------------------------------------

			ImGui::Toggle("HeadOutline");
			window->DC.CursorPos.x = ToolLocation.x;
			ImGui::SameLine();
			ImGui::ColorEdit4("##zdfbnhzadfdgaehtsarn", (float*)&CaneHandle_picker, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			if (ImGui::Button("RESET##4", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{

			}

			//---------------------------------------------------------------------------------------------------

			ImGui::Toggle("Ears");
			window->DC.CursorPos.x = ToolLocation.x;
			ImGui::SameLine();
			ImGui::ColorEdit4("##dzsfnaarthnsmst", (float*)&CaneHandle_picker, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			if (ImGui::Button("RESET##5", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{

			}

			//---------------------------------------------------------------------------------------------------

			ImGui::Toggle("Hat");
			window->DC.CursorPos.x = ToolLocation.x;
			ImGui::SameLine();
			ImGui::ColorEdit4("##dvfbanrthsrtym", (float*)&CaneHandle_picker, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			if (ImGui::Button("RESET##6", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{

			}

			//---------------------------------------------------------------------------------------------------

			ImGui::Toggle("Backpack");
			window->DC.CursorPos.x = ToolLocation.x;
			ImGui::SameLine();
			ImGui::ColorEdit4("##zdngbtrazbgdzshrt", (float*)&CaneHandle_picker, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			if (ImGui::Button("RESET##7", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{

			}

			//---------------------------------------------------------------------------------------------------

		}

		if (ImGui::Toggle(" RGB SLY ", &bRGB_SLY))
		{
			// on deactivate restore sky outline to normal
			if (!bRGB_SLY)
				ChangeBaseOutlineColor(offsets.EntityOutlineColors, defaultColor);
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Test Read Memory", ImVec2(ImGui::GetWindowContentRegionWidth(), 20)))
		{
			auto address = offsets.CustomColors;

			auto value = g_Engine->PS2Read<int>(address);

			g_Console->printdbg("Test Read Memory Address: %llX\nVALUE: [%08X]\n\n", Console::Colors::blue, address, value);
		}

		if (ImGui::Button("Test Write Memory", ImVec2(ImGui::GetWindowContentRegionWidth(), 20)))
		{

			auto address = offsets.CustomColors;

			unsigned int value = g_Engine->PS2Read<int>(address);

			if (value != NULL)
				g_Engine->PS2Write<int>(address, NULL);
			else
				g_Engine->PS2Write<int>(address, 0xAC910268);

			g_Console->printdbg("Test Write Memory: %llX\nVALUE: [%08X]\n\n", Console::Colors::blue, address, g_Engine->PS2Read<int>(address));

		}

		if (ImGui::Button("Display Offsets Data", ImVec2(ImGui::GetWindowContentRegionWidth(), 20)))
			g_Console->printdbg("[+] LOG::OffsetData\n%s", Console::Colors::DEFAULT, offsets.LogData().c_str());
	
	
	*/
}