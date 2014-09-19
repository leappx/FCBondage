/**=================================================================================================
 * File: FCBondage.h
 * =================================================================================================
 * \file FCBondage.h
 *
 * \brief Declares the fc bondage class.
 */

#include <string>
#include <mutex>

/**=================================================================================================
 * Def: SetFunctionPointer
 * =================================================================================================
 * \def SetFunctionPointer(f,p) *(PINT)(&f) = p;
 *
 * \brief A macro that defines set function pointer.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param f The void to process.
 * \param p The void to process.
 */

#define SetFunctionPointer(f,p) *(PINT)(&f) = p

/**=================================================================================================
 * Typedef: pUserInputObject
 * =================================================================================================
 * \typedef void* (__thiscall* GetChatLogObject)(void* pUserInputObject)
 *
 * \brief Defines an alias representing the user input object.
 */

typedef void* (__thiscall* GetChatLogObject)(void* pUserInputObject);

/**=================================================================================================
 * Struct: UserInputObject
 * =================================================================================================
 * \struct UserInputObject
 *
 * \brief A user input object.
 *
 * \author DevNull
 * \date 9/19/2014
 */

struct UserInputObject
{
	/**=================================================================================================
	 * Member: CalculateAddressOfChatLog
	 * =================================================================================================
	 * \brief The calculate address of chat log.
	 */

	GetChatLogObject* CalculateAddressOfChatLog;
};

#pragma pack(push, 1)

/**=================================================================================================
 * Struct: ItemObtainedPacket
 * =================================================================================================
 * \struct ItemObtainedPacket
 *
 * \brief An item obtained packet.
 *
 * \author DevNull
 * \date 9/19/2014
 */

struct ItemObtainedPacket
{
	__int64 : 64;
	__int64 : 64;

	/**=================================================================================================
	 * Property: ItemID
	 * =================================================================================================
	 * \property unsigned __int16 ItemID
	 *
	 * \brief Gets the identifier of the item.
	 *
	 * \return The identifier of the item.
	 */

	unsigned __int16 ItemID;
	__int64 : 64;
	__int64 : 64;
	__int16 : 16;

	/**=================================================================================================
	 * Property: SpiritBind
	 * =================================================================================================
	 * \property unsigned __int16 SpiritBind
	 *
	 * \brief Gets the spirit bind.
	 *
	 * \return The spirit bind.
	 */

	unsigned __int16 SpiritBind;
};

/**=================================================================================================
 * Struct: BasicItemInfo
 * =================================================================================================
 * \struct BasicItemInfo
 *
 * \brief Information about the basic item.
 *
 * \author DevNull
 * \date 9/19/2014
 */

struct BasicItemInfo
{
	__int64 : 64;
	__int32 : 32;

	/**=================================================================================================
	 * Property: OffsetOfName
	 * =================================================================================================
	 * \property unsigned __int16 OffsetOfName
	 *
	 * \brief Gets the name of the offset of.
	 *
	 * \return The name of the offset of.
	 */

	unsigned __int16 OffsetOfName;
};

/**=================================================================================================
 * Struct: CommandStruct
 * =================================================================================================
 * \struct CommandStruct
 *
 * \brief A command structure.
 *
 * \author DevNull
 * \date 9/19/2014
 */

struct CommandStruct
{
	/**=================================================================================================
	 * Member: pCommandString
	 * =================================================================================================
	 * \brief The command string.
	 */

	const char* pCommandString;

	/**=================================================================================================
	 * Property: BufferSize
	 * =================================================================================================
	 * \property unsigned __int32 BufferSize
	 *
	 * \brief Gets the size of the buffer.
	 *
	 * \return The size of the buffer.
	 */

	unsigned __int32 BufferSize;

	/**=================================================================================================
	 * Property: CommandStrlen
	 * =================================================================================================
	 * \property unsigned __int32 CommandStrlen
	 *
	 * \brief Gets the command strlen.
	 *
	 * \return The command strlen.
	 */

	unsigned __int32 CommandStrlen;

	/**=================================================================================================
	 * Property: UnkUint32
	 * =================================================================================================
	 * \property unsigned __int32 UnkUint32
	 *
	 * \brief Gets the unk uint 32.
	 *
	 * \return The unk uint 32.
	 */

	unsigned __int32 UnkUint32;

