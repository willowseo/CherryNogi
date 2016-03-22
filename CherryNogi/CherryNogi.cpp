#include "stdafx.h"
#include "CherryNogi.h"
#include "PatternSearch.h"
#include "Pattern.h"
#include "tchar.h"
#include "direct.h"
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <windows.h>
#include <Tlhelp32.h>
#include <fstream>
#include <iostream>
#pragma pack(1)

HINSTANCE 	hLThisInstance = 0;
HINSTANCE 	hOriginalLibrary = 0;
HMODULE CherryDinput;
HMODULE CherryPake;
HANDLE hConsoleW, hConsoleR;
HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

FARPROC 	p[12] = { 0 };
TCHAR* version = L"CherryNogi ver. 3.65 (by 블스체리)\n\n";

int debugMode = 0, CherryC = 0;
int ZoomC = 0, ZoomC2 = 0, ZoomC3 = 0, PartyC = 0, HouseC = 0, HouseC2 = 0, ClockC = 0, AllNamesInVisionC = 0, RWantedC = 0;
int MoveToSameChannelC = 0, PutFxAnyItemC = 0, EgoC = 0, DisplayRobberyC = 0, MimicC = 0, NameColoringC = 0;
int Power1C = 0, Power2C = 0, Power3C = 0, Power4C = 0, Power5C = 0, ScrapC = 0, NangC = 0, AvonC = 0;
int ZoomSC = 0, ZoomS200C = 0, PetLegC = 0, CookC = 0, NightTC = 0, PerC = 0, StreetLampC = 0, RightC = 0;
int mapColorC = 0, PriceC = 0, BlackC = 0, MiniNogiC = 0, Nang2C = 0;
int ZeroC = 0, Zero50C = 0, Zero500C = 0, RecC = 0, TitleC = 0, FramesC = 0;
int Nosky1C = 0, Nosky2C = 0, LoginC = 0, Login2C = 0, TalkC = 0, NopeC = 0;
int offsetLogin;
int floche1C = 0, floche2C = 0, floche3C = 0, floche4C = 0, floche5C = 0, floche6C = 0, floche7C = 0, floche8C = 0, luncherC = 0, aaaC = 0;
int iniZoom, iniParty, iniAdv, iniPower, iniZoomSpeed, iniLag, iniAllNames, iniNameColoring, iniCookingNoise;
int iniNighttime, iniClock, iniAnyItem, iniEgo, iniRobbery, iniPercentage, iniStreetLamp, iniRightClick;
int iniMiniNogi, iniScrapBook, iniBlackList, iniitemPrice, iniMinimapColor, iniQuietPet, iniMimic;
int iniRWanted, iniChannel, iniLflying, iniAvon, iniSleepTime, iniZero, iniRec, iniTitle, iniFrames, iniNosky, iniLogin, iniTalk;
int iniPatch, CherryE;

enum Color { black = 0, blue, green, cyan, red, purple, yellow, grey, dgrey, lblue, lgreen, lcyan, lred, lpurple, lyellow, white };

DWORD DisableClosePartyAddr, PartyToAdv1Addr, PartyToAdvMainAddr, ZoomAddr, Zoom_EmptyAddr, Zoom_10000Addr, Zoom_5000Addr, ClockAddr;
DWORD MoveToSameChannelAddr, AllNamesInVisionAddr, PutFxAnyItemAddr, EgoAddr, DisplayRobberyAddr, MimicAddr, NameColoringAddr;
DWORD RWantedAddr, CombatAddr, Power1Addr, Power2Addr, Power3Addr, Power4Addr, Power5Addr, ScrapAddr, NangAddr, AvonAddr;
DWORD ZoomSAddr, ZoomS200Addr, PetLegAddr, CookAddr, NightTAddr, PerAddr, StreetLampAddr, RightAddr, mapColorAddr, PriceAddr;
DWORD BlackAddr, MiniNogiAddr, Nang2Addr;
DWORD floche1Addr, floche11Addr, floche12Addr, floche2Addr, floche3Addr, floche4Addr, floche5Addr, floche6Addr;
DWORD VM_Instance, floche7Addr, floche8Addr, floche13Addr, luncherAddr, aaaAddr;
DWORD ZeroAddr, Zero50Addr, Zero500Addr, RecAddr, TitleAddr, FramesAddr, Nosky1Addr, Nosky2Addr, LoginAddr, Login2Addr, TalkAddr, NopeAddr, CherryNogi_Dsound_Addr;

bool showAddr = 1;

LPVOID MemCpyEx(LPVOID lpDestination, LPVOID lpSource, int nLen);

LPVOID MemCpyEx(LPVOID lpDestination, LPVOID lpSource, int nLen)
{
	DWORD dwOldDestProt = 0;
	DWORD dwOldSrcProt = 0;

	VirtualProtect(lpDestination, nLen, PAGE_EXECUTE_READWRITE, &dwOldDestProt);
	VirtualProtect(lpSource, nLen, PAGE_EXECUTE_READWRITE, &dwOldSrcProt);
	memcpy(lpDestination, lpSource, nLen);
	VirtualProtect(lpDestination, nLen, dwOldDestProt, NULL);
	VirtualProtect(lpSource, nLen, dwOldSrcProt, NULL);

	return lpDestination;
}


void ConsoleOutput(TCHAR* lpszMessage, Color color = white)
{
	int len = wcslen(lpszMessage);
	DWORD numOfCharWritten;
	SetConsoleTextAttribute(hConsoleW, color);
	WriteConsole(hConsoleW, lpszMessage, len, &numOfCharWritten, NULL);
}


void PatternSearchLogSuccess(TCHAR* address)
{
	if (iniPatch == 10011)
	{
		if (!showAddr)
			address = L"Ok";
		ConsoleOutput(L"	[");
		ConsoleOutput(address);
		ConsoleOutput(L"]\n");
	}
	if (iniPatch != 10011)
	{
		if (!showAddr)
			address = L"Ok";
		ConsoleOutput(L"	[");
		ConsoleOutput(L"검색 성공");
		ConsoleOutput(L"]\n");
	}
}


void PatternSearchLogFailure()
{
	ConsoleOutput(L"	[");
	ConsoleOutput(L"실패, 업데이트 필요", lred);
	ConsoleOutput(L"]\n");
}


TCHAR* DwordToLptstr(DWORD address)
{
	static TCHAR lpszAddress[11] = { 0 };
	wsprintf(lpszAddress, L"0x%08x", address);
	return lpszAddress;
}


void WaitUserResponse()
{
	char buffer[128];
	DWORD NumberOfBytesRead = 0;
	while (1)
	{
		ReadConsoleA(hConsoleR, buffer, 128, &NumberOfBytesRead, NULL);
		if (NumberOfBytesRead >= 2)
			break;
	}
}


void ErrorExit(TCHAR* message)
{
	ConsoleOutput(message, lred);
	ConsoleOutput(L"Press enter to close this window.");
	WaitUserResponse();
	FreeConsole();
}


bool CheckPatternSearchSuccess(DWORD address)
{
	if (address != NULL)
	{
		PatternSearchLogSuccess(DwordToLptstr(address));
		return 1;
	}
	else
	{
		PatternSearchLogFailure();
		return 0;
	}
}



DWORD GetVMInstance()
{
	return *(DWORD*)VM_Instance;
}


__declspec(naked) void GetReceiverId()
{
	__asm
	{
		mov     ecx, [ecx + 8]
		test    ecx, ecx
			jnz     LABLE1
			xor     eax, eax
			xor     edx, edx
			retn
			LABLE1 :
		mov     eax, [ecx + 10h]
			mov     edx, [ecx + 14h]
			retn
	}

}


__declspec(naked) void CherryPakeJmp()
{
	__asm
	{
		push ebp
		mov ebp, esp
			push 0XFF
			jmp floche11Addr
	}

}


__declspec(naked) void CherryLogin()
{
	__asm
	{
		push ebp
		mov ebp, esp
			push 0XFF
			jmp LoginAddr
	}

}


