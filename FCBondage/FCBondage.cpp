#include "stdafx.h"
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
	std::cout << ++pcount <<". Item: ~" << GetItemNameFromID(packet->ItemID) << "~ || New SB: " << NewSB << " %\n";
	return Real_ReadObtainedItemPacket(unk,packet);
}

VOID __stdcall InitHook()
{
	MODULEINFO modinfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA("ffxiv.exe"), &modinfo, sizeof(MODULEINFO));
	SetFunctionPointer(Real_ReadObtainedItemPacket, FindPattern(ItemPacket_FuncSig,"xxxxxxx????xxxxxxx", &modinfo));
	SetFunctionPointer(GetItemInfo, FindPattern(GetItemInfo_FuncSig,"xxxxxxx????xxx", &modinfo));
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

const char* __stdcall GetItemNameFromID(USHORT ItemID)
{
	auto itemInfo = GetItemInfo(ItemID);
	if (itemInfo == NULL)
		return NULL;
	return &reinterpret_cast<const char*>(itemInfo)[itemInfo->OffsetOfName + offsetof(BasicItemInfo,OffsetOfName)];
}
