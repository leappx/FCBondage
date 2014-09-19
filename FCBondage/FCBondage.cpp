#include "stdafx.h"
#include "FCBondage.h"
#include "SigScan.h"
#include <iostream>
#include <future>
#include <sstream>
#include <detours.h>
#pragma comment( lib, "detours.lib" )



BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		InitHook();
		break;
	default:
		break;
	}
	return TRUE;
}

INT __cdecl My_ReadObtainedItemPacket(INT unk, ItemObtainedPacket* packet)
{
	if (SetValues(packet->ItemID, packet->SpiritBind) && AutoUpdateFlag)
		std::async(std::launch::async, &ShowCurrentValues);
	if (ConsoleFlag || ShowAllFlag)
	{
		auto NewSB = packet->SpiritBind * 0.01;
		auto ItemName = GetItemNameFromID(packet->ItemID);
		if (ShowAllFlag)
		{
			std::stringstream ss;
			ss << "/echo ~" << (ItemName != 0 ? ItemName : "Item Not Found") << "~ || SB: " << NewSB << " %";
			std::async(std::launch::async, &SendCommand,ss.str());
		}
		if (ConsoleFlag)
			std::cout << ++pcount <<". Item: ~" << (ItemName != 0 ? ItemName : "Item Not Found") << "~ || New SB: " << NewSB << " %\n";
	}
	return Real_ReadObtainedItemPacket(unk,packet);
}

VOID __stdcall InitHook()
{
	MODULEINFO modinfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA("ffxiv.exe"), &modinfo, sizeof(MODULEINFO));
	SetFunctionPointer(Real_ReadObtainedItemPacket, FindPattern(ItemPacket_FuncSig,"xxxxxxx????xxxxxxx", &modinfo));
	SetFunctionPointer(GetItemInfo, FindPattern(GetItemInfo_FuncSig,"xxxxxxx????xxx", &modinfo));
	SetFunctionPointer(Real_IncommingCommand, FindPattern(IncommingCommand_FuncSig,"xxxxxxxxxxxxxx", &modinfo) - 0x23);
	SetFunctionPointer(InitCommandStruct, FindPattern(InitCommandStruct_FuncSig,"xxxxxxxxxxxx", &modinfo) - 0x2D);
	InitInstanceRetrievalRoutines(&modinfo);
	InitThreadSecurity(&modinfo);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)Real_ReadObtainedItemPacket,(PVOID)My_ReadObtainedItemPacket);
	DetourAttach(&(PVOID&)Real_IncommingCommand,(PVOID)My_IncommingCommand);
	DetourAttach(&(PVOID&)Real_VerifyThreadOgrigin,(PVOID)My_VerifyThreadOgrigin);
	DetourTransactionCommit();
	std::async(std::launch::async, &SendCommand,"/echo SB Tracker Loaded...");
}

VOID __stdcall ToggleConsole(BOOL on)
{
	ConsoleFlag = on;
	if (on)
	{
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		return;
	}
	FreeConsole();
}

const char* __stdcall GetItemNameFromID(USHORT ItemID)
{
	std::lock_guard<std::mutex> lock(GetItemMutex);
	const char* result;
	try
	{
		auto itemInfo = GetItemInfo(ItemID);
		if (itemInfo == NULL)
			return NULL;
		result = &reinterpret_cast<const char*>(itemInfo)[itemInfo->OffsetOfName + offsetof(BasicItemInfo,OffsetOfName)];
	}
	catch (...)
	{
		result = 0;
	}
	return result;
}

VOID __stdcall InitInstanceRetrievalRoutines(void* modinfo)
{
	auto pppRootInputObject = FindPattern(Input_ObjectSig,"xxxxxxxxxxxxxxx", (MODULEINFO*)modinfo) -0x24;
	ppRootInputObject = *(void***)pppRootInputObject;
	int JmpDistance = *(int*)(pppRootInputObject + 5) + 9;
	SetFunctionPointer(GetUserInputObjectFromRootInput, pppRootInputObject + JmpDistance);
}

