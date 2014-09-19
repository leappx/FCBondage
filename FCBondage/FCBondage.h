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