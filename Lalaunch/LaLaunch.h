/**=================================================================================================
 * File: LaLaunch.h
 * =================================================================================================
 * \file LaLaunch.h
 *
 * \brief Declares the la launch class.
 */

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <string>

using namespace std;;

/**=================================================================================================
 * Def: LOADLIBRARY_ROUTINE
 * =================================================================================================
 * \def LOADLIBRARY_ROUTINE
 *
 * \brief A macro that defines loadlibrary routine.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param LPTHREAD_START_ROUTINE The lpthread start routine.
 */

#define LOADLIBRARY_ROUTINE (LPTHREAD_START_ROUTINE)&LoadLibraryA

/**=================================================================================================
 * Method: ExePath
 * =================================================================================================
 * \fn string ExePath()
 *
 * \brief Executable path.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \return A string.
 */

string ExePath() 
{
	CHAR buffer[MAX_PATH];
	GetModuleFileNameA( NULL, buffer, MAX_PATH );
	string::size_type pos = string( buffer ).find_last_of( "\\/" );
	return string( buffer ).substr( 0, pos);
}

/**=================================================================================================
 * Method: GetFFXIVProcId
 * =================================================================================================
 * \fn UINT32 GetFFXIVProcId()
 *
 * \brief Gets ffxiv proc identifier.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \return The ffxiv proc identifier.
 */

UINT32 GetFFXIVProcId()
{
	HANDLE hProcessSnap;
	UINT32 pid = 0;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		printf( "CreateToolhelp32Snapshot (of processes)" );
		return( FALSE );
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		printf( "Process32First" ); // show cause of failure
		CloseHandle( hProcessSnap );          // clean the snapshot object
		return( FALSE );
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		if (!wcscmp(pe32.szExeFile,L"ffxiv.exe"))
		{
			pid = pe32.th32ProcessID;
			break;
		}
		
	} while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );
	return pid;
}

/**=================================================================================================
 * Method: IsDLLLoaded
 * =================================================================================================
 * \fn BOOL IsDLLLoaded( DWORD dwPID )
 *
 * \brief Query if 'dwPID' is DLL loaded.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param dwPID The PID.
 *
 * \return true if DLL loaded, false if not.
 */

BOOL IsDLLLoaded( DWORD dwPID )
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;
	BOOL result = FALSE;
	// Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID );
	if( hModuleSnap == INVALID_HANDLE_VALUE )
	{
		printf( "CreateToolhelp32Snapshot (of modules)" );
		return( FALSE );
	}

	// Set the size of the structure before using it.
	me32.dwSize = sizeof( MODULEENTRY32 );

	// Retrieve information about the first module,
	// and exit if unsuccessful
	if( !Module32First( hModuleSnap, &me32 ) )
	{
		printf( "Module32First" );  // show cause of failure
		CloseHandle( hModuleSnap );           // clean the snapshot object
		return FALSE;
	}

	// Now walk the module list of the process
	do
	{
		if (!wcscmp(me32.szModule, L"FCBondage.dll"))
		{
			result = TRUE;
			break;
		}

	} while( Module32Next( hModuleSnap, &me32 ) );

	CloseHandle( hModuleSnap );
	return result;
}