bool PatchClient()
{
	try {
		ConsoleOutput(L"\n\n== 패치 중 =========================\n\n");

		if (floche1C == 1 && floche2C == 1 && floche3C == 1 && floche4C == 1 && floche5C == 1)
		{
			int offset = floche12Addr - floche1Addr - 5;
			int offset2 = floche13Addr - floche8Addr - 5;

			floche12Addr = (DWORD)CherryPake + 0x121c0;
			floche13Addr = (DWORD)CherryPake + 0x12520;
			VM_Instance = *(DWORD*)(floche6Addr + 21);

			*(BYTE*)floche1Addr = 0xE9;
			*(BYTE*)floche8Addr = 0xE9;

			*(DWORD*)(floche1Addr + 1) = offset;
			*(DWORD*)(floche8Addr + 1) = offset2;

			*(DWORD*)((DWORD)CherryPake + 0x2FFF0) = (floche3Addr + 6);
			*(DWORD*)((DWORD)CherryPake + 0x2FFF4) = floche4Addr;
			*(DWORD*)((DWORD)CherryPake + 0x2FFE8) = floche2Addr;
			*(DWORD*)((DWORD)CherryPake + 0x2FFE4) = floche5Addr;
			*(DWORD*)((DWORD)CherryPake + 0x2FFFC) = (DWORD)CherryPakeJmp;
			*(DWORD*)((DWORD)CherryPake + 0x2FFEC) = (DWORD)GetReceiverId;
			*(DWORD*)((DWORD)CherryPake + 0x2FFDC) = (DWORD)GetVMInstance;
			*(DWORD*)((DWORD)CherryPake + 0x2FFE0) = floche7Addr;
			*(DWORD*)((DWORD)CherryPake + 0x2FFF8) = floche8Addr + 9;
		}

		if (PartyC == 1)
		{
			*(BYTE*)(DisableClosePartyAddr + 13) = 0xEB;

			ConsoleOutput(L"\n-파티창 유지시간 무제한    [패치 성공]\n");
		}


		if (HouseC == 1 && HouseC2 == 1)
		{
			int offset = PartyToAdv1Addr - PartyToAdvMainAddr - 5;
			*(BYTE*)PartyToAdvMainAddr = 0xE8;
			*(DWORD*)(PartyToAdvMainAddr + 1) = offset;

			ConsoleOutput(L"-하우징 게시판 스왑    [패치 성공]\n");
		}


		if (ZoomC == 1 && ZoomC2 == 1)
		{
			*(BYTE*)(ZoomAddr + 24) = 0x05;
			*(DWORD*)(ZoomAddr + 25) = Zoom_10000Addr;

			ConsoleOutput(L"-시야 확장(10000)    [패치 성공]\n");
		}


		if (ZoomC == 1 && ZoomC2 == 2)
		{
			*(BYTE*)(ZoomAddr + 24) = 0x05;
			*(DWORD*)(ZoomAddr + 25) = Zoom_5000Addr;

			ConsoleOutput(L"-시야 확장(5000)    [패치 성공]\n");
		}


		if (ClockC == 1)
		{
			*(BYTE*)ClockAddr = 0x90;
			*(BYTE*)(ClockAddr + 1) = 0x90;
			*(BYTE*)(ClockAddr + 2) = 0x90;
			*(BYTE*)(ClockAddr + 3) = 0x90;
			*(BYTE*)(ClockAddr + 4) = 0x90;
			*(BYTE*)(ClockAddr + 5) = 0x90;
			*(BYTE*)(ClockAddr + 6) = 0x90;
			*(BYTE*)(ClockAddr + 7) = 0x90;
			*(BYTE*)(ClockAddr + 8) = 0x90;
			*(BYTE*)(ClockAddr + 9) = 0x90;
			*(BYTE*)(ClockAddr + 10) = 0x90;
			*(BYTE*)(ClockAddr + 11) = 0x90;
			*(BYTE*)(ClockAddr + 12) = 0x90;
			*(BYTE*)(ClockAddr + 13) = 0x90;

			ConsoleOutput(L"-분 단위 시계    [패치 성공]\n");
		}


		if (AllNamesInVisionC == 1)
		{
			*(BYTE*)(AllNamesInVisionAddr + 4) = 0x60;
			*(BYTE*)(AllNamesInVisionAddr + 5) = 0x6A;
			*(BYTE*)(AllNamesInVisionAddr + 6) = 0x46;

			ConsoleOutput(L"-시야내 모든 이름 표시    [패치 성공]\n");
		}


		if (MoveToSameChannelC == 1)
		{
			*(BYTE*)MoveToSameChannelAddr = 0x90;
			*(BYTE*)(MoveToSameChannelAddr + 1) = 0xE9;

			ConsoleOutput(L"-동일 채널 재접속    [패치 성공]\n");
		}


		if (PutFxAnyItemC == 1)
		{
			*(BYTE*)(PutFxAnyItemAddr + 2) = 0x00;

			ConsoleOutput(L"-단축키에 무엇이든 올리기    [패치 성공]\n");
		}


		if (EgoC == 1)
		{
			*(BYTE*)EgoAddr = 0x90;
			*(BYTE*)(EgoAddr + 1) = 0xE9;

			ConsoleOutput(L"-미장착 정령대화    [패치 성공]\n");
		}


		if (DisplayRobberyC == 1)
		{
			*(BYTE*)(DisplayRobberyAddr + 2) = 0x00;
			*(BYTE*)(DisplayRobberyAddr + 3) = 0x00;
			*(BYTE*)(DisplayRobberyAddr + 4) = 0x00;
			*(BYTE*)(DisplayRobberyAddr + 5) = 0x00;

			ConsoleOutput(L"-미니맵에 약탈단 표시    [패치 성공]\n");
		}


		if (MimicC == 1)
		{
			*(BYTE*)(MimicAddr + 4) = 0x0F;

			ConsoleOutput(L"-미믹 타겟팅    [패치 성공]\n");
		}


		if (NameColoringC == 1)
		{
			*(BYTE*)(NameColoringAddr + 4) = 0xA3;
			*(BYTE*)(NameColoringAddr + 5) = 0xF0;
			*(BYTE*)(NameColoringAddr + 6) = 0xF0;
			*(BYTE*)(NameColoringAddr + 11) = 0x28;
			*(BYTE*)(NameColoringAddr + 12) = 0x28;
			*(BYTE*)(NameColoringAddr + 13) = 0x28;

			ConsoleOutput(L"-이름표시 색 변경    [패치 성공]\n");
		}



		if (RWantedC == 1)
		{
			*(BYTE*)(RWantedAddr + 1) = 0xC2;
			*(BYTE*)(RWantedAddr + 2) = 0xAA;

			ConsoleOutput(L"-콤보카드 활성 버튼 -> 추적게시판    [패치 성공]\n");
		}



		if (NangC == 1)
		{
			*(BYTE*)(NangAddr + 1) = 0xA8;
			*(BYTE*)(NangAddr + 2) = 0x65;

			ConsoleOutput(L"-정령대화 -> 낭만비행    [패치 성공]\n");
		}




		if (Nang2C == 1)
		{
			*(BYTE*)(Nang2Addr + 1) = 0xA8;
			*(BYTE*)(Nang2Addr + 2) = 0x65;

			ConsoleOutput(L"-콤보카드 비활성 버튼 -> 낭만비행    [패치 성공]\n");
		}


		if (Power1C == 1 && Power2C == 1 && Power3C == 1 && Power4C == 1 && Power5C == 1)
		{
			*(BYTE*)(Power1Addr + 17) = 0x00;
			*(BYTE*)(Power1Addr + 18) = 0x00;
			*(BYTE*)(Power1Addr + 32) = 0x00;
			*(BYTE*)(Power1Addr + 33) = 0x00;
			*(BYTE*)(Power1Addr + 67) = 0x00;
			*(BYTE*)(Power1Addr + 68) = 0x00;
			*(BYTE*)(Power1Addr + 95) = 0x8B;
			*(BYTE*)(Power1Addr + 96) = 0xCE;
			*(BYTE*)(Power1Addr + 97) = 0x8B;
			*(BYTE*)(Power1Addr + 98) = 0x01;
			*(BYTE*)(Power1Addr + 99) = 0xFF;
			*(BYTE*)(Power1Addr + 100) = 0x50;
			*(BYTE*)(Power1Addr + 101) = 0x54;
			*(BYTE*)(Power1Addr + 102) = 0x8B;
			*(BYTE*)(Power1Addr + 103) = 0xC8;

			*(BYTE*)(Power1Addr + 104) = 0xE8;
			int offset = 0xFFFFFFFF - ((Power1Addr + 104) - Power2Addr + 4);
			*(DWORD*)(Power1Addr + 105) = offset;

			*(BYTE*)(Power1Addr + 109) = 0x83;
			*(BYTE*)(Power1Addr + 110) = 0xEC;
			*(BYTE*)(Power1Addr + 111) = 0x08;
			*(BYTE*)(Power1Addr + 112) = 0xDD;
			*(BYTE*)(Power1Addr + 113) = 0x1C;
			*(BYTE*)(Power1Addr + 114) = 0x24;
			*(BYTE*)(Power1Addr + 115) = 0x56;
			*(BYTE*)(Power1Addr + 116) = 0x8B;
			*(BYTE*)(Power1Addr + 117) = 0xCF;

			*(BYTE*)(Power1Addr + 118) = 0xE8;
			int offset2 = 0xFFFFFFFF - ((Power1Addr + 118) - Power3Addr + 4);
			*(DWORD*)(Power1Addr + 119) = offset2;

			*(BYTE*)(Power1Addr + 123) = 0xB9;
			*(BYTE*)(Power1Addr + 124) = 0x40;
			*(BYTE*)(Power1Addr + 125) = 0x00;
			*(BYTE*)(Power1Addr + 126) = 0x00;
			*(BYTE*)(Power1Addr + 127) = 0x00;
			*(BYTE*)(Power1Addr + 128) = 0xF7;
			*(BYTE*)(Power1Addr + 129) = 0xE1;
			*(BYTE*)(Power1Addr + 130) = 0x8D;
			*(BYTE*)(Power1Addr + 131) = 0x80;

			*(DWORD*)(Power1Addr + 132) = NopeAddr;

			*(BYTE*)(Power1Addr + 136) = 0x50;
			*(BYTE*)(Power1Addr + 137) = 0x8D;
			*(BYTE*)(Power1Addr + 138) = 0x4D;
			*(BYTE*)(Power1Addr + 139) = 0xF0;
			*(BYTE*)(Power1Addr + 140) = 0x51;

			*(BYTE*)(Power1Addr + 141) = 0xE8;
			int offset3 = 0xFFFFFFFF - ((Power1Addr + 141) - Power4Addr + 4);
			*(DWORD*)(Power1Addr + 142) = offset3;

			*(BYTE*)(Power1Addr + 146) = 0x83;
			*(BYTE*)(Power1Addr + 147) = 0xC4;
			*(BYTE*)(Power1Addr + 148) = 0x10;
			*(BYTE*)(Power1Addr + 149) = 0xEB;
			*(BYTE*)(Power1Addr + 150) = 0x06;

			*(BYTE*)(NopeAddr + 0) = 0x25;
			*(BYTE*)(NopeAddr + 1) = 0x00;
			*(BYTE*)(NopeAddr + 2) = 0x2E;
			*(BYTE*)(NopeAddr + 3) = 0x00;
			*(BYTE*)(NopeAddr + 4) = 0x32;
			*(BYTE*)(NopeAddr + 5) = 0x00;
			*(BYTE*)(NopeAddr + 6) = 0x66;
			*(BYTE*)(NopeAddr + 7) = 0x00;
			*(BYTE*)(NopeAddr + 8) = 0x20;
			*(BYTE*)(NopeAddr + 9) = 0x00;
			*(BYTE*)(NopeAddr + 10) = 0x00;
			*(BYTE*)(NopeAddr + 11) = 0x00;
			*(BYTE*)(NopeAddr + 12) = 0x00;
			*(BYTE*)(NopeAddr + 13) = 0x00;
			*(BYTE*)(NopeAddr + 14) = 0x00;
			*(BYTE*)(NopeAddr + 15) = 0x00;
			*(BYTE*)(NopeAddr + 16) = 0x00;
			*(BYTE*)(NopeAddr + 17) = 0x00;
			*(BYTE*)(NopeAddr + 18) = 0x00;
			*(BYTE*)(NopeAddr + 19) = 0x00;
			*(BYTE*)(NopeAddr + 20) = 0x00;
			*(BYTE*)(NopeAddr + 21) = 0x00;
			*(BYTE*)(NopeAddr + 22) = 0x00;
			*(BYTE*)(NopeAddr + 23) = 0x00;
			*(BYTE*)(NopeAddr + 24) = 0x00;
			*(BYTE*)(NopeAddr + 25) = 0x00;
			*(BYTE*)(NopeAddr + 26) = 0x00;
			*(BYTE*)(NopeAddr + 27) = 0x00;
			*(BYTE*)(NopeAddr + 28) = 0x00;
			*(BYTE*)(NopeAddr + 29) = 0x00;
			*(BYTE*)(NopeAddr + 30) = 0x00;
			*(BYTE*)(NopeAddr + 31) = 0x00;
			*(BYTE*)(NopeAddr + 32) = 0x00;
			*(BYTE*)(NopeAddr + 33) = 0x00;
			*(BYTE*)(NopeAddr + 34) = 0x00;
			*(BYTE*)(NopeAddr + 35) = 0x00;
			*(BYTE*)(NopeAddr + 36) = 0x00;
			*(BYTE*)(NopeAddr + 37) = 0x00;
			*(BYTE*)(NopeAddr + 38) = 0x00;
			*(BYTE*)(NopeAddr + 39) = 0x00;
			*(BYTE*)(NopeAddr + 40) = 0x00;
			*(BYTE*)(NopeAddr + 41) = 0x00;
			*(BYTE*)(NopeAddr + 42) = 0x00;
			*(BYTE*)(NopeAddr + 43) = 0x00;
			*(BYTE*)(NopeAddr + 44) = 0x00;
			*(BYTE*)(NopeAddr + 45) = 0x00;
			*(BYTE*)(NopeAddr + 46) = 0x00;
			*(BYTE*)(NopeAddr + 47) = 0x00;
			*(BYTE*)(NopeAddr + 48) = 0x00;
			*(BYTE*)(NopeAddr + 49) = 0x00;
			*(BYTE*)(NopeAddr + 50) = 0x00;
			*(BYTE*)(NopeAddr + 51) = 0x00;
			*(BYTE*)(NopeAddr + 52) = 0x00;
			*(BYTE*)(NopeAddr + 53) = 0x00;
			*(BYTE*)(NopeAddr + 54) = 0x00;
			*(BYTE*)(NopeAddr + 55) = 0x00;
			*(BYTE*)(NopeAddr + 56) = 0x00;
			*(BYTE*)(NopeAddr + 57) = 0x00;
			*(BYTE*)(NopeAddr + 58) = 0x00;
			*(BYTE*)(NopeAddr + 59) = 0x00;
			*(BYTE*)(NopeAddr + 60) = 0x00;
			*(BYTE*)(NopeAddr + 61) = 0x00;
			*(BYTE*)(NopeAddr + 62) = 0x00;
			*(BYTE*)(NopeAddr + 63) = 0x00;													//%...2.f. .<.m.i.n.i.>.W.E.A.K.E.S.T.<./.m.i.n.i.>
			*(BYTE*)(NopeAddr + 64) = 0x25;
			*(BYTE*)(NopeAddr + 65) = 0x00;
			*(BYTE*)(NopeAddr + 66) = 0x2E;
			*(BYTE*)(NopeAddr + 67) = 0x00;
			*(BYTE*)(NopeAddr + 68) = 0x32;
			*(BYTE*)(NopeAddr + 69) = 0x00;
			*(BYTE*)(NopeAddr + 70) = 0x66;
			*(BYTE*)(NopeAddr + 71) = 0x00;
			*(BYTE*)(NopeAddr + 72) = 0x20;
			*(BYTE*)(NopeAddr + 73) = 0x00;
			*(BYTE*)(NopeAddr + 74) = 0x3C;
			*(BYTE*)(NopeAddr + 75) = 0x00;
			*(BYTE*)(NopeAddr + 76) = 0x6D;
			*(BYTE*)(NopeAddr + 77) = 0x00;
			*(BYTE*)(NopeAddr + 78) = 0x69;
			*(BYTE*)(NopeAddr + 79) = 0x00;
			*(BYTE*)(NopeAddr + 80) = 0x6E;
			*(BYTE*)(NopeAddr + 81) = 0x00;
			*(BYTE*)(NopeAddr + 82) = 0x69;
			*(BYTE*)(NopeAddr + 83) = 0x00;
			*(BYTE*)(NopeAddr + 84) = 0x3E;
			*(BYTE*)(NopeAddr + 85) = 0x00;
			*(BYTE*)(NopeAddr + 86) = 0x57;
			*(BYTE*)(NopeAddr + 87) = 0x00;
			*(BYTE*)(NopeAddr + 88) = 0x45;
			*(BYTE*)(NopeAddr + 89) = 0x00;
			*(BYTE*)(NopeAddr + 90) = 0x41;
			*(BYTE*)(NopeAddr + 91) = 0x00;
			*(BYTE*)(NopeAddr + 92) = 0x4B;
			*(BYTE*)(NopeAddr + 93) = 0x00;
			*(BYTE*)(NopeAddr + 94) = 0x45;
			*(BYTE*)(NopeAddr + 95) = 0x00;
			*(BYTE*)(NopeAddr + 96) = 0x53;
			*(BYTE*)(NopeAddr + 97) = 0x00;
			*(BYTE*)(NopeAddr + 98) = 0x54;
			*(BYTE*)(NopeAddr + 99) = 0x00;
			*(BYTE*)(NopeAddr + 100) = 0x3C;
			*(BYTE*)(NopeAddr + 101) = 0x00;
			*(BYTE*)(NopeAddr + 102) = 0x2F;
			*(BYTE*)(NopeAddr + 103) = 0x00;
			*(BYTE*)(NopeAddr + 104) = 0x6D;
			*(BYTE*)(NopeAddr + 105) = 0x00;
			*(BYTE*)(NopeAddr + 106) = 0x69;
			*(BYTE*)(NopeAddr + 107) = 0x00;
			*(BYTE*)(NopeAddr + 108) = 0x6E;
			*(BYTE*)(NopeAddr + 109) = 0x00;
			*(BYTE*)(NopeAddr + 110) = 0x69;
			*(BYTE*)(NopeAddr + 111) = 0x00;
			*(BYTE*)(NopeAddr + 112) = 0x3E;
			*(BYTE*)(NopeAddr + 113) = 0x00;
			*(BYTE*)(NopeAddr + 114) = 0x20;
			*(BYTE*)(NopeAddr + 115) = 0x00;
			*(BYTE*)(NopeAddr + 116) = 0x00;
			*(BYTE*)(NopeAddr + 117) = 0x00;
			*(BYTE*)(NopeAddr + 118) = 0x00;
			*(BYTE*)(NopeAddr + 119) = 0x00;
			*(BYTE*)(NopeAddr + 120) = 0x00;
			*(BYTE*)(NopeAddr + 121) = 0x00;
			*(BYTE*)(NopeAddr + 122) = 0x00;
			*(BYTE*)(NopeAddr + 123) = 0x00;
			*(BYTE*)(NopeAddr + 124) = 0x00;
			*(BYTE*)(NopeAddr + 125) = 0x00;
			*(BYTE*)(NopeAddr + 126) = 0x00;
			*(BYTE*)(NopeAddr + 127) = 0x00;											//%...2.f. .<.m.i.n.i.>.W.E.A.K.<./.m.i.n.i.>
			*(BYTE*)(NopeAddr + 128) = 0x25;
			*(BYTE*)(NopeAddr + 129) = 0x00;
			*(BYTE*)(NopeAddr + 130) = 0x2E;
			*(BYTE*)(NopeAddr + 131) = 0x00;
			*(BYTE*)(NopeAddr + 132) = 0x32;
			*(BYTE*)(NopeAddr + 133) = 0x00;
			*(BYTE*)(NopeAddr + 134) = 0x66;
			*(BYTE*)(NopeAddr + 135) = 0x00;
			*(BYTE*)(NopeAddr + 136) = 0x20;
			*(BYTE*)(NopeAddr + 137) = 0x00;
			*(BYTE*)(NopeAddr + 138) = 0x3C;
			*(BYTE*)(NopeAddr + 139) = 0x00;
			*(BYTE*)(NopeAddr + 140) = 0x6D;
			*(BYTE*)(NopeAddr + 141) = 0x00;
			*(BYTE*)(NopeAddr + 142) = 0x69;
			*(BYTE*)(NopeAddr + 143) = 0x00;
			*(BYTE*)(NopeAddr + 144) = 0x6E;
			*(BYTE*)(NopeAddr + 145) = 0x00;
			*(BYTE*)(NopeAddr + 146) = 0x69;
			*(BYTE*)(NopeAddr + 147) = 0x00;
			*(BYTE*)(NopeAddr + 148) = 0x3E;
			*(BYTE*)(NopeAddr + 149) = 0x00;
			*(BYTE*)(NopeAddr + 150) = 0x57;
			*(BYTE*)(NopeAddr + 151) = 0x00;
			*(BYTE*)(NopeAddr + 152) = 0x45;
			*(BYTE*)(NopeAddr + 153) = 0x00;
			*(BYTE*)(NopeAddr + 154) = 0x41;
			*(BYTE*)(NopeAddr + 155) = 0x00;
			*(BYTE*)(NopeAddr + 156) = 0x4B;
			*(BYTE*)(NopeAddr + 157) = 0x00;
			*(BYTE*)(NopeAddr + 158) = 0x3C;
			*(BYTE*)(NopeAddr + 159) = 0x00;
			*(BYTE*)(NopeAddr + 160) = 0x2F;
			*(BYTE*)(NopeAddr + 161) = 0x00;
			*(BYTE*)(NopeAddr + 162) = 0x6D;
			*(BYTE*)(NopeAddr + 163) = 0x00;
			*(BYTE*)(NopeAddr + 164) = 0x69;
			*(BYTE*)(NopeAddr + 165) = 0x00;
			*(BYTE*)(NopeAddr + 166) = 0x6E;
			*(BYTE*)(NopeAddr + 167) = 0x00;
			*(BYTE*)(NopeAddr + 168) = 0x69;
			*(BYTE*)(NopeAddr + 169) = 0x00;
			*(BYTE*)(NopeAddr + 170) = 0x3E;
			*(BYTE*)(NopeAddr + 171) = 0x00;
			*(BYTE*)(NopeAddr + 172) = 0x20;
			*(BYTE*)(NopeAddr + 173) = 0x00;
			*(BYTE*)(NopeAddr + 174) = 0x00;
			*(BYTE*)(NopeAddr + 175) = 0x00;
			*(BYTE*)(NopeAddr + 176) = 0x00;
			*(BYTE*)(NopeAddr + 177) = 0x00;
			*(BYTE*)(NopeAddr + 178) = 0x00;
			*(BYTE*)(NopeAddr + 179) = 0x00;
			*(BYTE*)(NopeAddr + 180) = 0x00;
			*(BYTE*)(NopeAddr + 181) = 0x00;
			*(BYTE*)(NopeAddr + 182) = 0x00;
			*(BYTE*)(NopeAddr + 183) = 0x00;
			*(BYTE*)(NopeAddr + 184) = 0x00;
			*(BYTE*)(NopeAddr + 185) = 0x00;
			*(BYTE*)(NopeAddr + 186) = 0x00;
			*(BYTE*)(NopeAddr + 187) = 0x00;
			*(BYTE*)(NopeAddr + 188) = 0x00;
			*(BYTE*)(NopeAddr + 189) = 0x00;
			*(BYTE*)(NopeAddr + 190) = 0x00;
			*(BYTE*)(NopeAddr + 191) = 0x00;										//%...2.f. .......
			*(BYTE*)(NopeAddr + 192) = 0x25;
			*(BYTE*)(NopeAddr + 193) = 0x00;
			*(BYTE*)(NopeAddr + 194) = 0x2E;
			*(BYTE*)(NopeAddr + 195) = 0x00;
			*(BYTE*)(NopeAddr + 196) = 0x32;
			*(BYTE*)(NopeAddr + 197) = 0x00;
			*(BYTE*)(NopeAddr + 198) = 0x66;
			*(BYTE*)(NopeAddr + 199) = 0x00;
			*(BYTE*)(NopeAddr + 200) = 0x20;
			*(BYTE*)(NopeAddr + 201) = 0x00;
			*(BYTE*)(NopeAddr + 202) = 0x00;
			*(BYTE*)(NopeAddr + 203) = 0x00;
			*(BYTE*)(NopeAddr + 204) = 0x00;
			*(BYTE*)(NopeAddr + 205) = 0x00;
			*(BYTE*)(NopeAddr + 206) = 0x00;
			*(BYTE*)(NopeAddr + 207) = 0x00;
			*(BYTE*)(NopeAddr + 208) = 0x00;
			*(BYTE*)(NopeAddr + 209) = 0x00;
			*(BYTE*)(NopeAddr + 210) = 0x00;
			*(BYTE*)(NopeAddr + 211) = 0x00;
			*(BYTE*)(NopeAddr + 212) = 0x00;
			*(BYTE*)(NopeAddr + 213) = 0x00;
			*(BYTE*)(NopeAddr + 214) = 0x00;
			*(BYTE*)(NopeAddr + 215) = 0x00;
			*(BYTE*)(NopeAddr + 216) = 0x00;
			*(BYTE*)(NopeAddr + 217) = 0x00;
			*(BYTE*)(NopeAddr + 218) = 0x00;
			*(BYTE*)(NopeAddr + 219) = 0x00;
			*(BYTE*)(NopeAddr + 220) = 0x00;
			*(BYTE*)(NopeAddr + 221) = 0x00;
			*(BYTE*)(NopeAddr + 222) = 0x00;
			*(BYTE*)(NopeAddr + 223) = 0x00;
			*(BYTE*)(NopeAddr + 224) = 0x00;
			*(BYTE*)(NopeAddr + 225) = 0x00;
			*(BYTE*)(NopeAddr + 226) = 0x00;
			*(BYTE*)(NopeAddr + 227) = 0x00;
			*(BYTE*)(NopeAddr + 228) = 0x00;
			*(BYTE*)(NopeAddr + 229) = 0x00;
			*(BYTE*)(NopeAddr + 230) = 0x00;
			*(BYTE*)(NopeAddr + 231) = 0x00;
			*(BYTE*)(NopeAddr + 232) = 0x00;
			*(BYTE*)(NopeAddr + 233) = 0x00;
			*(BYTE*)(NopeAddr + 234) = 0x00;
			*(BYTE*)(NopeAddr + 235) = 0x00;
			*(BYTE*)(NopeAddr + 236) = 0x00;
			*(BYTE*)(NopeAddr + 237) = 0x00;
			*(BYTE*)(NopeAddr + 238) = 0x00;
			*(BYTE*)(NopeAddr + 239) = 0x00;
			*(BYTE*)(NopeAddr + 240) = 0x00;
			*(BYTE*)(NopeAddr + 241) = 0x00;
			*(BYTE*)(NopeAddr + 242) = 0x00;
			*(BYTE*)(NopeAddr + 243) = 0x00;
			*(BYTE*)(NopeAddr + 244) = 0x00;
			*(BYTE*)(NopeAddr + 245) = 0x00;
			*(BYTE*)(NopeAddr + 246) = 0x00;
			*(BYTE*)(NopeAddr + 247) = 0x00;
			*(BYTE*)(NopeAddr + 248) = 0x00;
			*(BYTE*)(NopeAddr + 249) = 0x00;
			*(BYTE*)(NopeAddr + 250) = 0x00;
			*(BYTE*)(NopeAddr + 251) = 0x00;
			*(BYTE*)(NopeAddr + 252) = 0x00;
			*(BYTE*)(NopeAddr + 253) = 0x00;
			*(BYTE*)(NopeAddr + 254) = 0x00;
			*(BYTE*)(NopeAddr + 255) = 0x00;													//%...2.f. .<.m.i.n.i.>.S.T.R.O.N.G.<./.m.i.n.i.>.
			*(BYTE*)(NopeAddr + 256) = 0x25;
			*(BYTE*)(NopeAddr + 257) = 0x00;
			*(BYTE*)(NopeAddr + 258) = 0x2E;
			*(BYTE*)(NopeAddr + 259) = 0x00;
			*(BYTE*)(NopeAddr + 260) = 0x32;
			*(BYTE*)(NopeAddr + 261) = 0x00;
			*(BYTE*)(NopeAddr + 262) = 0x66;
			*(BYTE*)(NopeAddr + 263) = 0x00;
			*(BYTE*)(NopeAddr + 264) = 0x20;
			*(BYTE*)(NopeAddr + 265) = 0x00;
			*(BYTE*)(NopeAddr + 266) = 0x3C;
			*(BYTE*)(NopeAddr + 267) = 0x00;
			*(BYTE*)(NopeAddr + 268) = 0x6D;
			*(BYTE*)(NopeAddr + 269) = 0x00;
			*(BYTE*)(NopeAddr + 270) = 0x69;
			*(BYTE*)(NopeAddr + 271) = 0x00;
			*(BYTE*)(NopeAddr + 272) = 0x6E;
			*(BYTE*)(NopeAddr + 273) = 0x00;
			*(BYTE*)(NopeAddr + 274) = 0x69;
			*(BYTE*)(NopeAddr + 275) = 0x00;
			*(BYTE*)(NopeAddr + 276) = 0x3E;
			*(BYTE*)(NopeAddr + 277) = 0x00;
			*(BYTE*)(NopeAddr + 278) = 0x53;
			*(BYTE*)(NopeAddr + 279) = 0x00;
			*(BYTE*)(NopeAddr + 280) = 0x54;
			*(BYTE*)(NopeAddr + 281) = 0x00;
			*(BYTE*)(NopeAddr + 282) = 0x52;
			*(BYTE*)(NopeAddr + 283) = 0x00;
			*(BYTE*)(NopeAddr + 284) = 0x4F;
			*(BYTE*)(NopeAddr + 285) = 0x00;
			*(BYTE*)(NopeAddr + 286) = 0x4E;
			*(BYTE*)(NopeAddr + 287) = 0x00;
			*(BYTE*)(NopeAddr + 288) = 0x47;
			*(BYTE*)(NopeAddr + 289) = 0x00;
			*(BYTE*)(NopeAddr + 290) = 0x3C;
			*(BYTE*)(NopeAddr + 291) = 0x00;
			*(BYTE*)(NopeAddr + 292) = 0x2F;
			*(BYTE*)(NopeAddr + 293) = 0x00;
			*(BYTE*)(NopeAddr + 294) = 0x6D;
			*(BYTE*)(NopeAddr + 295) = 0x00;
			*(BYTE*)(NopeAddr + 296) = 0x69;
			*(BYTE*)(NopeAddr + 297) = 0x00;
			*(BYTE*)(NopeAddr + 298) = 0x6E;
			*(BYTE*)(NopeAddr + 299) = 0x00;
			*(BYTE*)(NopeAddr + 300) = 0x69;
			*(BYTE*)(NopeAddr + 301) = 0x00;
			*(BYTE*)(NopeAddr + 302) = 0x3E;
			*(BYTE*)(NopeAddr + 303) = 0x00;
			*(BYTE*)(NopeAddr + 304) = 0x20;
			*(BYTE*)(NopeAddr + 305) = 0x00;
			*(BYTE*)(NopeAddr + 306) = 0x00;
			*(BYTE*)(NopeAddr + 307) = 0x00;
			*(BYTE*)(NopeAddr + 308) = 0x00;
			*(BYTE*)(NopeAddr + 309) = 0x00;
			*(BYTE*)(NopeAddr + 310) = 0x00;
			*(BYTE*)(NopeAddr + 311) = 0x00;
			*(BYTE*)(NopeAddr + 312) = 0x00;
			*(BYTE*)(NopeAddr + 313) = 0x00;
			*(BYTE*)(NopeAddr + 314) = 0x00;
			*(BYTE*)(NopeAddr + 315) = 0x00;
			*(BYTE*)(NopeAddr + 316) = 0x00;
			*(BYTE*)(NopeAddr + 317) = 0x00;
			*(BYTE*)(NopeAddr + 318) = 0x00;
			*(BYTE*)(NopeAddr + 319) = 0x00;										//%...2.f. .<.m.i.n.i.>.A.W.F.U.L.<./.m.i.n.i.>. .
			*(BYTE*)(NopeAddr + 320) = 0x25;
			*(BYTE*)(NopeAddr + 321) = 0x00;
			*(BYTE*)(NopeAddr + 322) = 0x2E;
			*(BYTE*)(NopeAddr + 323) = 0x00;
			*(BYTE*)(NopeAddr + 324) = 0x32;
			*(BYTE*)(NopeAddr + 325) = 0x00;
			*(BYTE*)(NopeAddr + 326) = 0x66;
			*(BYTE*)(NopeAddr + 327) = 0x00;
			*(BYTE*)(NopeAddr + 328) = 0x20;
			*(BYTE*)(NopeAddr + 329) = 0x00;
			*(BYTE*)(NopeAddr + 330) = 0x3C;
			*(BYTE*)(NopeAddr + 331) = 0x00;
			*(BYTE*)(NopeAddr + 332) = 0x6D;
			*(BYTE*)(NopeAddr + 333) = 0x00;
			*(BYTE*)(NopeAddr + 334) = 0x69;
			*(BYTE*)(NopeAddr + 335) = 0x00;
			*(BYTE*)(NopeAddr + 336) = 0x6E;
			*(BYTE*)(NopeAddr + 337) = 0x00;
			*(BYTE*)(NopeAddr + 338) = 0x69;
			*(BYTE*)(NopeAddr + 339) = 0x00;
			*(BYTE*)(NopeAddr + 340) = 0x3E;
			*(BYTE*)(NopeAddr + 341) = 0x00;
			*(BYTE*)(NopeAddr + 342) = 0x41;
			*(BYTE*)(NopeAddr + 343) = 0x00;
			*(BYTE*)(NopeAddr + 344) = 0x57;
			*(BYTE*)(NopeAddr + 345) = 0x00;
			*(BYTE*)(NopeAddr + 346) = 0x46;
			*(BYTE*)(NopeAddr + 347) = 0x00;
			*(BYTE*)(NopeAddr + 348) = 0x55;
			*(BYTE*)(NopeAddr + 349) = 0x00;
			*(BYTE*)(NopeAddr + 350) = 0x4C;
			*(BYTE*)(NopeAddr + 351) = 0x00;
			*(BYTE*)(NopeAddr + 352) = 0x3C;
			*(BYTE*)(NopeAddr + 353) = 0x00;
			*(BYTE*)(NopeAddr + 354) = 0x2F;
			*(BYTE*)(NopeAddr + 355) = 0x00;
			*(BYTE*)(NopeAddr + 356) = 0x6D;
			*(BYTE*)(NopeAddr + 357) = 0x00;
			*(BYTE*)(NopeAddr + 358) = 0x69;
			*(BYTE*)(NopeAddr + 359) = 0x00;
			*(BYTE*)(NopeAddr + 360) = 0x6E;
			*(BYTE*)(NopeAddr + 361) = 0x00;
			*(BYTE*)(NopeAddr + 362) = 0x69;
			*(BYTE*)(NopeAddr + 363) = 0x00;
			*(BYTE*)(NopeAddr + 364) = 0x3E;
			*(BYTE*)(NopeAddr + 365) = 0x00;
			*(BYTE*)(NopeAddr + 366) = 0x20;
			*(BYTE*)(NopeAddr + 367) = 0x00;
			*(BYTE*)(NopeAddr + 368) = 0x00;
			*(BYTE*)(NopeAddr + 369) = 0x00;
			*(BYTE*)(NopeAddr + 370) = 0x00;
			*(BYTE*)(NopeAddr + 371) = 0x00;
			*(BYTE*)(NopeAddr + 372) = 0x00;
			*(BYTE*)(NopeAddr + 373) = 0x00;
			*(BYTE*)(NopeAddr + 374) = 0x00;
			*(BYTE*)(NopeAddr + 375) = 0x00;
			*(BYTE*)(NopeAddr + 376) = 0x00;
			*(BYTE*)(NopeAddr + 377) = 0x00;
			*(BYTE*)(NopeAddr + 378) = 0x00;
			*(BYTE*)(NopeAddr + 379) = 0x00;
			*(BYTE*)(NopeAddr + 380) = 0x00;
			*(BYTE*)(NopeAddr + 381) = 0x00;
			*(BYTE*)(NopeAddr + 382) = 0x00;
			*(BYTE*)(NopeAddr + 383) = 0x00;													//%...2.f. .<.m.i.n.i.>.B.O.S.S.<./.m.i.n.i.>. ...
			*(BYTE*)(NopeAddr + 384) = 0x25;
			*(BYTE*)(NopeAddr + 385) = 0x00;
			*(BYTE*)(NopeAddr + 386) = 0x2E;
			*(BYTE*)(NopeAddr + 387) = 0x00;
			*(BYTE*)(NopeAddr + 388) = 0x32;
			*(BYTE*)(NopeAddr + 389) = 0x00;
			*(BYTE*)(NopeAddr + 390) = 0x66;
			*(BYTE*)(NopeAddr + 391) = 0x00;
			*(BYTE*)(NopeAddr + 392) = 0x20;
			*(BYTE*)(NopeAddr + 393) = 0x00;
			*(BYTE*)(NopeAddr + 394) = 0x3C;
			*(BYTE*)(NopeAddr + 395) = 0x00;
			*(BYTE*)(NopeAddr + 396) = 0x6D;
			*(BYTE*)(NopeAddr + 397) = 0x00;
			*(BYTE*)(NopeAddr + 398) = 0x69;
			*(BYTE*)(NopeAddr + 399) = 0x00;
			*(BYTE*)(NopeAddr + 400) = 0x6E;
			*(BYTE*)(NopeAddr + 401) = 0x00;
			*(BYTE*)(NopeAddr + 402) = 0x69;
			*(BYTE*)(NopeAddr + 403) = 0x00;
			*(BYTE*)(NopeAddr + 404) = 0x3E;
			*(BYTE*)(NopeAddr + 405) = 0x00;
			*(BYTE*)(NopeAddr + 406) = 0x42;
			*(BYTE*)(NopeAddr + 407) = 0x00;
			*(BYTE*)(NopeAddr + 408) = 0x4F;
			*(BYTE*)(NopeAddr + 409) = 0x00;
			*(BYTE*)(NopeAddr + 410) = 0x53;
			*(BYTE*)(NopeAddr + 411) = 0x00;
			*(BYTE*)(NopeAddr + 412) = 0x53;
			*(BYTE*)(NopeAddr + 413) = 0x00;
			*(BYTE*)(NopeAddr + 414) = 0x3C;
			*(BYTE*)(NopeAddr + 415) = 0x00;
			*(BYTE*)(NopeAddr + 416) = 0x2F;
			*(BYTE*)(NopeAddr + 417) = 0x00;
			*(BYTE*)(NopeAddr + 418) = 0x6D;
			*(BYTE*)(NopeAddr + 419) = 0x00;
			*(BYTE*)(NopeAddr + 420) = 0x69;
			*(BYTE*)(NopeAddr + 421) = 0x00;
			*(BYTE*)(NopeAddr + 422) = 0x6E;
			*(BYTE*)(NopeAddr + 423) = 0x00;
			*(BYTE*)(NopeAddr + 424) = 0x69;
			*(BYTE*)(NopeAddr + 425) = 0x00;
			*(BYTE*)(NopeAddr + 426) = 0x3E;
			*(BYTE*)(NopeAddr + 427) = 0x00;
			*(BYTE*)(NopeAddr + 428) = 0x20;
			*(BYTE*)(NopeAddr + 429) = 0x00;
			*(BYTE*)(NopeAddr + 430) = 0x00;
			*(BYTE*)(NopeAddr + 431) = 0x00;
			*(BYTE*)(NopeAddr + 432) = 0x00;
			*(BYTE*)(NopeAddr + 433) = 0x00;
			*(BYTE*)(NopeAddr + 434) = 0x00;
			*(BYTE*)(NopeAddr + 435) = 0x00;
			*(BYTE*)(NopeAddr + 436) = 0x00;
			*(BYTE*)(NopeAddr + 437) = 0x00;
			*(BYTE*)(NopeAddr + 438) = 0x00;
			*(BYTE*)(NopeAddr + 439) = 0x00;
			*(BYTE*)(NopeAddr + 440) = 0x00;
			*(BYTE*)(NopeAddr + 441) = 0x00;
			*(BYTE*)(NopeAddr + 442) = 0x00;
			*(BYTE*)(NopeAddr + 443) = 0x00;
			*(BYTE*)(NopeAddr + 444) = 0x00;
			*(BYTE*)(NopeAddr + 445) = 0x00;
			*(BYTE*)(NopeAddr + 446) = 0x00;
			*(BYTE*)(NopeAddr + 447) = 0x00;

			*(BYTE*)(Power5Addr + 2) = 0x90;
			*(BYTE*)(Power5Addr + 3) = 0xe9;

			ConsoleOutput(L"-전투력 스카우터    [패치 성공]\n");
		}


		if (ScrapC == 1)
		{
			*(BYTE*)ScrapAddr = 0xB0;
			*(BYTE*)(ScrapAddr + 1) = 0x01;
			*(BYTE*)(ScrapAddr + 2) = 0xC2;
			*(BYTE*)(ScrapAddr + 3) = 0x04;
			*(BYTE*)(ScrapAddr + 4) = 0x00;

			ConsoleOutput(L"-수집일기 별 표시    [패치 성공]\n");
		}


		if (AvonC == 1)
		{
			*(BYTE*)(AvonAddr + 2) = 0x90;
			*(BYTE*)(AvonAddr + 3) = 0xE9;

			ConsoleOutput(L"-아본 커튼 제거    [패치 성공]\n");
		}


		if (ZoomSC == 1)
		{
			*(DWORD*)(ZoomSAddr + 52) = ZoomS200Addr;

			ConsoleOutput(L"-줌 속도 빠르게    [패치 성공]\n");
		}


		if (PetLegC == 99887766)
		{
			*(DWORD*)PetLegAddr = 0x70;

			ConsoleOutput(L"-펫 소환 지연 렉 제거    [패치 성공]\n");
		}


		if (CookC == 1)
		{
			*(BYTE*)(CookAddr + 6) = 0xEB;

			ConsoleOutput(L"-요리 게이지 흔들림 제거    [패치 성공]\n");
		}


		if (NightTC == 1)
		{
			*(BYTE*)(NightTAddr + 2) = 0xC7;
			*(BYTE*)(NightTAddr + 3) = 0x40;
			*(BYTE*)(NightTAddr + 4) = 0x0C;
			*(BYTE*)(NightTAddr + 5) = 0x00;
			*(BYTE*)(NightTAddr + 6) = 0x00;
			*(BYTE*)(NightTAddr + 7) = 0x00;
			*(BYTE*)(NightTAddr + 8) = 0x3F;
			*(BYTE*)(NightTAddr + 9) = 0x5D;
			*(BYTE*)(NightTAddr + 10) = 0xC2;
			*(BYTE*)(NightTAddr + 11) = 0x04;
			*(BYTE*)(NightTAddr + 12) = 0x00;

			ConsoleOutput(L"-에린을 항상 낮으로    [패치 성공]\n");
		}


		if (PerC == 1)
		{
			*(BYTE*)(PerAddr + 4) = 0x34;

			ConsoleOutput(L"-각종 수치 자세히 보기    [패치 성공]\n");
		}


		if (StreetLampC == 1)
		{
			*(BYTE*)(StreetLampAddr + 6) = 0x44;
			*(BYTE*)(StreetLampAddr + 8) = 0x61;
			*(BYTE*)(StreetLampAddr + 10) = 0x79;
			*(BYTE*)(StreetLampAddr + 12) = 0x00;
			*(BYTE*)(StreetLampAddr + 14) = 0x00;
			*(BYTE*)(StreetLampAddr + 52) = 0x44;
			*(BYTE*)(StreetLampAddr + 54) = 0x61;
			*(BYTE*)(StreetLampAddr + 56) = 0x79;
			*(BYTE*)(StreetLampAddr + 58) = 0x00;
			*(BYTE*)(StreetLampAddr + 60) = 0x00;

			ConsoleOutput(L"-가로등 불 끄기    [패치 성공]\n");
		}


		if (RightC == 1)
		{
			*(BYTE*)RightAddr = 0x90;
			*(BYTE*)(RightAddr + 1) = 0xE9;


			ConsoleOutput(L"-자기 자신 우 클릭 활성화    [패치 성공]\n");
		}


		if (mapColorC == 1)
		{
			*(BYTE*)(mapColorAddr + 1) = 0x00;
			*(BYTE*)(mapColorAddr + 2) = 0x00;
			*(BYTE*)(mapColorAddr + 3) = 0x00;

			ConsoleOutput(L"-미니맵 컬러링_검정    [패치 성공]\n");
		}


		if (mapColorC == 2)
		{
			*(BYTE*)(mapColorAddr + 1) = 0xFF;
			*(BYTE*)(mapColorAddr + 2) = 0x00;
			*(BYTE*)(mapColorAddr + 3) = 0x00;

			ConsoleOutput(L"-미니맵 컬러링_파랑    [패치 성공]\n");
		}


		if (PriceC == 1)
		{
			*(BYTE*)(PriceAddr + 12) = 0x90;
			*(BYTE*)(PriceAddr + 13) = 0x90;
			*(BYTE*)(PriceAddr + 14) = 0x90;
			*(BYTE*)(PriceAddr + 15) = 0x90;
			*(BYTE*)(PriceAddr + 16) = 0x90;
			*(BYTE*)(PriceAddr + 17) = 0x90;

			ConsoleOutput(L"-아이템 가격 표시    [패치 성공]\n");
		}


		if (BlackC == 1)
		{
			*(BYTE*)(BlackAddr + 3) = 0x86;

			ConsoleOutput(L"-블랙리스트    [패치 성공]\n");
		}


		if (MiniNogiC == 1 || MiniNogiC == 2 || MiniNogiC == 3)
		{
			*(BYTE*)(MiniNogiAddr + 0x2300000) = 0x33;
			*(BYTE*)(MiniNogiAddr + 0x2300001) = 0x33;
			*(BYTE*)(MiniNogiAddr + 0x2300002) = 0x33;
			*(BYTE*)(MiniNogiAddr + 0x2300003) = 0x33;
			*(BYTE*)(MiniNogiAddr + 0x2300004) = 0x33;
			*(BYTE*)(MiniNogiAddr + 0x2300005) = 0x33;
			*(BYTE*)(MiniNogiAddr + 0x2300006) = 0xD3;
			*(BYTE*)(MiniNogiAddr + 0x2300007) = 0x3F;

			*(BYTE*)(MiniNogiAddr + 0x2300010) = 0x9A;
			*(BYTE*)(MiniNogiAddr + 0x2300011) = 0x99;
			*(BYTE*)(MiniNogiAddr + 0x2300012) = 0x99;
			*(BYTE*)(MiniNogiAddr + 0x2300013) = 0x99;
			*(BYTE*)(MiniNogiAddr + 0x2300014) = 0x99;
			*(BYTE*)(MiniNogiAddr + 0x2300015) = 0x99;
			*(BYTE*)(MiniNogiAddr + 0x2300016) = 0xD9;
			*(BYTE*)(MiniNogiAddr + 0x2300017) = 0x3F;

			if (MiniNogiC == 1 || MiniNogiC == 2)
			{
				*(DWORD*)(MiniNogiAddr + 70) = (MiniNogiAddr + 0x2300010);
			}

			if (MiniNogiC == 1 || MiniNogiC == 3)
			{
				*(DWORD*)(MiniNogiAddr + 2) = (MiniNogiAddr + 0x2300000);
			}

			ConsoleOutput(L"-미니노기    [패치 성공]\n");
		}

		if (ZeroC == 1)
		{
			*(DWORD*)(ZeroAddr + 32) = Zero50Addr;
			*(DWORD*)(ZeroAddr + 55) = Zero50Addr;
			*(BYTE*)(Zero500Addr + 8) = 0x90;
			*(BYTE*)(Zero500Addr + 9) = 0xE9;
			ConsoleOutput(L"-제로 고도 비행    [패치 성공]\n");
		}


		if (RecC == 1)
		{
			*(BYTE*)(RecAddr + 7) = 0x80;
			*(BYTE*)(RecAddr + 8) = 0xEE;
			*(BYTE*)(RecAddr + 9) = 0x36;
			ConsoleOutput(L"-영상 녹화 제한시간 확장    [패치 성공]\n");
		}


		if (TitleC == 1)
		{
			*(BYTE*)(TitleAddr) = 0xEB;
			ConsoleOutput(L"-2차 및 재능 타이틀 제거    [패치 성공]\n");
		}


		if (FramesC == 1)
		{
			*(BYTE*)(FramesAddr - 5) = 0xB8;
			*(BYTE*)(FramesAddr - 4) = 0x00;
			*(BYTE*)(FramesAddr - 3) = 0x00;
			*(BYTE*)(FramesAddr - 2) = 0x00;
			*(BYTE*)(FramesAddr - 1) = 0x00;

			ConsoleOutput(L"-간소화 프레임 무시    [패치 성공]\n");
		}

		if (Nosky1C == 1 && Nosky2C == 1)
		{
			*(BYTE*)(Nosky1Addr) = 0xEB;

			*(BYTE*)(Nosky2Addr + 5) = 0x90;
			*(BYTE*)(Nosky2Addr + 6) = 0xE9;

			ConsoleOutput(L"-에린 하늘 제거    [패치 성공]\n");
		}


		if (TalkC == 1)
		{
			*(BYTE*)(TalkAddr + 3) = 0x90;
			*(BYTE*)(TalkAddr + 4) = 0x90;

			ConsoleOutput(L"-대사 빠르게    [패치 성공]\n");
		}
	}
	catch (...) {
		ConsoleOutput(L"패치 중 에러가 발생하였습니다.\n", lred);
		return 1;
	}
	return 0;
}