VOID __stdcall InitThreadSecurity(void* modinfo)
{
	auto ppTls = FindPattern(VerifyThreadOgrigin_FuncSig,"xxxxxxx????x",(MODULEINFO*)modinfo) + 7;
	pThreadVerifyIndex = *(PDWORD*)ppTls;
	SetFunctionPointer(Real_VerifyThreadOgrigin, ppTls - 4);
}

BOOL __fastcall My_IncommingCommand(void* pChatLogObject, int, CommandStruct* pCmdStruct, UserInputObject* pInputObject)
{
	if (pCmdStruct->pCommandString[1] != 0x2F)
		return Real_IncommingCommand(pChatLogObject,pCmdStruct,pInputObject);

	auto firstArg = FirstArg(pCmdStruct->pCommandString);
	if (StringContains(pCmdStruct->pCommandString, "//console"))
	{
		if (firstArg == NULL)
			return 0;
		if (!strcmp(firstArg,"on"))
			ToggleConsole(TRUE);
		else if (!strcmp(firstArg,"off"))
			ToggleConsole(FALSE);
		return 1;
	}
	if (StringContains(pCmdStruct->pCommandString, "//autoupdate"))
	{
		if (firstArg == NULL)
			return 0;
		if (!strcmp(firstArg,"on"))
			AutoUpdateFlag = TRUE;
		else if (!strcmp(firstArg,"off"))
			AutoUpdateFlag = FALSE;
		return 1;
	}
	if (StringContains(pCmdStruct->pCommandString, "//showall"))
	{
		if (firstArg == NULL)
			return 0;
		if (!strcmp(firstArg,"on"))
			ShowAllFlag = TRUE;
		else if (!strcmp(firstArg,"off"))
			ShowAllFlag = FALSE;
		return 1;
	}
	if (StringContains(pCmdStruct->pCommandString, "//showcur"))
	{
		std::async(std::launch::async, &ShowCurrentValues);
		return 1;
	}
	return 0;
}

VOID __stdcall SendCommand(std::string pCmdStr)
{
	std::lock_guard<std::mutex> lock(CommandMutex);
	try
	{
		CommandStruct cmdStruct;
		InitCommandStruct(&cmdStruct,pCmdStr.c_str(),-1);
		auto userInputObject = GetUserInputObjectFromRootInput(*ppRootInputObject);
		auto chatLogObject = userInputObject->CalculateAddressOfChatLog[8](userInputObject);
		Real_IncommingCommand(chatLogObject,&cmdStruct,userInputObject);
	}
	catch (...)
	{
	}
}

INT __fastcall My_VerifyThreadOgrigin(VOID* pThis, INT, INT a2, CHAR a3, INT a4)
{
	auto ctTls = FastTls(*pThreadVerifyIndex, NULL);
	if (ctTls == NULL)
		FastTls(*pThreadVerifyIndex, pThreadVerify);
	else
		pThreadVerify = ctTls;
	return Real_VerifyThreadOgrigin(pThis,a2,a3,a4);
}

BOOL __stdcall StringContains(const char* str1, const char* str2)
{
	return strstr(str1,str2) != NULL;
}

const char* __stdcall FirstArg(const char* pCmd)
{
	while (pCmd[0])
	{
		if (pCmd++[0] == ' ')
			return pCmd;
	}
	return NULL;
}

BOOL __stdcall SetValues(UINT16 itemID, UINT16 newSB)
{
	auto NewSB = newSB * 0.01;
	switch (itemID)
	{
	case 7863:
		Curtana = NewSB;
		return TRUE;
	case 7864:
		Sphairai = NewSB;
		return TRUE;
	case 7865:
		Bravura = NewSB;
		return TRUE;
	case 7866:
		GaeBolg = NewSB;
		return TRUE;
	case 7867:
		ArtemisBow = NewSB;
		return TRUE;
	case 7868:
		Thyrus = NewSB;
		return TRUE;
	case 7869:
		StardustRod = NewSB;
		return TRUE;
	case 7870:
		VeilOfWiyu = NewSB;
		return TRUE;
	case 7871:
		Omnilex = NewSB;
		return TRUE;
	case 7872:
		HolyShield = NewSB;
		return TRUE;
	default:
		return FALSE;
	}
	return FALSE;
}

