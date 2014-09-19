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