/**=================================================================================================
 * File: FCBondage.cpp
 * =================================================================================================
 * \file FCBondage.cpp
 *
 * \brief Implements the fc bondage class.
 */

#include "stdafx.h"
#include "FCBondage.h"
#include "SigScan.h"
#include <iostream>
#include <future>
#include <sstream>
#include <detours.h>
#pragma comment( lib, "detours.lib" )

/**=================================================================================================
 * Method: DllMain
 * =================================================================================================
 * \fn BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
 *
 * \brief DLL main.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param parameter1 The first parameter.
 * \param fdwReason  The fdw reason.
 * \param parameter3 The third parameter.
 *
 * \return A WINAPI.
 */

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

/**=================================================================================================
 * Method: My_ReadObtainedItemPacket
 * =================================================================================================
 * \fn INT __cdecl My_ReadObtainedItemPacket(INT unk, ItemObtainedPacket* packet)
 *
 * \brief My read obtained item packet.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param unk			  The unk.
 * \param [in,out] packet If non-null, the packet.
 *
 * \return An INT.
 */

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

/**=================================================================================================
 * Method: InitHook
 * =================================================================================================
 * \fn VOID __stdcall InitHook()
 *
 * \brief Initialises the hook.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \return A VOID.
 */

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

/**=================================================================================================
 * Method: ToggleConsole
 * =================================================================================================
 * \fn VOID __stdcall ToggleConsole(BOOL on)
 *
 * \brief Toggle console.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param on true to on.
 *
 * \return A VOID.
 */

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

/**=================================================================================================
 * Method: GetItemNameFromID
 * =================================================================================================
 * \fn const char* __stdcall GetItemNameFromID(USHORT ItemID)
 *
 * \brief Gets item name from identifier.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param ItemID Identifier for the item.
 *
 * \return null if it fails, else the item name from identifier.
 */

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

/**=================================================================================================
 * Method: InitInstanceRetrievalRoutines
 * =================================================================================================
 * \fn VOID __stdcall InitInstanceRetrievalRoutines(void* modinfo)
 *
 * \brief Initialises the instance retrieval routines.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param [in,out] modinfo If non-null, the modinfo.
 *
 * \return A VOID.
 */

VOID __stdcall InitInstanceRetrievalRoutines(void* modinfo)
{
	auto pppRootInputObject = FindPattern(Input_ObjectSig,"xxxxxxxxxxxxxxx", (MODULEINFO*)modinfo) -0x24;
	ppRootInputObject = *(void***)pppRootInputObject;
	int JmpDistance = *(int*)(pppRootInputObject + 5) + 9;
	SetFunctionPointer(GetUserInputObjectFromRootInput, pppRootInputObject + JmpDistance);
}

/**=================================================================================================
 * Method: InitThreadSecurity
 * =================================================================================================
 * \fn VOID __stdcall InitThreadSecurity(void* modinfo)
 *
 * \brief Initialises the thread security.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param [in,out] modinfo If non-null, the modinfo.
 *
 * \return A VOID.
 */

VOID __stdcall InitThreadSecurity(void* modinfo)
{
	auto ppTls = FindPattern(VerifyThreadOgrigin_FuncSig,"xxxxxxx????x",(MODULEINFO*)modinfo) + 7;
	pThreadVerifyIndex = *(PDWORD*)ppTls;
	SetFunctionPointer(Real_VerifyThreadOgrigin, ppTls - 4);
}

/**=================================================================================================
 * Method: My_IncommingCommand
 * =================================================================================================
 * \fn BOOL __fastcall My_IncommingCommand(void* pChatLogObject, int, CommandStruct* pCmdStruct, UserInputObject* pInputObject)
 *
 * \brief My incomming command.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param [in,out] pChatLogObject If non-null, the chat log object.
 * \param parameter2			  The second parameter.
 * \param [in,out] pCmdStruct	  If non-null, the command structure.
 * \param [in,out] pInputObject   If non-null, the input object.
 *
 * \return true if it succeeds, false if it fails.
 */

BOOL __fastcall My_IncommingCommand(void* pChatLogObject, int, CommandStruct* pCmdStruct, UserInputObject* pInputObject)
{
	if (pCmdStruct->pCommandString[1] != 0x2F)
		return Real_IncommingCommand(pChatLogObject,pCmdStruct,pInputObject);

	auto firstArg = FirstArg(pCmdStruct->pCommandString);
	if (StringContains(pCmdStruct->pCommandString, "//console"))
	{
		if (firstArg == NULL)
			return Real_IncommingCommand(pChatLogObject,pCmdStruct,pInputObject);
		if (!strcmp(firstArg,"on"))
		{
			ToggleConsole(TRUE);
			std::async(std::launch::async, &SendCommand,"/echo Console Opened.");
		}
		else if (!strcmp(firstArg,"off"))
		{
			ToggleConsole(FALSE);
			std::async(std::launch::async, &SendCommand,"/echo Console Closed.");
		}
		return 1;
	}
	if (StringContains(pCmdStruct->pCommandString, "//autoupdate"))
	{
		if (firstArg == NULL)
			return Real_IncommingCommand(pChatLogObject,pCmdStruct,pInputObject);
		if (!strcmp(firstArg,"on"))
		{
			AutoUpdateFlag = TRUE;
			std::async(std::launch::async, &SendCommand,"/echo Auto Updating Values.");
		}
		else if (!strcmp(firstArg,"off"))
		{
			AutoUpdateFlag = FALSE;
			std::async(std::launch::async, &SendCommand,"/echo Not Auto Updating Values.");
		}
		return 1;
	}
	if (StringContains(pCmdStruct->pCommandString, "//showall"))
	{
		if (firstArg == NULL)
			return Real_IncommingCommand(pChatLogObject,pCmdStruct,pInputObject);
		if (!strcmp(firstArg,"on"))
		{
			ShowAllFlag = TRUE;
			std::async(std::launch::async, &SendCommand,"/echo Showing All Items.");
		}
		else if (!strcmp(firstArg,"off"))
		{
			ShowAllFlag = FALSE;
			std::async(std::launch::async, &SendCommand,"/echo Showing Only Novus Items.");
		}
		return 1;
	}
	if (StringContains(pCmdStruct->pCommandString, "//showcur"))
	{
		std::async(std::launch::async, &ShowCurrentValues);
		return 1;
	}
	return Real_IncommingCommand(pChatLogObject,pCmdStruct,pInputObject);
}

