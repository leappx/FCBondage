#include "stdafx.h"
#include "ItemData.h"
#include "FCBondage.h"
#include "SigScan.h"
#include <iostream>
#include <detours.h>
#pragma comment( lib, "detours.lib" )

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		SpawnConsole();
		PopulateItemDatabase();
		InitHook();
		break;
	default:
		break;
	}
	return TRUE;
}

INT __cdecl My_ReadObtainedItemPacket(INT unk, ItemObtainedPacket* packet)
{
	auto NewSB = packet->SpiritBind * 0.01;
	auto ItemName = ItemMap[packet->ItemID];
	std::cout << ++pcount <<". Item: ~" << ItemName << "~ || New SB: " << NewSB << " %\n";
	return Real_ReadObtainedItemPacket(unk,packet);
}

VOID __stdcall InitHook()
{
	MODULEINFO modinfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA("ffxiv.exe"), &modinfo, sizeof(MODULEINFO));
	SetFunctionPointer(Real_ReadObtainedItemPacket, FindPattern(FunctionSig,"xxxxxxx????xxxxxxx", &modinfo));
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)Real_ReadObtainedItemPacket,(PVOID)My_ReadObtainedItemPacket);
	DetourTransactionCommit();
}

VOID __stdcall SpawnConsole()
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}