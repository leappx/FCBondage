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


#ifndef _FC_BONDAGE_H_
#define _FC_BONDAGE_H_

#define SetFunctionPointer(f,p) *(PINT)(&f) = p

void __stdcall InitInstanceRetrievalRoutines(MODULEINFO*);

void __stdcall InitThreadSecurity(MODULEINFO*);

void __stdcall InitHook();

/**=================================================================================================
 * Method: FastTls
 * =================================================================================================
 * \fn void* __fastcall FastTls(unsigned __int32 dwIndex, void* val)
 *
 * \brief Fast TLS.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param dwIndex	   The index.
 * \param [in,out] val If non-null, the value, else ignore and get.
 *
 * \return null if setting, else a void* if getting.
 */

__declspec(naked) void* __fastcall FastTls(unsigned __int32 dwIndex, void* val)
{
	__asm 
	{
		lea ecx, [ecx*4+0xE10]
		test edx,edx
		jne GET
		mov eax, fs:[ecx]
		ret
GET:
		mov fs:[ecx], edx
		xor eax,eax
		ret
	}
}
#endif // !_FC_BONDAGE_H_