bool PatternSearch()
{
	if (iniParty == 1)
	{
		ConsoleOutput(L"-파티 창 유지시간 무제한");
		CPatternSearch ptnDisableCloseParty(DisableCloseParty);
		DisableClosePartyAddr = ptnDisableCloseParty.BytePatternSearch();

		if (CheckPatternSearchSuccess(DisableClosePartyAddr))
		{
			int PartyC = 0;
			::PartyC += 1;
		}
	}

	if (iniAdv == 1)
	{
		ConsoleOutput(L"-하우징 게시판 스왑_1");
		CPatternSearch ptnPartyToAdv1(PartyToAdv1);
		PartyToAdv1Addr = ptnPartyToAdv1.BytePatternSearch();

		if (CheckPatternSearchSuccess(PartyToAdv1Addr))
		{
			int HouseC = 0;
			::HouseC += 1;
		}

		ConsoleOutput(L"-하우징 게시판 스왑_2");
		CPatternSearch ptnPartyToAdvMain(PartyToAdvMain);
		PartyToAdvMainAddr = ptnPartyToAdvMain.BytePatternSearch();

		if (CheckPatternSearchSuccess(PartyToAdvMainAddr))
		{
			int HouseC2 = 0;
			::HouseC2 += 1;
		}
	}


	if (iniZoom == 1)
	{
		ConsoleOutput(L"-시야확장_1");
		CPatternSearch ptnZoom(Zoom);
		ZoomAddr = ptnZoom.BytePatternSearch();

		if (CheckPatternSearchSuccess(ZoomAddr))
		{
			int ZoomC = 0;
			::ZoomC += 1;
		}

		ConsoleOutput(L"-시야확장_2");
		CPatternSearch ptnZoom_10000(Zoom_10000);
		Zoom_10000Addr = ptnZoom_10000.BytePatternSearch();

		if (CheckPatternSearchSuccess(Zoom_10000Addr))
		{
			int ZoomC2 = 0;
			::ZoomC2 += 1;
		}
	}


	if (iniZoom == 2)
	{
		ConsoleOutput(L"-시야확장_1");
		CPatternSearch ptnZoom(Zoom);
		ZoomAddr = ptnZoom.BytePatternSearch();

		if (CheckPatternSearchSuccess(ZoomAddr))
		{
			int ZoomC = 0;
			::ZoomC += 1;
		}


		ConsoleOutput(L"-시야확장_2");
		CPatternSearch ptnZoom_5000(Zoom_5000);
		Zoom_5000Addr = ptnZoom_5000.BytePatternSearch();

		if (CheckPatternSearchSuccess(Zoom_5000Addr))
		{
			int ZoomC2 = 0;
			::ZoomC2 += 2;
		}
	}


	if (iniClock == 1)
	{
		ConsoleOutput(L"-분 단위 시계");
		CPatternSearch ptnClock(Clock);
		ClockAddr = ptnClock.BytePatternSearch();

		if (CheckPatternSearchSuccess(ClockAddr))
		{
			int ClockC = 0;
			::ClockC += 1;
		}
	}


	if (iniAllNames == 1)
	{
		ConsoleOutput(L"-시야내 모든 이름표시");
		CPatternSearch ptnAllNamesInVision(AllNamesInVision);
		AllNamesInVisionAddr = ptnAllNamesInVision.BytePatternSearch();

		if (CheckPatternSearchSuccess(AllNamesInVisionAddr))
		{
			int AllNamesInVisionC = 0;
			::AllNamesInVisionC += 1;
		}
	}


	if (iniChannel == 1)
	{
		ConsoleOutput(L"-동일 채널 재접속");
		CPatternSearch ptnMoveToSameChannel(MoveToSameChannel);
		MoveToSameChannelAddr = ptnMoveToSameChannel.BytePatternSearch();

		if (CheckPatternSearchSuccess(MoveToSameChannelAddr))
		{
			int MoveToSameChannelC = 0;
			::MoveToSameChannelC += 1;
		}
	}


	if (iniAnyItem == 1)
	{
		ConsoleOutput(L"-단축키에 무엇이든 올리기");
		CPatternSearch ptnPutFxAnyItem(PutFxAnyItem);
		PutFxAnyItemAddr = ptnPutFxAnyItem.BytePatternSearch();

		if (CheckPatternSearchSuccess(PutFxAnyItemAddr))
		{
			int PutFxAnyItemC = 0;
			::PutFxAnyItemC += 1;
		}
	}


	if (iniEgo == 1)
	{
		ConsoleOutput(L"-미장착 정령대화");
		CPatternSearch ptnEgo(Ego);
		EgoAddr = ptnEgo.BytePatternSearch();

		if (CheckPatternSearchSuccess(EgoAddr))
		{
			int EgoC = 0;
			::EgoC += 1;
		}
	}


	if (iniRobbery == 1)
	{
		ConsoleOutput(L"-미니맵에 약탈단 표시");
		CPatternSearch ptnDisplayRobbery(DisplayRobbery);
		DisplayRobberyAddr = ptnDisplayRobbery.BytePatternSearch();

		if (CheckPatternSearchSuccess(DisplayRobberyAddr))
		{
			int DisplayRobberyC = 0;
			::DisplayRobberyC += 1;
		}
	}


	if (iniMimic == 1)
	{
		ConsoleOutput(L"-미믹 타겟팅");
		CPatternSearch ptnMimic(Mimic);
		MimicAddr = ptnMimic.BytePatternSearch();

		if (CheckPatternSearchSuccess(MimicAddr))
		{
			int MimicC = 0;
			::MimicC += 1;
		}
	}


	if (iniNameColoring == 1)
	{
		ConsoleOutput(L"-이름표시 색 변경");
		CPatternSearch ptnNameColoring(NameColoring);
		NameColoringAddr = ptnNameColoring.BytePatternSearch();

		if (CheckPatternSearchSuccess(NameColoringAddr))
		{
			int NameColoringC = 0;
			::NameColoringC += 1;
		}
	}


	if (iniRWanted == 1)
	{
		ConsoleOutput(L"-콤보카드 활성 버튼 -> 추적게시판");
		CPatternSearch ptnRWanted(RWanted);
		RWantedAddr = ptnRWanted.BytePatternSearch();

		if (CheckPatternSearchSuccess(RWantedAddr))
		{
			int RWantedC = 0;
			::RWantedC += 1;
		}
	}


	if (iniLflying == 1)
	{
		ConsoleOutput(L"-정령대화 -> 낭만비행");
		CPatternSearch ptnNang(Nang);
		NangAddr = ptnNang.BytePatternSearch();

		if (CheckPatternSearchSuccess(NangAddr))
		{
			int NangC = 0;
			::NangC += 1;
		}
	}



	if (iniLflying == 2)
	{
		ConsoleOutput(L"-콤보카드 비활성 버튼 -> 낭만비행");
		CPatternSearch ptnNang2(Nang2);
		Nang2Addr = ptnNang2.BytePatternSearch();

		if (CheckPatternSearchSuccess(Nang2Addr))
		{
			int Nang2C = 0;
			::Nang2C += 1;
		}
	}



	if (iniPower == 1)
	{
		ConsoleOutput(L"-전투력 표시_1");
		CPatternSearch ptnPower1(Power1);
		Power1Addr = ptnPower1.BytePatternSearch();

		if (CheckPatternSearchSuccess(Power1Addr))
		{
			int Power1C = 0;
			::Power1C += 1;
		}


		ConsoleOutput(L"-전투력 표시_2");
		CPatternSearch ptnPower2(Power2);
		Power2Addr = ptnPower2.BytePatternSearch();

		if (CheckPatternSearchSuccess(Power2Addr))
		{
			int Power2C = 0;
			::Power2C += 1;
		}


		ConsoleOutput(L"-전투력 표시_3");
		CPatternSearch ptnPower3(Power3);
		Power3Addr = ptnPower3.BytePatternSearch();

		if (CheckPatternSearchSuccess(Power3Addr))
		{
			int Power3C = 0;
			::Power3C += 1;
		}


		ConsoleOutput(L"-전투력 표시_4");
		CPatternSearch ptnPower4(Power4);
		Power4Addr = ptnPower4.BytePatternSearch();

		if (CheckPatternSearchSuccess(Power4Addr))
		{
			int Power4C = 0;
			::Power4C += 1;
		}


		ConsoleOutput(L"-전투력 표시_5");
		CPatternSearch ptnPower5(Power5);
		Power5Addr = ptnPower5.BytePatternSearch();

		if (CheckPatternSearchSuccess(Power5Addr))
		{
			int Power5C = 0;
			::Power5C += 1;
		}



		ConsoleOutput(L"-전투력 표시_6");
		CPatternSearch ptnNope(Nope);
		NopeAddr = ptnNope.BytePatternSearch();

		if (CheckPatternSearchSuccess(NopeAddr))
		{
			int NopeC = 0;
			::NopeC += 1;
		}
	}


	if (iniScrapBook == 1)
	{
		ConsoleOutput(L"-수집일기 별 표시");
		CPatternSearch ptnScrap(Scrap);
		ScrapAddr = ptnScrap.BytePatternSearch();

		if (CheckPatternSearchSuccess(ScrapAddr))
		{
			int ScrapC = 0;
			::ScrapC += 1;
		}
	}


	if (iniAvon == 1)
	{
		ConsoleOutput(L"-아본 커튼 제거");
		CPatternSearch ptnAvon(Avon);
		AvonAddr = ptnAvon.BytePatternSearch();

		if (CheckPatternSearchSuccess(AvonAddr))
		{
			int AvonC = 0;
			::AvonC += 1;
		}
	}


	if (iniZoomSpeed == 1)
	{
		ConsoleOutput(L"-줌 속도 빠르게_1");
		CPatternSearch ptnZoomS(ZoomS);
		ZoomSAddr = ptnZoomS.BytePatternSearch();

		if (CheckPatternSearchSuccess(ZoomSAddr))
		{
			int ZoomSC = 0;
			::ZoomSC += 1;
		}


		ConsoleOutput(L"-줌 속도 빠르게_2");
		CPatternSearch ptnZoomS200(ZoomS200);
		ZoomS200Addr = ptnZoomS200.BytePatternSearch();

		if (CheckPatternSearchSuccess(ZoomS200Addr))
		{
			int ZoomS200C = 0;
			::ZoomS200C += 1;
		}
	}


	if (iniLag == 1)
	{
		ConsoleOutput(L"-펫 소환 지연 렉 제거");
		CPatternSearch ptnPetLeg(PetLeg);
		PetLegAddr = ptnPetLeg.BytePatternSearch();

		if (CheckPatternSearchSuccess(PetLegAddr))
		{
			int PetLegC = 0;
			::PetLegC += 1;
		}
	}


	if (iniCookingNoise == 1)
	{
		ConsoleOutput(L"-요리 게이지 흔들림 제거");
		CPatternSearch ptnCook(Cook);
		CookAddr = ptnCook.BytePatternSearch();

		if (CheckPatternSearchSuccess(CookAddr))
		{
			int CookC = 0;
			::CookC += 1;
		}
	}


	if (iniNighttime == 1)
	{
		ConsoleOutput(L"-에린을 항상 낮으로");
		CPatternSearch ptnNightT(NightT);
		NightTAddr = ptnNightT.BytePatternSearch();

		if (CheckPatternSearchSuccess(NightTAddr))
		{
			int NightTC = 0;
			::NightTC += 1;
		}
	}


	if (iniPercentage == 1)
	{
		ConsoleOutput(L"-각종 수치 자세히 보기");
		CPatternSearch ptnPer(Per);
		PerAddr = ptnPer.BytePatternSearch();

		if (CheckPatternSearchSuccess(PerAddr))
		{
			int PerC = 0;
			::PerC += 1;
		}
	}


	if (iniStreetLamp == 1)
	{
		ConsoleOutput(L"-가로등 불 끄기");
		CPatternSearch ptnStreetLamp(StreetLamp);
		StreetLampAddr = ptnStreetLamp.BytePatternSearch();

		if (CheckPatternSearchSuccess(StreetLampAddr))
		{
			int StreetLampC = 0;
			::StreetLampC += 1;
		}
	}


	if (iniRightClick == 1)
	{
		ConsoleOutput(L"-자기 자신 우 클릭 활성화");
		CPatternSearch ptnRight(Right);
		RightAddr = ptnRight.BytePatternSearch();

		if (CheckPatternSearchSuccess(RightAddr))
		{
			int RightC = 0;
			::RightC += 1;
		}
	}


	if (iniMinimapColor == 1)
	{
		ConsoleOutput(L"-미니맵 컬러링");
		CPatternSearch ptnmapColor(mapColor);
		mapColorAddr = ptnmapColor.BytePatternSearch();

		if (CheckPatternSearchSuccess(mapColorAddr))
		{
			int mapColorC = 0;
			::mapColorC += 1;
		}
	}


	if (iniMinimapColor == 2)
	{
		ConsoleOutput(L"-미니맵 컬러링");
		CPatternSearch ptnmapColor(mapColor);
		mapColorAddr = ptnmapColor.BytePatternSearch();

		if (CheckPatternSearchSuccess(mapColorAddr))
		{
			int mapColorC = 0;
			::mapColorC += 2;
		}
	}


	if (iniitemPrice == 1)
	{
		ConsoleOutput(L"-아이템 가격 표시");
		CPatternSearch ptnPrice(Price);
		PriceAddr = ptnPrice.BytePatternSearch();

		if (CheckPatternSearchSuccess(PriceAddr))
		{
			int PriceC = 0;
			::PriceC += 1;
		}
	}


	if (iniBlackList == 1)
	{
		ConsoleOutput(L"-블랙리스트");
		CPatternSearch ptnBlack(Black);
		BlackAddr = ptnBlack.BytePatternSearch();

		if (CheckPatternSearchSuccess(BlackAddr))
		{
			int BlackC = 0;
			::BlackC += 1;
		}
	}


	if (iniMiniNogi == 1)
	{
		ConsoleOutput(L"-미니노기");
		CPatternSearch ptnMiniNogi(MiniNogi);
		MiniNogiAddr = ptnMiniNogi.BytePatternSearch();

		if (CheckPatternSearchSuccess(MiniNogiAddr))
		{
			int MiniNogiC = 0;
			::MiniNogiC += 1;
		}
	}


	if (iniMiniNogi == 2)
	{
		ConsoleOutput(L"-미니노기");
		CPatternSearch ptnMiniNogi(MiniNogi);
		MiniNogiAddr = ptnMiniNogi.BytePatternSearch();

		if (CheckPatternSearchSuccess(MiniNogiAddr))
		{
			int MiniNogiC = 0;
			::MiniNogiC += 2;
		}
	}


	if (iniMiniNogi == 3)
	{
		ConsoleOutput(L"-미니노기");
		CPatternSearch ptnMiniNogi(MiniNogi);
		MiniNogiAddr = ptnMiniNogi.BytePatternSearch();

		if (CheckPatternSearchSuccess(MiniNogiAddr))
		{
			int MiniNogiC = 0;
			::MiniNogiC += 3;
		}
	}


	if (iniZero == 1)
	{

		ConsoleOutput(L"-제로 고도 비행");
		CPatternSearch ptnZero(Zero);
		ZeroAddr = ptnZero.BytePatternSearch();

		if (CheckPatternSearchSuccess(ZeroAddr))
		{
			int ZeroC = 0;
			::ZeroC += 1;
		}

		ConsoleOutput(L"-제로 고도 비행_2");
		CPatternSearch ptnZero50(Zero50);
		Zero50Addr = ptnZero50.BytePatternSearch();

		if (CheckPatternSearchSuccess(Zero50Addr))
		{
			int Zero50C = 0;
			::Zero50C += 1;
		}

		ConsoleOutput(L"-제로 고도 비행_3");
		CPatternSearch ptnZero500(Zero500);
		Zero500Addr = ptnZero500.BytePatternSearch();

		if (CheckPatternSearchSuccess(Zero500Addr))
		{
			int Zero500C = 0;
			::Zero500C += 1;
		}
	}

	if (iniRec == 1)
	{
		ConsoleOutput(L"-영상 녹화시간 확장");
		CPatternSearch ptnRec(Rec);
		RecAddr = ptnRec.BytePatternSearch();

		if (CheckPatternSearchSuccess(RecAddr))
		{
			int RecC = 0;
			::RecC += 1;
		}
	}


	if (iniTitle == 1)
	{
		ConsoleOutput(L"-2차 및 재능 타이틀 제거");
		CPatternSearch ptnTitle(Title);
		TitleAddr = ptnTitle.BytePatternSearch();

		if (CheckPatternSearchSuccess(TitleAddr))
		{
			int TitleC = 0;
			::TitleC += 1;
		}
	}



	if (iniFrames == 1)
	{
		ConsoleOutput(L"-간소화 프레임 무시");
		CPatternSearch ptnFrames(Frames);
		FramesAddr = ptnFrames.BytePatternSearch();

		if (CheckPatternSearchSuccess(FramesAddr))
		{
			int FramesC = 0;
			::FramesC += 1;
		}
	}


	if (iniNosky == 1)
	{
		ConsoleOutput(L"-에린 하늘 제거_1");
		CPatternSearch ptnNosky1(Nosky1);
		Nosky1Addr = ptnNosky1.BytePatternSearch();

		if (CheckPatternSearchSuccess(Nosky1Addr))
		{
			int Nosky1C = 0;
			::Nosky1C += 1;
		}

		ConsoleOutput(L"-에린 하늘 제거_2");
		CPatternSearch ptnNosky2(Nosky2);
		Nosky2Addr = ptnNosky2.BytePatternSearch();

		if (CheckPatternSearchSuccess(Nosky2Addr))
		{
			int Nosky2C = 0;
			::Nosky2C += 1;
		}
	}


	if (iniTalk == 1)
	{
		ConsoleOutput(L"-대사 빠르게");
		CPatternSearch ptnTalkSpeed(TalkSpeed);
		TalkAddr = ptnTalkSpeed.BytePatternSearch();

		if (CheckPatternSearchSuccess(TalkAddr))
		{
			int TalkC = 0;
			::TalkC += 1;
		}
	}



	if (iniPatch == 10011)
	{
		ConsoleOutput(L"-Cherry_SendHookee");
		CPatternSearch ptnfloche1(floche1);
		floche1Addr = ptnfloche1.BytePatternSearch();
		floche11Addr = (floche1Addr + 0x5);

		if (CheckPatternSearchSuccess(floche1Addr))
		{
			int floche1C = 0;
			::floche1C += 1;
		}


		ConsoleOutput(L"-Cherry_CMessage");
		CPatternSearch ptnfloche2(floche2);
		floche2Addr = ptnfloche2.BytePatternSearch();

		if (CheckPatternSearchSuccess(floche2Addr))
		{
			int floche2C = 0;
			::floche2C += 1;
		}



		ConsoleOutput(L"-Cherry_GetStreamLength");
		CPatternSearch ptnfloche3(floche3);
		floche3Addr = ptnfloche3.BytePatternSearch();

		if (CheckPatternSearchSuccess(floche3Addr))
		{
			int floche3C = 0;
			::floche3C += 1;
		}


		ConsoleOutput(L"-Cherry_WriteToNetworkBuffer");
		CPatternSearch ptnfloche4(floche4);
		floche4Addr = ptnfloche4.BytePatternSearch();

		if (CheckPatternSearchSuccess(floche4Addr))
		{
			int floche4C = 0;
			::floche4C += 1;
		}


		ConsoleOutput(L"-Cherry_CMessage2");
		CPatternSearch ptnfloche5(floche5);
		floche5Addr = ptnfloche5.BytePatternSearch();

		if (CheckPatternSearchSuccess(floche5Addr))
		{
			int floche5C = 0;
			::floche5C += 1;
		}


		ConsoleOutput(L"-Cherry_Mint_236E2D_4F");
		CPatternSearch ptnfloche6(floche6);
		floche6Addr = ptnfloche6.BytePatternSearch();

		if (CheckPatternSearchSuccess(floche6Addr))
		{
			int floche6C = 0;
			::floche6C += 1;
		}


		ConsoleOutput(L"-Cherry_Post");
		CPatternSearch ptnfloche7(floche7);
		floche7Addr = ptnfloche7.BytePatternSearch();

		if (CheckPatternSearchSuccess(floche7Addr))
		{
			int floche7C = 0;
			::floche7C += 1;
		}


		ConsoleOutput(L"-Cherry_ReadFromNetworkBuffer");
		CPatternSearch ptnfloche8(floche8);
		floche8Addr = ptnfloche8.BytePatternSearch();

		if (CheckPatternSearchSuccess(floche8Addr))
		{
			int floche8C = 0;
			::floche8C += 1;
		}
	}
	return 0;
}



