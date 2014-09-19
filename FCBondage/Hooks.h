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

#ifndef _HOOKS_H_
#define _HOOKS_H_

GetItemInfoFunction GetItemInfo;
GetUserInputObjectFromRootInputFunction GetUserInputObjectFromRootInput;
InitCommandStructFunction InitCommandStruct;
ReadObtainedItemPacketFunction Real_ReadObtainedItemPacket;
VerifyThreadOriginFunction Real_VerifyThreadOrigin;
IncomingCommandFunction Real_IncomingCommand;
__int32 __cdecl My_ReadObtainedItemPacket(__int32 unk, ItemUpdatePacket* packet);
__int32 __fastcall My_VerifyThreadOrigin(void* pThis, __int32, __int32 a2, __int8 a3, __int32 a4);
char __fastcall My_IncomingCommand(void* pChatLogObject, __int32, CommandStruct* pCmdStruct, UserInputObject* pInputObject);

#endif // !_HOOKS_H_