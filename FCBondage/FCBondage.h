#define SetFunctionPointer(f,p) *(PINT)(&f) = p

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
#pragma pack(pop)

UINT8 GetItemInfo_FuncSig[14] =
{
	0x55, 0x8B, 0xEC, 0x8B, 0x45, 0x08, 0x3D, 0x40, 0x42, 0x0F, 0x00, 0x72, 0x05, 0x2D
};

UINT8 ItemPacket_FuncSig[18] = 
{
	0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x44, 0xA1, 0x68, 0xC0, 0x11, 0x02, 0x33, 0xC5, 0x89, 0x45, 0xFC, 0x53, 0x57
};

UINT pcount = 0;

const char* __stdcall GetItemNameFromID(USHORT ItemID);

BasicItemInfo* (_cdecl* GetItemInfo)(USHORT);

INT (__cdecl* Real_ReadObtainedItemPacket)(INT,ItemObtainedPacket*);

INT __cdecl My_ReadObtainedItemPacket(INT unk, ItemObtainedPacket* packet);

VOID __stdcall InitHook();

VOID __stdcall SpawnConsole();