bool PatchClient2()
{
	try {
		ConsoleOutput(L"\n\n== 패치 중 2 =========================\n\n");
		::Login2C += 1;
		if (Login2C == 1)
		{
			*(BYTE*)(LoginAddr + 0x1970BA4) = 0x55;
			*(BYTE*)(LoginAddr + 0x1970BA5) = 0x8B;
			*(BYTE*)(LoginAddr + 0x1970BA6) = 0xEC;
			*(BYTE*)(LoginAddr + 0x1970BA7) = 0xB8;
			*(BYTE*)(LoginAddr + 0x1970BA8) = 0x01;
			*(BYTE*)(LoginAddr + 0x1970BA9) = 0x00;
			*(BYTE*)(LoginAddr + 0x1970BAA) = 0x00;
			*(BYTE*)(LoginAddr + 0x1970BAB) = 0x00;
			*(BYTE*)(LoginAddr + 0x1970BAC) = 0x8B;
			*(BYTE*)(LoginAddr + 0x1970BAD) = 0x4D;
			*(BYTE*)(LoginAddr + 0x1970BAE) = 0x08;
			*(BYTE*)(LoginAddr + 0x1970BAF) = 0x3B;
			*(BYTE*)(LoginAddr + 0x1970BB0) = 0xC1;
			*(BYTE*)(LoginAddr + 0x1970BB1) = 0x76;
			*(BYTE*)(LoginAddr + 0x1970BB2) = 0x06;
			*(BYTE*)(LoginAddr + 0x1970BB3) = 0xB0;
			*(BYTE*)(LoginAddr + 0x1970BB4) = 0x01;
			*(BYTE*)(LoginAddr + 0x1970BB5) = 0x5D;
			*(BYTE*)(LoginAddr + 0x1970BB6) = 0xC2;
			*(BYTE*)(LoginAddr + 0x1970BB7) = 0x0C;
			*(BYTE*)(LoginAddr + 0x1970BB8) = 0x00;
			*(BYTE*)(LoginAddr + 0x1970BB9) = 0x73;
			*(BYTE*)(LoginAddr + 0x1970BBA) = 0x06;
			*(BYTE*)(LoginAddr + 0x1970BBB) = 0x32;
			*(BYTE*)(LoginAddr + 0x1970BBC) = 0xC0;
			*(BYTE*)(LoginAddr + 0x1970BBD) = 0x5D;
			*(BYTE*)(LoginAddr + 0x1970BBE) = 0xC2;
			*(BYTE*)(LoginAddr + 0x1970BBF) = 0x0C;
			*(BYTE*)(LoginAddr + 0x1970BC0) = 0x00;
			*(BYTE*)(LoginAddr + 0x1970BC1) = 0xB8;
			*(BYTE*)(LoginAddr + 0x1970BC2) = 0x00;
			*(BYTE*)(LoginAddr + 0x1970BC3) = 0x00;
			*(BYTE*)(LoginAddr + 0x1970BC4) = 0x00;
			*(BYTE*)(LoginAddr + 0x1970BC5) = 0x00;
			*(BYTE*)(LoginAddr + 0x1970BC6) = 0x3B;
			*(BYTE*)(LoginAddr + 0x1970BC7) = 0x45;
			*(BYTE*)(LoginAddr + 0x1970BC8) = 0x0C;
			*(BYTE*)(LoginAddr + 0x1970BC9) = 0x1B;
			*(BYTE*)(LoginAddr + 0x1970BCA) = 0xC0;
			*(BYTE*)(LoginAddr + 0x1970BCB) = 0x40;
			*(BYTE*)(LoginAddr + 0x1970BCC) = 0x5D;
			*(BYTE*)(LoginAddr + 0x1970BCD) = 0xC2;
			*(BYTE*)(LoginAddr + 0x1970BCE) = 0x0C;
			*(BYTE*)(LoginAddr + 0x1970BCF) = 0x00;
			*(BYTE*)(LoginAddr + 0x1970BD0) = 0x00;
		}
		offsetLogin = (LoginAddr + 0x1970BA4) - LoginAddr - 5;
		*(BYTE*)(LoginAddr) = 0xE9;
		*(DWORD*)(LoginAddr + 1) = ((DWORD)CherryLogin - LoginAddr + 0xBF2);
	}
	catch (...) {
		ConsoleOutput(L"\n", lred);
		return 1;
	}
	return 0;
}

