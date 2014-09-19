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

#include "stdafx.h"
#include "LaLaunch.h"
#include <iostream>

INT _tmain(INT argc, _TCHAR* argv[])
{
	ULONG o;
	auto PID = GetFFXIVProcId();
	if (PID == 0)
	{
		std::cout << "*** NO FFXIV FOUND\n";
		Sleep(4000);
		return 0;
	}
	if (IsDLLLoaded(PID))
	{
		std::cout << "*** ALREADY LOADED\n";
		Sleep(4000);
		return 0;
	}
	auto DLLPath = ExePath().append("\\FCBondage.dll");
	auto hProc = OpenProcess(PROCESS_ALL_ACCESS,FALSE,PID);
	auto arg1 = VirtualAllocEx(hProc,NULL,MAX_PATH,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
	WriteProcessMemory(hProc,arg1,DLLPath.c_str(),DLLPath.size(),&o);
	auto hThread = CreateRemoteThread(hProc,NULL,NULL,LOADLIBRARY_ROUTINE,arg1,NULL,&o);
	WaitForSingleObject(hThread,INFINITE);
	CloseHandle(hThread);
	VirtualFreeEx(hProc,arg1,MAX_PATH,MEM_RELEASE);
	CloseHandle(hProc);
	std::cout << "Hook Complete, goodbye!\n";
	Sleep(4000);
	return 0;
}

