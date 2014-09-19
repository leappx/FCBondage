/**=================================================================================================
 * File: SigScan.cpp
 * =================================================================================================
 * \file SigScan.cpp
 *
 * \brief Implements the signature scan class.
 */

#include "stdafx.h"

/**=================================================================================================
 * Method: FindPattern
 * =================================================================================================
 * \fn INT FASTCALL FindPattern(CONPUCHAR lpPattern, CONPCHAR sigMask, LPMODULEINFO modInfo)
 *
 * \brief Scans the given module's memory for the first occurrence of a given pattern.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param lpPattern Pointer to the pattern to scan for.
 * \param sigMask   The pattern mask. This is a scan rule string "x" = valid byte | "?" = wild card
 * \param modInfo   Pointer to the target module info structure.
 *
 * \return The found pattern.
 */

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