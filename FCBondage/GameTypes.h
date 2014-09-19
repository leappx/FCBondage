#ifndef _GAME_TYPES_H_
#define _GAME_TYPES_H_
typedef void* (__thiscall* GetChatLogObject)(void* pUserInputObject);

typedef struct UserInputObject
{
	GetChatLogObject* CalculateAddressOfChatLog;
} _UserInputObject, *PUserInputObject;
#pragma pack(push, 1)

typedef struct ItemUpdatePacket
{
	__int64 : 64;
	__int64 : 64;
	unsigned __int16 ItemID;
	__int64 : 64;
	__int64 : 64;
	__int16 : 16;
	unsigned __int16 SpiritBind;
} _ItemUpdatePacket, *PItemUpdatePacket;

typedef struct BasicItemInfo
{
	__int64 : 64;
	__int32 : 32;
	unsigned __int16 OffsetOfName;
} _BasicItemInfo, *PBasicItemInfo;

typedef struct CommandStruct
{
	const char* pCommandString;
	unsigned __int32 BufferSize;
	unsigned __int32 CommandStrlen;
	unsigned __int32 UnkUint32;
	unsigned __int16 UnkUint16;
	char StringBuffer[512];
} _CommandStruct, *PCommandStruct;

#pragma pack(pop)
typedef PBasicItemInfo (_cdecl* GetItemInfoFunction)(unsigned __int16);
typedef PUserInputObject (__thiscall* GetUserInputObjectFromRootInputFunction)(void* pRootInputObject);
typedef PCommandStruct (__thiscall* InitCommandStructFunction)(PCommandStruct pCmdStruct, const char* cmdStr, size_t strSize);
typedef char (__thiscall* IncomingCommandFunction)(void* pChatLogObject, PCommandStruct pCmdStruct, PUserInputObject pInputObject);
typedef __int32 (__thiscall* VerifyThreadOriginFunction)(void* pThis, __int32 a2, __int8 a3, __int32 a4);
typedef __int32 (__cdecl* ReadObtainedItemPacketFunction)(__int32,PItemUpdatePacket);
#endif