bool PatternSearch2()
{
	LoginC = 0;
	if (iniLogin == 1)
	{
		ConsoleOutput(L"-로그인 화면 변경");
		CPatternSearch ptnLogin(Login);
		LoginAddr = ptnLogin.BytePatternSearch();

		if (CheckPatternSearchSuccess(LoginAddr))
		{
			::LoginC += 1;
		}
	}
	return 0;
}




BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);


	if (Process32First(hSnap, &pe32))
	{
		do
		{
			if (!_tcscmp(L"Mabinogi.exe", pe32.szExeFile))
			{
				CherryC++;
			}
			if (!_tcscmp(L"HSLaunch.exe", pe32.szExeFile))
			{
				CherryE++;
			}
		} while (Process32Next(hSnap, &pe32));
	}


	if (dwReason == DLL_PROCESS_ATTACH)
	{
		SECURITY_ATTRIBUTES SCAttrib;
		SCAttrib.bInheritHandle = NULL;
		SCAttrib.lpSecurityDescriptor = NULL;
		SCAttrib.nLength = sizeof(SCAttrib);
		hLThisInstance = hInstance;
		WCHAR fpath[256];
		wcscpy(fpath + GetSystemDirectoryW(fpath, 256), L"\\dsound.dll");
		hOriginalLibrary = LoadLibraryW(fpath);
		if (!hOriginalLibrary) return false;
		p[0] = GetProcAddress(hOriginalLibrary, "DirectSoundCaptureCreate");
		p[1] = GetProcAddress(hOriginalLibrary, "DirectSoundCaptureCreate8");
		p[2] = GetProcAddress(hOriginalLibrary, "DirectSoundCaptureEnumerateA");
		p[3] = GetProcAddress(hOriginalLibrary, "DirectSoundCaptureEnumerateW");
		p[4] = GetProcAddress(hOriginalLibrary, "DirectSoundCreate");
		p[5] = GetProcAddress(hOriginalLibrary, "DirectSoundCreate8");
		p[6] = GetProcAddress(hOriginalLibrary, "DirectSoundEnumerateA");
		p[7] = GetProcAddress(hOriginalLibrary, "DirectSoundEnumerateW");
		p[8] = GetProcAddress(hOriginalLibrary, "DirectSoundFullDuplexCreate");
		p[9] = GetProcAddress(hOriginalLibrary, "DllCanUnloadNow");
		p[10] = GetProcAddress(hOriginalLibrary, "DllGetClassObject");
		p[11] = GetProcAddress(hOriginalLibrary, "GetDeviceID");
	}


	if (dwReason == DLL_PROCESS_DETACH)
	{
		FreeLibrary(hOriginalLibrary);
	}


	BOOL WINAPI WritePrivateProfileString
		(
			__in  LPCTSTR lpAppName,
			__in  LPCTSTR lpKeyName,
			__in  LPCTSTR lpString,
			__in  LPCTSTR lpFileName
			);


	DWORD WINAPI GetPrivateProfileString
		(
			__in   LPCTSTR lpAppName,
			__in   LPCTSTR lpKeyName,
			__in   LPCTSTR lpDefault,
			__out  LPTSTR lpReturnedString,
			__in   DWORD nSize,
			__in   LPCTSTR lpFileName
			);


	UINT WINAPI GetPrivateProfileInt
		(
			__in  LPCTSTR lpAppName,
			__in  LPCTSTR lpKeyName,
			__in  INT nDefault,
			__in  LPCTSTR lpFileName
			);






	if (CherryC == 0)
	{																			//         ini 읽어오기 시작 ////////
		iniZoom = GetPrivateProfileInt(L"기능설정", L"ModifyZoomLimit", 0, L"./CherryNogi.ini");
		iniParty = GetPrivateProfileInt(L"기능설정", L"DisableCloseParty", 0, L"./CherryNogi.ini");
		iniAdv = GetPrivateProfileInt(L"기능설정", L"PartyToAdv", 0, L"./CherryNogi.ini");
		iniPower = GetPrivateProfileInt(L"기능설정", L"ShowCombatPower", 0, L"./CherryNogi.ini");
		iniZoomSpeed = GetPrivateProfileInt(L"기능설정", L"ZoomSpeed", 0, L"./CherryNogi.ini");
		iniLag = GetPrivateProfileInt(L"기능설정", L"SummonLaggingFix", 0, L"./CherryNogi.ini");
		iniAllNames = GetPrivateProfileInt(L"기능설정", L"AllNamesInVision", 0, L"./CherryNogi.ini");
		iniNameColoring = GetPrivateProfileInt(L"기능설정", L"EnableNameColoring", 0, L"./CherryNogi.ini");
		iniCookingNoise = GetPrivateProfileInt(L"기능설정", L"DisableCookingNoise", 0, L"./CherryNogi.ini");
		iniNighttime = GetPrivateProfileInt(L"기능설정", L"DisableNighttime", 0, L"./CherryNogi.ini");
		iniClock = GetPrivateProfileInt(L"기능설정", L"PreciseClock", 0, L"./CherryNogi.ini");
		iniAnyItem = GetPrivateProfileInt(L"기능설정", L"PutFxAnyItem", 0, L"./CherryNogi.ini");
		iniEgo = GetPrivateProfileInt(L"기능설정", L"TalkToUnequippedEgo", 0, L"./CherryNogi.ini");
		iniRobbery = GetPrivateProfileInt(L"기능설정", L"DisplayRobbery", 0, L"./CherryNogi.ini");
		iniPercentage = GetPrivateProfileInt(L"기능설정", L"DetailPercentage", 0, L"./CherryNogi.ini");
		iniStreetLamp = GetPrivateProfileInt(L"기능설정", L"OffTheStreetLamp", 0, L"./CherryNogi.ini");
		iniRightClick = GetPrivateProfileInt(L"기능설정", L"EnableSelfRightClick", 0, L"./CherryNogi.ini");
		iniMiniNogi = GetPrivateProfileInt(L"기능설정", L"MiniNogi", 0, L"./CherryNogi.ini");
		iniScrapBook = GetPrivateProfileInt(L"기능설정", L"ScrapBookStar", 0, L"./CherryNogi.ini");
		iniBlackList = GetPrivateProfileInt(L"기능설정", L"BlackList", 0, L"./CherryNogi.ini");
		iniitemPrice = GetPrivateProfileInt(L"기능설정", L"itemPrice", 0, L"./CherryNogi.ini");
		iniMinimapColor = GetPrivateProfileInt(L"기능설정", L"MinimapColoring", 0, L"./CherryNogi.ini");
		iniMimic = GetPrivateProfileInt(L"기능설정", L"Mimic", 0, L"./CherryNogi.ini");
		iniRWanted = GetPrivateProfileInt(L"기능설정", L"RWanted", 0, L"./CherryNogi.ini");
		iniChannel = GetPrivateProfileInt(L"기능설정", L"MoveToSameChannel", 0, L"./CherryNogi.ini");
		iniQuietPet = GetPrivateProfileInt(L"기능설정", L"QuietPet", 0, L"./CherryNogi.ini");
		iniLflying = GetPrivateProfileInt(L"기능설정", L"Lflying", 0, L"./CherryNogi.ini");
		iniAvon = GetPrivateProfileInt(L"기능설정", L"DisableCurtainEffect", 0, L"./CherryNogi.ini");
		iniZero = GetPrivateProfileInt(L"기능설정", L"FlyingAboveZero", 0, L"./CherryNogi.ini");
		iniTitle = GetPrivateProfileInt(L"기능설정", L"Disable2ndTitle", 0, L"./CherryNogi.ini");
		iniFrames = GetPrivateProfileInt(L"기능설정", L"DisableSimplifyingFrames", 0, L"./CherryNogi.ini");
		iniNosky = GetPrivateProfileInt(L"기능설정", L"DisableSky", 0, L"./CherryNogi.ini");
		iniLogin = GetPrivateProfileInt(L"기능설정", L"LoginScreenHook", 0, L"./CherryNogi.ini");
		iniTalk = GetPrivateProfileInt(L"기능설정", L"TalkSpeed", 0, L"./CherryNogi.ini");
		iniRec = GetPrivateProfileInt(L"기능설정", L"ExtendVideoTime", 0, L"./CherryNogi.ini");
		iniSleepTime = GetPrivateProfileInt(L"기능설정", L"SleepTime", 0, L"./CherryNogi.ini");
		iniPatch = GetPrivateProfileInt(L"기능설정", L"PatchMode", 0, L"./CherryNogi.ini");


		TCHAR* version = L"CherryNogi ver. 3.65 (by 블스체리)\n\n";
		TCHAR* version2 = L"CherryNogi ver. 3.65 (by 블스체리)";

		AllocConsole();
		SetConsoleTitle(version2);
		hConsoleW = GetStdHandle(STD_OUTPUT_HANDLE);
		hConsoleR = GetStdHandle(STD_INPUT_HANDLE);

		ConsoleOutput(version);
		if (CherryE == 0)
		{
			ConsoleOutput(L"[경고] 크랙실드가 작동중이 아닙니다. \n", lred);
			ConsoleOutput(L" - 정상적인 이용이 불가능할 수도 있습니다.. \n\n", lred);
			Sleep(2000);
		}

		if (iniPatch == 10011)
		{
			LoadLibrary(L"Cherry_Pake.dll");
			CherryPake = GetModuleHandleA("Cherry_Pake.dll");
		}

		ConsoleOutput(L"체리노기 적용을 시작합니다...\n");
		ConsoleOutput(L"체리노기의 설정은 CherryNogi.ini에서 가능합니다.\n\n\n");
		Sleep(1000);

		LoginC = 1;

		PatternSearch();
		Sleep(1000);
		PatchClient();
		ConsoleOutput(L"\n== 패치 완료 =========================\n\n\n");

		Sleep(iniSleepTime);

		FreeConsole();
		::CherryC += 1;
	}
	return 1;
}