	/**=================================================================================================
	 * Property: UnkUint16
	 * =================================================================================================
	 * \property unsigned __int16 UnkUint16
	 *
	 * \brief Gets the unk uint 16.
	 *
	 * \return The unk uint 16.
	 */

	unsigned __int16 UnkUint16;

	/**=================================================================================================
	 * Member: StringBuffer[512]
	 * =================================================================================================
	 * \brief The string buffer[ 512].
	 */

	char StringBuffer[512];
};

#pragma pack(pop)

/**=================================================================================================
 * Member: VerifyThreadOgrigin_FuncSig[12]
 * =================================================================================================
 * \brief The verify thread ogrigin function sig[ 12].
 */

UINT8 VerifyThreadOgrigin_FuncSig[12] =
{
	0x5E, 0x5B, 0xC3, 0x55, 0x8B, 0xEC, 0xA1, 0x88, 0x8A, 0x4B, 0x02, 0x56
};

/**=================================================================================================
 * Member: Input_ObjectSig[15]
 * =================================================================================================
 * \brief The input object sig[ 15].
 */

UINT8 Input_ObjectSig[15] =
{
	0x5D, 0xFF, 0xE2, 0x8B, 0x01, 0x8B, 0x50, 0x2C, 0x5D, 0xFF, 0xE2, 0x8B, 0x01, 0x8B, 0x50
};

/**=================================================================================================
 * Member: InitCommandStruct_FuncSig[12]
 * =================================================================================================
 * \brief The initialise command structure function sig[ 12].
 */

UINT8 InitCommandStruct_FuncSig[12] =
{
	0x89, 0x06, 0xC6, 0x00, 0x00, 0x83, 0xFF, 0xFF, 0x75, 0x14, 0x8B, 0xC3
};

/**=================================================================================================
 * Member: IncommingCommand_FuncSig[14]
 * =================================================================================================
 * \brief The incomming command function sig[ 14].
 */

UINT8 IncommingCommand_FuncSig[14] =
{
	0x8D, 0x55, 0xDC, 0x52, 0x56, 0x8B, 0xCF, 0x89, 0x45, 0xEC, 0x89, 0x45, 0xF0, 0x89
};

/**=================================================================================================
 * Member: GetItemInfo_FuncSig[14]
 * =================================================================================================
 * \brief The get item information function sig[ 14].
 */

UINT8 GetItemInfo_FuncSig[14] =
{
	0x55, 0x8B, 0xEC, 0x8B, 0x45, 0x08, 0x3D, 0x40, 0x42, 0x0F, 0x00, 0x72, 0x05, 0x2D
};

/**=================================================================================================
 * Member: ItemPacket_FuncSig[18]
 * =================================================================================================
 * \brief The item packet function sig[ 18].
 */

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

/**=================================================================================================
 * Member: CommandMutex
 * =================================================================================================
 * \brief The command mutex.
 */

std::mutex CommandMutex;

/**=================================================================================================
 * Member: GetItemMutex
 * =================================================================================================
 * \brief The get item mutex.
 */

std::mutex GetItemMutex;

/**=================================================================================================
 * Member: ShowAllFlag
 * =================================================================================================
 * \brief true to show, false to hide all flag.
 */

BOOL ShowAllFlag = FALSE;

/**=================================================================================================
 * Member: AutoUpdateFlag
 * =================================================================================================
 * \brief true to automatically update flag.
 */

BOOL AutoUpdateFlag = TRUE;

/**=================================================================================================
 * Member: ConsoleFlag
 * =================================================================================================
 * \brief true to console flag.
 */

BOOL ConsoleFlag = FALSE;

/**=================================================================================================
 * Member: pcount
 * =================================================================================================
 * \brief The pcount.
 */

UINT pcount = 0;

/**=================================================================================================
 * Member: ppRootInputObject
 * =================================================================================================
 * \brief The root input object.
 */

VOID** ppRootInputObject;

/**=================================================================================================
 * Member: pThreadVerify
 * =================================================================================================
 * \brief The thread verify.
 */

VOID* pThreadVerify;

/**=================================================================================================
 * Member: pThreadVerifyIndex
 * =================================================================================================
 * \brief Zero-based index of the thread verify.
 */

PDWORD pThreadVerifyIndex;

