#ifndef _CARUSO_SCAN_H_
#define _CARUSO_SCAN_H_
#include "stdafx.h"
#include <psapi.h>
#pragma comment(lib, "Psapi.lib")
#define CONPUCHAR const unsigned char*
#define CONPCHAR const char*
#define FASTCALL __fastcall

INT FASTCALL FindPattern(CONPUCHAR lpPattern, CONPCHAR sigMask, LPMODULEINFO modInfo)
{
	auto pos = 0;
	auto ModStart = (CONPUCHAR)modInfo->lpBaseOfDll;
	auto ModEnd = &ModStart[modInfo->SizeOfImage];

	while ( 1 )
	{
		if ( (*lpPattern++ == *ModStart++) || (sigMask[pos] == '?') )
			if ( sigMask[++pos] != 0 )
				continue;
			else
				return (INT)&ModStart[-pos];
		ModStart -= pos;
		lpPattern -= ++pos;
		pos = 0;
		if ( ModEnd == ModStart )
			break;
	}

	return 0;
}

#endif