extern "C" __declspec(naked) void __stdcall DirectSoundCaptureCreate()
{
	__asm
	{
		jmp p[0 * 4];
	}
}

extern "C" __declspec(naked) void __stdcall DirectSoundCaptureCreate8()
{
	__asm
	{
		jmp p[1 * 4];
	}
}

extern "C" __declspec(naked) void __stdcall DirectSoundCaptureEnumerateA()
{
	__asm
	{
		jmp p[2 * 4];
	}
}

extern "C" __declspec(naked) void __stdcall DirectSoundCaptureEnumerateW()
{
	__asm
	{
		jmp p[3 * 4];
	}
}

extern "C" __declspec(naked) void __stdcall DirectSoundCreate()
{
	__asm
	{
		jmp p[4 * 4];
	}
}

extern "C" __declspec(naked) void __stdcall DirectSoundCreate8()
{
	__asm
	{
		jmp p[5 * 4];
	}
}

extern "C" __declspec(naked) void __stdcall DirectSoundEnumerateA()
{
	__asm
	{
		jmp p[6 * 4];
	}
}

extern "C" __declspec(naked) void __stdcall DirectSoundEnumerateW()
{
	__asm
	{
		jmp p[7 * 4];
	}
}

extern "C" __declspec(naked) void __stdcall DirectSoundFullDuplexCreate()
{
	__asm
	{
		jmp p[8 * 4];
	}
}

extern "C" __declspec(naked) void __stdcall DllCanUnloadNow()
{
	__asm
	{
		jmp p[9 * 4];
	}
}

extern "C" __declspec(naked) void __stdcall DllGetClassObject()
{
	__asm
	{
		jmp p[10 * 4];
	}
}

extern "C" __declspec(naked) void __stdcall GetDeviceID()
{
	__asm
	{
		jmp p[11 * 4];
	}
}
