#pragma once
#include "../../pch.h"

/**
 * Name: PlayStation2 - PCSX2 :: Sly Cooper and the Thievous Racoonus
 * Version: 0.0.1
 * Author: NightFyre
	
RESOURCES & CREDITS: Sly Cooper Modding Server
	https://github.com/zzamizz/weed-sheet
	https://docs.google.com/spreadsheets/d/1ISxw587iICRDdaLJfLaTvJUaYkjGBReH4NY-yKN-Ip0/edit#gid=0
	https://discord.com/invite/2GSXcEzPJA
*/

#pragma pack(push, 0x01)
namespace PlayStation2
{
	namespace SlyCooper
	{
		//----------------------------------------------------------------------------------------------------
		//										OFFSETS
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
			std::string _data;
			for (int i = 0; i < OffsetArray.size(); i++) {
				sprintf_s(data, data2, g_PS2Mem->GetAddr(OffsetArray[i]), g_PS2Mem->PS2Read<int>(g_PS2Mem->GetAddr(OffsetArray[i])), names[i].c_str());
				_data += data;
			}
			return (_data += "\n");
		}

		//----------------------------------------------------------------------------------------------------
		//										JT
		// - Jt
		//-----------------------------------------------------------------------------------

		///---------------------------------------------------------------------------------------------------
		//	[JT]
		/// Returns Class Pointer Status
		bool JT::IsValid()
		{
			return ((unsigned int)this->pad_0000 == 0x000000) ? FALSE : TRUE;
		}

		///---------------------------------------------------------------------------------------------------
		//	[JT]
		void JT::SyncOutlines(float value)
		{
			Offsets offsets;
			auto address = offsets.CustomColors;
			if (g_PS2Mem->PS2Read<int>(address) != NULL)
				g_PS2Mem->PS2Write<int>(address, NULL);

			//	Change all Slys outline colors
			//this->CaneHandleOutline		= value;
			//this->CaneHookHandle			= value;	
			//this->LeftHandOutlineA		= value;
			//this->HeadOutline				= value;
			//this->HatOutline				= value;
			//this->BackpackOutline			= value;
		}

		///---------------------------------------------------------------------------------------------------
		//	[JT]
		void JT::RestoreOutlines()
		{
			Offsets offsets;
			auto address = offsets.CustomColors;
			if (g_PS2Mem->PS2Read<int>(address) == NULL)
				g_PS2Mem->PS2Write<int>(address, 0xAC910268);
		}
	}
}

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