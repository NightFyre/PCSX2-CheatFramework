#pragma once
#include <cstddef>

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
		class Offsets
		{
		public:
			std::string _data;

			//	Pointers
			int o_GS		= 0x2623C0;			//	GameState
			int o_WS		= 0x2623C4;			//  WorldState
			int o_LS		= 0x2623C8;			//  LevelState
			int o_JT		= 0x262E10;			//  SlyPointer
			int o_CC		= 0x126A64;			//	CustomColors
			int o_EOC		= 0x2623E0;			//	EntityOutlineColor

			std::vector<int> OffsetArray = 
			{
				o_GS,		
				o_WS,       
				o_LS,       
				o_JT,       
				o_CC,		
				o_EOC,		
			};

			std::vector<std::string> names = 
			{
				"GameState",
				"WorldState",
				"LevelState",
				"SlyPointer",
				"CustomColorAddr",
				"EntityOutlineColorAddr"
			};

			// Auto Resolve List
		public:
			uintptr_t slyPointer			= g_PS2Mem->GetClassPtr(o_JT);
			uintptr_t gsPointer				= g_PS2Mem->GetClassPtr(o_GS);
			uintptr_t wsPointer				= g_PS2Mem->GetClassPtr(o_WS);
			uintptr_t lsPointer				= g_PS2Mem->GetClassPtr(o_LS);
			uintptr_t CustomColors			= g_PS2Mem->GetAddr(o_CC);
			uintptr_t EntityOutlineColors	= g_PS2Mem->GetAddr(o_EOC);


			//	Functions
		public:
			std::string LogData();
		};

		class JT
		{

			//	NATIVE OFFSETS
		public:
			//int objectBase;			//0x0000
			//char pad_0004[16];		//0x0004
			//__int64 SWPointer;		//0x0014
			//char pad_001C[228];		//0x001C
			char			pad_0000[256];			//0x0000
			Vector3			Position;				//0x0100
			char			pad_010C[68];			//0x010C
			Vector3			Velocity;				//0x0150
			char			pad_015C[508];			//0x015C
			float			Gravity;				//0x0358
			char			pad_035C[732];			//0x035C
			float			FacingAngle;			//0x0638
			char			pad_063C[108];			//0x063C
			float			JumpHeight;				//0x06A8
			char			pad_06AC[7028];			//0x06AC
			JTStates		JTState;				//0x2220
			JTBodyStates	JTBodyState;			//0x2224
			JTCaneStates	JTCaneState;			//0x2228
			char			pad_222C[60];			//0x222C
			int				MomentumHalting;		//0x2268
			char			pad_226C[556];			//0x226C
			int				JumpAddress2;			//0x2498
			char			pad_249C[367884];		//0x249C
			BYTE			CaneHandleOutline[4];	//0x5C1A8
			char			pad_5C1AC[3420];		//0x5C1AC
			BYTE			CaneHookHandle[4];		//0x5CF08
			char			pad_5CF0C[192076];		//0x5CF0C
			BYTE			LeftHandOutlineA[4];	//0x8BD58
			char			pad_8BD5C[230044];		//0x8BD5C
			BYTE			HeadOutline[4];			//0xC3FF8
			char			pad_C3FFC[23868];		//0xC3FFC
			BYTE			EarsOutline[4];			//0xC9D38
			char			pad_C9D3C[23212];		//0xC9D3C
			BYTE			HatOutline[4];			//0xCF7E8
			char			pad_CF7EC[38844];		//0xCF7EC
			BYTE			BackpackOutline[4];		//0xD8FA8

			//	FUNCTIONS
		public:
			bool			IsValid();
			void			SyncOutlines(float value);
			void			RestoreOutlines();
		};
		//Size: 0xD8FAC
	}
}
#pragma pack(pop)