/**=================================================================================================
 * Method: GetItemNameFromID
 * =================================================================================================
 * \fn const char* __stdcall GetItemNameFromID(USHORT ItemID);
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

const char* __stdcall GetItemNameFromID(USHORT ItemID);

/**=================================================================================================
 * Constructor: UserInputObject*
 * =================================================================================================
 * \fn UserInputObject* (__thiscall* GetUserInputObjectFromRootInput)(VOID* pRootInputObject);
 *
 * \brief Constructor.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param [in,out] GetUserInputObjectFromRootInput If non-null, the get user input object from
 *  root input.
 */

UserInputObject* (__thiscall* GetUserInputObjectFromRootInput)(VOID* pRootInputObject);

/**=================================================================================================
 * Constructor: CommandStruct*
 * =================================================================================================
 * \fn CommandStruct* (__thiscall* InitCommandStruct)(CommandStruct* pCmdStruct, const char* cmdStr, size_t strSize);
 *
 * \brief Constructor.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param [in,out] InitCommandStruct If non-null, the initialise command structure.
 */

CommandStruct* (__thiscall* InitCommandStruct)(CommandStruct* pCmdStruct, const char* cmdStr, size_t strSize);

/**=================================================================================================
 * Constructor: BOOL
 * =================================================================================================
 * \fn BOOL (__thiscall* Real_IncommingCommand)(VOID* pChatLogObject, CommandStruct* pCmdStruct, UserInputObject* pInputObject);
 *
 * \brief Constructor.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param [in,out] Real_IncommingCommand If non-null, the real incomming command.
 */

BOOL (__thiscall* Real_IncommingCommand)(VOID* pChatLogObject, CommandStruct* pCmdStruct, UserInputObject* pInputObject);

/**=================================================================================================
 * Constructor: BasicItemInfo*
 * =================================================================================================
 * \fn BasicItemInfo* (_cdecl* GetItemInfo)(USHORT);
 *
 * \brief Constructor.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param [in,out] GetItemInfo If non-null, information describing the get item.
 */

BasicItemInfo* (_cdecl* GetItemInfo)(USHORT);

BOOL __fastcall My_IncommingCommand(VOID* pChatLogObject, INT, CommandStruct* pCmdStruct, UserInputObject* pInputObject);

VOID __stdcall SendCommand(std::string pCmdStr);

/**=================================================================================================
 * Constructor: INT
 * =================================================================================================
 * \fn INT (__cdecl* Real_ReadObtainedItemPacket)(INT,ItemObtainedPacket*);
 *
 * \brief Constructor.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param [in,out] Real_ReadObtainedItemPacket If non-null, message describing the real read
 *  obtained item.
 */

INT (__cdecl* Real_ReadObtainedItemPacket)(INT,ItemObtainedPacket*);

INT __cdecl My_ReadObtainedItemPacket(INT unk, ItemObtainedPacket* packet);

/**=================================================================================================
 * Constructor: INT
 * =================================================================================================
 * \fn INT (__thiscall* Real_VerifyThreadOgrigin)(VOID* pThis, INT a2, CHAR a3, INT a4);
 *
 * \brief Constructor.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param [in,out] Real_VerifyThreadOgrigin If non-null, the real verify thread ogrigin.
 */

INT (__thiscall* Real_VerifyThreadOgrigin)(VOID* pThis, INT a2, CHAR a3, INT a4);

INT __fastcall My_VerifyThreadOrigin(VOID* pThis, INT, INT a2, CHAR a3, INT a4);

BOOL __stdcall StringContains(const char* str1, const char* str2);

const char* __stdcall FirstArg(const char* pCmd);

VOID __stdcall InitInstanceRetrievalRoutines(void*);

VOID __stdcall InitThreadSecurity(void*);

VOID __stdcall InitHook();

VOID __stdcall ToggleConsole();

VOID __stdcall ShowCurrentValues();

VOID __stdcall PrintInfoToChatLog(UINT16 itemID, double SB);

BOOL __stdcall SetValues(UINT16 ItemID, UINT16 NewSB);

/**=================================================================================================
 * Method: FastTls
 * =================================================================================================
 * \fn VOID* __fastcall FastTls(DWORD dwIndex, VOID* val)
 *
 * \brief Fast TLS.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param dwIndex	   The index.
 * \param [in,out] val If non-null, the value.
 *
 * \return null if it fails, else a VOID*.
 */

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