VOID __stdcall ShowCurrentValues()
{
	const char* itemName = NULL;

	if (Curtana != NULL)
	{
		itemName = GetItemNameFromID(7863);
		if (itemName == NULL)
		{
			Sleep(500);
			itemName = GetItemNameFromID(7863);
		}
		std::stringstream ss;
		ss << "/echo ~" << (itemName != 0 ? itemName : "Item Not Found") << "~ || SB: " << Curtana << " %";
		SendCommand(ss.str().c_str());
	}
	if (Sphairai != NULL)
	{
		itemName = GetItemNameFromID(7864);
		if (itemName == NULL)
		{
			Sleep(500);
			itemName = GetItemNameFromID(7864);
		}
		std::stringstream ss;
		ss << "/echo ~" << (itemName != 0 ? itemName : "Item Not Found") << "~ || SB: " << Sphairai << " %";
		SendCommand(ss.str().c_str());
	}
	if (Bravura != NULL)
	{
		itemName = GetItemNameFromID(7865);
		if (itemName == NULL)
		{
			Sleep(500);
			itemName = GetItemNameFromID(7865);
		}
		std::stringstream ss;
		ss << "/echo ~" << (itemName != 0 ? itemName : "Item Not Found") << "~ || SB: " << Bravura << " %";
		SendCommand(ss.str().c_str());
	}
	if (GaeBolg != NULL)
	{
		itemName = GetItemNameFromID(7866);
		if (itemName == NULL)
		{
			Sleep(500);
			itemName = GetItemNameFromID(7866);
		}
		std::stringstream ss;
		ss << "/echo ~" << (itemName != 0 ? itemName : "Item Not Found") << "~ || SB: " << GaeBolg << " %";
		SendCommand(ss.str().c_str());
	}
	if (ArtemisBow != NULL)
	{
		itemName = GetItemNameFromID(7867);
		if (itemName == NULL)
		{
			Sleep(500);
			itemName = GetItemNameFromID(7867);
		}
		std::stringstream ss;
		ss << "/echo ~" << (itemName != 0 ? itemName : "Item Not Found") << "~ || SB: " << ArtemisBow << " %";
		SendCommand(ss.str().c_str());
	}
	if (Thyrus != NULL)
	{
		itemName = GetItemNameFromID(7868);
		if (itemName == NULL)
		{
			Sleep(500);
			itemName = GetItemNameFromID(7868);
		}
		std::stringstream ss;
		ss << "/echo ~" << (itemName != 0 ? itemName : "Item Not Found") << "~ || SB: " << Thyrus << " %";
		SendCommand(ss.str().c_str());
	}
	if (StardustRod != NULL)
	{
		itemName = GetItemNameFromID(7869);
		if (itemName == NULL)
		{
			Sleep(500);
			itemName = GetItemNameFromID(7869);
		}
		std::stringstream ss;
		ss << "/echo ~" << (itemName != 0 ? itemName : "Item Not Found") << "~ || SB: " << StardustRod << " %";
		SendCommand(ss.str().c_str());
	}
	if (VeilOfWiyu != NULL)
	{
		itemName = GetItemNameFromID(7870);
		if (itemName == NULL)
		{
			Sleep(500);
			itemName = GetItemNameFromID(7870);
		}
		std::stringstream ss;
		ss << "/echo ~" << (itemName != 0 ? itemName : "Item Not Found") << "~ || SB: " << VeilOfWiyu << " %";
		SendCommand(ss.str().c_str());
	}
	if (Omnilex != NULL)
	{
		itemName = GetItemNameFromID(7871);
		if (itemName == NULL)
		{
			Sleep(500);
			itemName = GetItemNameFromID(7871);
		}
		std::stringstream ss;
		ss << "/echo ~" << (itemName != 0 ? itemName : "Item Not Found") << "~ || SB: " << Omnilex << " %";
		SendCommand(ss.str().c_str());
	}
	if (HolyShield != NULL)
	{
		itemName = GetItemNameFromID(7872);
		if (itemName == NULL)
		{
			Sleep(500);
			itemName = GetItemNameFromID(7872);
		}
		std::stringstream ss;
		ss << "/echo ~" << (itemName != 0 ? itemName : "Item Not Found") << "~ || SB: " << HolyShield << " %";
		SendCommand(ss.str().c_str());
	}
}
