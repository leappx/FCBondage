/*
	Copyright (C) 2014  DevNull:ffevo

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**=================================================================================================
 * File: FCBondage.cpp
 * =================================================================================================
 * \file FCBondage.cpp
 *
 * \brief Implements the fc bondage class.
 */

#include "stdafx.h"
#include "FCBondage.h"
#include "Sigs.h"
#include "Hooks.h"
#include <detours.h>
#pragma comment( lib, "detours.lib" )

/**=================================================================================================
 * Member: TrackerObject
 * =================================================================================================
 * \brief The tracker object.
 */

FCBondage::SBTracker* TrackerObject = NULL;

/**=================================================================================================
 * Method: DllMain
 * =================================================================================================
 * \fn bool WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
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
 * \return A bool.
 */

bool WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		InitHook();
		break;
	default:
		break;
	}
	return true;
}

/**=================================================================================================
 * Method: InitHook
 * =================================================================================================
 * \fn void __stdcall InitHook()
 *
 * \brief Initializes the hook.
 *
 * \author DevNull
 * \date 9/19/2014
 */

void __stdcall InitHook()
{
	MODULEINFO modinfo;

	// Retrieve the module info structure of the "ffxiv.exe" base executable.
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA("ffxiv.exe"), &modinfo, sizeof(MODULEINFO));

	// Scan for and set our proxy function pointers.
	SetFunctionPointer(Real_ReadObtainedItemPacket, FindPattern(ItemPacket_FuncSig,"xxxxxxx????xxxxxxx", &modinfo));
	SetFunctionPointer(Real_IncomingCommand, FindPattern(IncomingCommand_FuncSig,"xxxxxxxxxxxxxx", &modinfo) - 0x23);
	
	// Preform the hooking and instantiation process.
	// TODO: Clean this up, order of operations is causing it to be an eyesore.
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)Real_ReadObtainedItemPacket,(PVOID)My_ReadObtainedItemPacket);
	DetourAttach(&(PVOID&)Real_IncomingCommand,(PVOID)My_IncomingCommand);
	TrackerObject = new FCBondage::SBTracker(Real_IncomingCommand);
	SetFunctionPointer(TrackerObject->InitCommandStruct, FindPattern(InitCommandStruct_FuncSig,"xxxxxxxxxxxx", &modinfo) - 0x2D);
	SetFunctionPointer(TrackerObject->GetItemInfo, FindPattern(GetItemInfo_FuncSig,"xxxxxxx????xxx", &modinfo));
	InitInstanceRetrievalRoutines(&modinfo);
	InitThreadSecurity(&modinfo);
	DetourAttach(&(PVOID&)Real_VerifyThreadOrigin,(PVOID)My_VerifyThreadOrigin);
	DetourTransactionCommit();

	// Print to chat that we are finished.
	std::async(std::launch::async, &FCBondage::SBTracker::SendCommand,TrackerObject,"/echo SB Tracker Loaded...");
}

/**=================================================================================================
 * Method: InitInstanceRetrievalRoutines
 * =================================================================================================
 * \fn void __stdcall InitInstanceRetrievalRoutines(MODULEINFO* modinfo)
 *
 * \brief Initializes the instance retrieval routines.
 * 		  NOTE: These routines are the functions the game uses to obtain pointers for a needed class object.
 * 				We can also use these for such, and doing so eliminates the need for static pointers.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param [in,out] modinfo If non-null, the modinfo.
 */

void __stdcall InitInstanceRetrievalRoutines(MODULEINFO* modinfo)
{
	auto pppRootInputObject = FindPattern(Input_ObjectSig,"xxxxxxxxxxxxxxx", modinfo) -0x24;
	TrackerObject->ppRootInputObject = *(void***)pppRootInputObject;
	int JmpDistance = *(int*)(pppRootInputObject + 5) + 9;
	SetFunctionPointer(TrackerObject->GetUserInputObjectFromRootInput, pppRootInputObject + JmpDistance);
}

/**=================================================================================================
 * Method: InitThreadSecurity
 * =================================================================================================
 * \fn void __stdcall InitThreadSecurity(MODULEINFO* modinfo)
 *
 * \brief Initializes the thread security.
 * 		  NOTE: Various routines within the game, can only be called from a thread created by the game.
 * 				We can bypass this by making sure all of the threads "we" create, inherit the game's TIB values.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param [in,out] modinfo If non-null, the modinfo.
 */

void __stdcall InitThreadSecurity(MODULEINFO* modinfo)
{
	auto ppTls = FindPattern(VerifyThreadOrigin_FuncSig,"xxxxxxx????x", modinfo) + 7;
	TrackerObject->pThreadVerifyIndex = *(unsigned __int32**)ppTls;
	SetFunctionPointer(Real_VerifyThreadOrigin, ppTls - 4);
}

/**=================================================================================================
 * Method: My_ReadObtainedItemPacket
 * =================================================================================================
 * \fn __int32 __cdecl My_ReadObtainedItemPacket(INT unk, ItemUpdatePacket* packet)
 *
 * \brief Our hook into the games Item Update function.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param unk			  The unk.
 * \param [in,out] packet If non-null, the packet.
 */

__int32 __cdecl My_ReadObtainedItemPacket(INT unk, ItemUpdatePacket* packet)
{
	TrackerObject->HandleItemPacket(packet->ItemID,packet->SpiritBind);
	return Real_ReadObtainedItemPacket(unk,packet);
}

/**=================================================================================================
 * Method: My_IncomingCommand
 * =================================================================================================
 * \fn char __fastcall My_IncomingCommand(void* pChatLogObject, __int32, CommandStruct* pCmdStruct, UserInputObject* pInputObject)
 *
 * \brief Our hook into the games command handler.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param [in,out] pChatLogObject If non-null, the chat log object.
 * \param parameter2			  The second parameter.
 * \param [in,out] pCmdStruct	  If non-null, the command structure.
 * \param [in,out] pInputObject   If non-null, the input object.
 *
 * \return true = discard command, false = handle command.
 */

char __fastcall My_IncomingCommand(void* pChatLogObject, __int32, CommandStruct* pCmdStruct, UserInputObject* pInputObject)
{
	return TrackerObject->HandleCommand(pCmdStruct->pCommandString) ? 1 : Real_IncomingCommand(pChatLogObject,pCmdStruct,pInputObject);
}

/**=================================================================================================
 * Method: My_VerifyThreadOrigin
 * =================================================================================================
 * \fn __int32 __fastcall My_VerifyThreadOrigin(void* pThis, __int32, __int32 a2, __int8 a3, __int32 a4)
 *
 * \brief Our hook into the games thread verification routine.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param [in,out] pThis If non-null, this.
 * \param parameter2	 The second parameter.
 * \param a2			 The second __int32.
 * \param a3			 The third __int8.
 * \param a4			 The fourth __int32.
 */

__int32 __fastcall My_VerifyThreadOrigin(void* pThis, __int32, __int32 a2, __int8 a3, __int32 a4)
{
	auto ctTls = FastTls(*TrackerObject->pThreadVerifyIndex, NULL);
	if (ctTls == NULL)
		FastTls(*TrackerObject->pThreadVerifyIndex, TrackerObject->pThreadVerify);
	else
		TrackerObject->pThreadVerify = ctTls;
	return Real_VerifyThreadOrigin(pThis,a2,a3,a4);
}
