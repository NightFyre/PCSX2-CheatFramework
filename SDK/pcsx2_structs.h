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


	struct GPRRegs
	{
		__int32 r0;	//0x0000
		__int32 at;	//0x0004
		__int32 v0;	//0x0008
		__int32 v1;	//0x000C
		__int32 a0;	//0x0010
		__int32 a1;	//0x0014
		__int32 a2;	//0x0018
		__int32 a3;	//0x001C
		__int32 t0;	//0x0020
		__int32 t1;	//0x0024
		__int32 t2;	//0x0028
		__int32 t3;	//0x002C
		__int32 t4;	//0x0030
		__int32 t5;	//0x0034
		__int32 t6;	//0x0038
		__int32 t7;	//0x003C
		__int32 s0;	//0x0040
		__int32 s1;	//0x0044
		__int32 s2;	//0x0048
		__int32 s3;	//0x004C
		__int32 s4;	//0x0050
		__int32 s5;	//0x0054
		__int32 s6;	//0x0058
		__int32 s7;	//0x005C
		__int32 t8;	//0x0060
		__int32 t9;	//0x0064
		__int32 k0;	//0x0068
		__int32 k1;	//0x006C
		__int32 gp;	//0x0070
		__int32 sp;	//0x0074
		__int32 s8;	//0x0078
		__int32 ra;	//0x007C
		__int32 hi;	//0x0080
		__int32 lo;	//0x0084
	};	//Size: 0x0088

	struct CP0Regs
	{
		__int32 Index;			//0x0000
		__int32 Random;			//0x0004
		__int32 EntryLo0;		//0x0008
		__int32 EntryLo1;		//0x000C
		__int32 Context;		//0x0010
		__int32 PageMask;		//0x0014
		__int32 Wired;			//0x0018
		__int32 Reserved0;		//0x001C
		__int32 BadVAddr;		//0x0020
		__int32 Count;			//0x0024
		__int32 EntryHi;		//0x0028
		__int32 Compare;		//0x002C
		__int32 Status;			//0x0030
		__int32 Cause;			//0x0034
		__int32 EPC;			//0x0038
		__int32 PRid;			//0x003C
		__int32 Config;			//0x0040
		__int32 LLAddr;			//0x0044
		__int32 WatchLO;		//0x0048
		__int32 WatchHI;		//0x004C
		__int32 XContext;		//0x0050
		__int32 Reserved1;		//0x0054
		__int32 Reserved2;		//0x0058
		__int32 Reserved3;		//0x005C
		__int32 Reserved4;		//0x0060
		__int32 Reserved5;		//0x0064
		__int32 ECC;			//0x0068
		__int32 CacheErr;		//0x006C
		__int32 TagLo;			//0x0070
		__int32 TagHi;			//0x0074
		__int32 ErrorEPC;		//0x0078
		__int32 Reserved6;		//0x007C
	};	//Size: 0x0080

	struct CP2Data
	{
		__int16 v0[4];			//0x0000
		__int16 v1[4];			//0x0008
		__int16 v2[4];			//0x0010
		__int8 rgb[4];			//0x0018
		__int32 otz;			//0x001C
		__int32 ir0;			//0x0020
		__int32 ir1;			//0x0024
		__int32 ir2;			//0x0028
		__int32 ir3;			//0x002C
		__int16 sxy0[2];		//0x0030
		__int16 sxy1[2];		//0x0034
		__int16 sxy2[2];		//0x0038
		__int16 sxyp[2];		//0x003C
		__int16 sz0[2];			//0x0040
		__int16 sz1[2];			//0x0044
		__int16 sz2[2];			//0x0048
		__int16 sz3[2];			//0x004C
		__int8 rgb0[4];			//0x0050
		__int8 rgb1[4];			//0x0054
		__int8 rgb2[4];			//0x0058
		__int32 reserved;		//0x005C
		__int32 mac0;			//0x0060
		__int32 mac1;			//0x0064
		__int32 mac2;			//0x0068
		__int32 mac3;			//0x006C
		__int32 irgb;			//0x0070
		__int32 orgb;			//0x0074
		__int32 lzcs;			//0x0078
		__int32 lzcr;			//0x007C
	};	//Size: 0x0080

	struct CP2Ctrl
	{
		unsigned __int16 rMatrix[10];	//0x0000
		__int32 trX;	//0x0014
		__int32 trY;	//0x0018
		__int32 trZ;	//0x001C
		unsigned __int16 lMatrix[10];	//0x0020
		__int32 rbk;	//0x0034
		__int32 gbk;	//0x0038
		__int32 bbk;	//0x003C
		unsigned __int16 cMatrix[10];	//0x0040
		__int32 rfc;	//0x0054
		__int32 gfc;	//0x0058
		__int32 bfc;	//0x005C
		__int32 ofx;	//0x0060
		__int32 ofy;	//0x0064
		__int32 h;		//0x0068
		__int32 dqa;	//0x006C
		__int32 dqb;	//0x0070
		__int32 zsf3;	//0x0074
		__int32 zsf4;	//0x0078
		__int32 flag;	//0x007C
	};	//Size: 0x0080

	struct psxRegisters
	{
		GPRRegs GPR;				//	0x0000	//	General Purpose Rehisters
		CP0Regs CP0;				//	0x0088	//	Coprocessor0 Registers
		CP2Data CP2D;				//	0x0108	//	Cop2 data registers
		CP2Ctrl CP2C;				//	0x0188	//	Cop2 control registers
		__int32 pc;					//	0x0208	//	Program counter
		__int32 code;				//	0x020C	//	The instruction
		__int32 cycle;				//	0x0210	//	
		__int32 interrupt;			//	0x0214	//	
		__int32 pcWriteback;		//	0x0218	//	
		__int32 iopNextEventCycle;	//	0x021C	//	Controls when branch tests are performed.
		
		// This value is used when the IOP execution is broken to return control to the EE.
		// (which happens when the IOP throws EE-bound interrupts).  It holds the value of
		// iopCycleEE (which is set to zero to facilitate the code break), so that the unrun
		// cycles can be accounted for later.
		__int32 iopBreak;			//	0x0220	


		// Tracks current number of cycles IOP can run in EE cycles. When it dips below zero,
		// control is returned to the EE.
		__int32 iopCycleEE;			//	0x0224

		__int32 sCycle[32];			//	0x0228	//	start cycle for signaled ints
		__int32 eCycle[32];			//	0x022C	//	 cycle delta for signaled ints
	};	//Size: 0x021C
}
#pragma pack(pop)