/**=================================================================================================
 * Method: SendCommand
 * =================================================================================================
 * \fn VOID __stdcall SendCommand(std::string pCmdStr)
 *
 * \brief Sends a command.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param pCmdStr The command string.
 *
 * \return A VOID.
 */

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

/**=================================================================================================
 * Method: My_VerifyThreadOgrigin
 * =================================================================================================
 * \fn INT __fastcall My_VerifyThreadOgrigin(VOID* pThis, INT, INT a2, CHAR a3, INT a4)
 *
 * \brief My verify thread origin.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param [in,out] pThis If non-null, this.
 * \param parameter2	 The second parameter.
 * \param a2			 The second INT.
 * \param a3			 The third CHAR.
 * \param a4			 The fourth INT.
 *
 * \return An INT.
 */

INT __fastcall My_VerifyThreadOrigin(VOID* pThis, INT, INT a2, CHAR a3, INT a4)
{
	auto ctTls = FastTls(*pThreadVerifyIndex, NULL);
	if (ctTls == NULL)
		FastTls(*pThreadVerifyIndex, pThreadVerify);
	else
		pThreadVerify = ctTls;
	return Real_VerifyThreadOgrigin(pThis,a2,a3,a4);
}

/**=================================================================================================
 * Method: StringContains
 * =================================================================================================
 * \fn BOOL __stdcall StringContains(const char* str1, const char* str2)
 *
 * \brief String contains.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param str1 The first string.
 * \param str2 The second string.
 *
 * \return true if it succeeds, false if it fails.
 */

BOOL __stdcall StringContains(const char* str1, const char* str2)
{
	return strstr(str1,str2) != NULL;
}

/**=================================================================================================
 * Method: FirstArg
 * =================================================================================================
 * \fn const char* __stdcall FirstArg(const char* pCmd)
 *
 * \brief First argument.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param pCmd The command.
 *
 * \return null if it fails, else a char*.
 */

const char* __stdcall FirstArg(const char* pCmd)
{
	while (pCmd[0])
	{
		if (pCmd++[0] == ' ')
			return pCmd;
	}
	return NULL;
}

/**=================================================================================================
 * Method: SetValues
 * =================================================================================================
 * \fn BOOL __stdcall SetValues(UINT16 itemID, UINT16 newSB)
 *
 * \brief Sets the values.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param itemID Identifier for the item.
 * \param newSB  The new sb.
 *
 * \return true if it succeeds, false if it fails.
 */

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

/**=================================================================================================
 * Method: ShowCurrentValues
 * =================================================================================================
 * \fn VOID __stdcall ShowCurrentValues()
 *
 * \brief Shows the current values.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \return A VOID.
 */

VOID __stdcall ShowCurrentValues()
{
	const char* itemName = NULL;

	if (Curtana != NULL)
		PrintInfoToChatLog(7863,Curtana);
	if (Sphairai != NULL)
		PrintInfoToChatLog(7864,Sphairai);
	if (Bravura != NULL)
		PrintInfoToChatLog(7865,Bravura);
	if (GaeBolg != NULL)
		PrintInfoToChatLog(7866,GaeBolg);
	if (ArtemisBow != NULL)
		PrintInfoToChatLog(7867,ArtemisBow);
	if (Thyrus != NULL)
		PrintInfoToChatLog(7868,Thyrus);
	if (StardustRod != NULL)
		PrintInfoToChatLog(7869,StardustRod);
	if (VeilOfWiyu != NULL)
		PrintInfoToChatLog(7870,VeilOfWiyu);
	if (Omnilex != NULL)
		PrintInfoToChatLog(7871,Omnilex);
	if (HolyShield != NULL)
		PrintInfoToChatLog(7872,HolyShield);
}

/**=================================================================================================
 * Method: PrintInfoToChatLog
 * =================================================================================================
 * \fn VOID __stdcall PrintInfoToChatLog(UINT16 itemID, double SB)
 *
 * \brief Print information to chat log.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param itemID Identifier for the item.
 * \param SB	 The sb.
 *
 * \return A VOID.
 */

VOID __stdcall PrintInfoToChatLog(UINT16 itemID, double SB)
{
	auto itemName = GetItemNameFromID(itemID);
	if (itemName == NULL)
	{
		Sleep(500);
		itemName = GetItemNameFromID(itemID);
	}
	std::stringstream ss;
	ss << "/echo ~" << (itemName != 0 ? itemName : "Item Not Found") << "~ || SB: " << SB << " %";
	SendCommand(ss.str());
}
