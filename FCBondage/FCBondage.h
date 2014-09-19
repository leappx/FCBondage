#include <string>
#include <mutex>
#define SetFunctionPointer(f,p) *(PINT)(&f) = p

typedef void* (__thiscall* GetChatLogObject)(void* pUserInputObject);

struct UserInputObject
{
	GetChatLogObject* CalculateAddressOfChatLog;
};

#pragma pack(push, 1)
struct ItemObtainedPacket
{
	__int64 : 64;
	__int64 : 64;
	unsigned __int16 ItemID;
	__int64 : 64;
	__int64 : 64;
	__int16 : 16;
	unsigned __int16 SpiritBind;
};

struct BasicItemInfo
{
	__int64 : 64;
	__int32 : 32;
	unsigned __int16 OffsetOfName;
};

struct CommandStruct
{
	const char* pCommandString;
	unsigned __int32 BufferSize;
	unsigned __int32 CommandStrlen;
	unsigned __int32 UnkUint32;
	unsigned __int16 UnkUint16;
	char StringBuffer[512];
};

#pragma pack(pop)

UINT8 VerifyThreadOgrigin_FuncSig[12] =
{
	0x5E, 0x5B, 0xC3, 0x55, 0x8B, 0xEC, 0xA1, 0x88, 0x8A, 0x4B, 0x02, 0x56
};

UINT8 Input_ObjectSig[15] =
{
	0x5D, 0xFF, 0xE2, 0x8B, 0x01, 0x8B, 0x50, 0x2C, 0x5D, 0xFF, 0xE2, 0x8B, 0x01, 0x8B, 0x50
};

UINT8 InitCommandStruct_FuncSig[12] =
{
	0x89, 0x06, 0xC6, 0x00, 0x00, 0x83, 0xFF, 0xFF, 0x75, 0x14, 0x8B, 0xC3
};

UINT8 IncommingCommand_FuncSig[14] =
{
	0x8D, 0x55, 0xDC, 0x52, 0x56, 0x8B, 0xCF, 0x89, 0x45, 0xEC, 0x89, 0x45, 0xF0, 0x89
};

UINT8 GetItemInfo_FuncSig[14] =
{
	0x55, 0x8B, 0xEC, 0x8B, 0x45, 0x08, 0x3D, 0x40, 0x42, 0x0F, 0x00, 0x72, 0x05, 0x2D
};

UINT8 ItemPacket_FuncSig[18] = 
{
	0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x44, 0xA1, 0x68, 0xC0, 0x11, 0x02, 0x33, 0xC5, 0x89, 0x45, 0xFC, 0x53, 0x57
};

double Curtana = NULL;
double Sphairai = NULL;
double Bravura = NULL;
double GaeBolg = NULL;
double ArtemisBow = NULL;
double Thyrus = NULL;
double StardustRod = NULL;
double VeilOfWiyu = NULL;
double Omnilex = NULL;
double HolyShield = NULL;

std::mutex CommandMutex;

std::mutex GetItemMutex;

BOOL ShowAllFlag = FALSE;

BOOL AutoUpdateFlag = TRUE;

BOOL ConsoleFlag = FALSE;

UINT pcount = 0;

VOID** ppRootInputObject;

VOID* pThreadVerify;

PDWORD pThreadVerifyIndex;

const char* __stdcall GetItemNameFromID(USHORT ItemID);

UserInputObject* (__thiscall* GetUserInputObjectFromRootInput)(VOID* pRootInputObject);

CommandStruct* (__thiscall* InitCommandStruct)(CommandStruct* pCmdStruct, const char* cmdStr, size_t strSize);

BOOL (__thiscall* Real_IncommingCommand)(VOID* pChatLogObject, CommandStruct* pCmdStruct, UserInputObject* pInputObject);

BasicItemInfo* (_cdecl* GetItemInfo)(USHORT);

BOOL __fastcall My_IncommingCommand(VOID* pChatLogObject, INT, CommandStruct* pCmdStruct, UserInputObject* pInputObject);

VOID __stdcall SendCommand(std::string pCmdStr);

INT (__cdecl* Real_ReadObtainedItemPacket)(INT,ItemObtainedPacket*);

INT __cdecl My_ReadObtainedItemPacket(INT unk, ItemObtainedPacket* packet);

INT (__thiscall* Real_VerifyThreadOgrigin)(VOID* pThis, INT a2, CHAR a3, INT a4);

INT __fastcall My_VerifyThreadOgrigin(VOID* pThis, INT, INT a2, CHAR a3, INT a4);

BOOL __stdcall StringContains(const char* str1, const char* str2);

const char* __stdcall FirstArg(const char* pCmd);

VOID __stdcall InitInstanceRetrievalRoutines(void*);

VOID __stdcall InitThreadSecurity(void*);

VOID __stdcall InitHook();

VOID __stdcall ToggleConsole();

VOID __stdcall ShowCurrentValues();

BOOL __stdcall SetValues(UINT16 ItemID, UINT16 NewSB);

__declspec(naked) VOID* __fastcall FastTls(DWORD dwIndex, VOID* val)
{
	__asm 
	{
		test edx,edx
			je GET
			lea ecx, [ecx*4+0xE10]
		mov fs:[ecx], edx
			xor eax,eax
			ret
GET:
		lea ecx, [ecx*4+0xE10]
		mov eax, fs:[ecx]
		ret
	}
}