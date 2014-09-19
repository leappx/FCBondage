#ifndef _CARUSO_SCAN_H_
#define _CARUSO_SCAN_H_
#define CONPUCHAR const unsigned char*
#define CONPCHAR const char*
#define FASTCALL __fastcall

INT FASTCALL FindPattern(CONPUCHAR lpPattern, CONPCHAR sigMask, LPMODULEINFO modInfo);

#endif