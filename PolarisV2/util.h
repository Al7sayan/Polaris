#pragma once

#include "SDK.hpp"
#include "globals.h"

#include <inttypes.h>
#include <Windows.h>
#include <psapi.h>
#include <stdio.h>
#include <iostream>

namespace polaris
{
	class Util
	{
	private:
		static BOOL MaskCompare(PVOID pBuffer, LPCSTR lpPattern, LPCSTR lpMask)
		{
			for (auto value = static_cast<PBYTE>(pBuffer); *lpMask; ++lpPattern, ++lpMask, ++value)
			{
				if (*lpMask == 'x' && *reinterpret_cast<LPCBYTE>(lpPattern) != *value)
					return false;
			}

			return true;
		}

		static PBYTE FindPattern(PVOID pBase, DWORD dwSize, LPCSTR lpPattern, LPCSTR lpMask)
		{
			dwSize -= static_cast<DWORD>(strlen(lpMask));

			for (auto i = 0UL; i < dwSize; ++i)
			{
				auto pAddress = static_cast<PBYTE>(pBase) + i;

				if (MaskCompare(pAddress, lpPattern, lpMask))
					return pAddress;
			}

			return NULL;
		}

	public:
		static uintptr_t BaseAddress()
		{
			return reinterpret_cast<uintptr_t>(GetModuleHandle(0));
		}

		static PBYTE FindPattern(LPCSTR lpPattern, LPCSTR lpMask)
		{
			MODULEINFO info = { 0 };

			GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));

			return Util::FindPattern(info.lpBaseOfDll, info.SizeOfImage, lpPattern, lpMask);
		}

		static VOID InitConsole()
		{
			AllocConsole();

			FILE* pFile;
			freopen_s(&pFile, "CONOUT$", "w", stdout);
		}

		static std::string GetConcatPath(const std::string& sFirst, const std::string& sSecond)
		{
			std::string sTemp = sFirst;

			if (sFirst[sFirst.length()] != '\\')
			{
				sTemp += '\\';

				return(sTemp + sSecond);
			}
			else
				return(sFirst + sSecond);
		}

		static std::string GetFontsDir()
		{
			char cWinDir[MAX_PATH];
			if (!GetWindowsDirectoryA(cWinDir, sizeof(cWinDir)))
			{
				// TODO(Cyuubi): This should be seperated elsewhere, maybe an error handling class?
				MessageBoxA(NULL, "An unknown error has occured. Please relaunch Fortnite and try again!", "Error", MB_ICONERROR);
				ExitProcess(EXIT_FAILURE);
			}

			return GetConcatPath(cWinDir, "Fonts");
		}

		static VOID InitSdk()
		{
			auto pUWorldAddress = Util::FindPattern("\x48\x8B\x1D\x00\x00\x00\x00\x00\x00\x00\x10\x4C\x8D\x4D\x00\x4C", "xxx???????xxxx?x");
			if (!pUWorldAddress)
			{
				MessageBoxA(NULL, "An unknown error has occured. Please relaunch Fortnite and try again!", "Error", MB_ICONERROR);
				ExitProcess(EXIT_FAILURE);
			}

			auto pUWorldOffset = *reinterpret_cast<uint32_t*>(pUWorldAddress + 3);

			Globals::gpWorld = reinterpret_cast<SDK::UWorld**>(pUWorldAddress + 7 + pUWorldOffset);

			auto pGObjectAddress = Util::FindPattern("\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\xD6", "xxx????x????x????x????xxx");
			if (!pGObjectAddress)
			{
				MessageBoxA(NULL, "An unknown error has occured. Please relaunch Fortnite and try again!", "Error", MB_ICONERROR);
				ExitProcess(EXIT_FAILURE);
			}

			auto pGObjectOffset = *reinterpret_cast<uint32_t*>(pGObjectAddress + 3);

			SDK::UObject::GObjects = reinterpret_cast<SDK::FUObjectArray*>(pGObjectAddress + 7 + pGObjectOffset);

			auto pGNameAddress = Util::FindPattern("\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x75\x50\xB9\x00\x00\x00\x00\x48\x89\x5C\x24", "xxx????xxxxxx????xxxx");
			if (!pGNameAddress)
			{
				MessageBoxA(NULL, "An unknown error has occured. Please relaunch Fortnite and try again!", "Error", MB_ICONERROR);
				ExitProcess(EXIT_FAILURE);
			}

			auto pGNameOffset = *reinterpret_cast<uint32_t*>(pGNameAddress + 3);

			SDK::FName::GNames = *reinterpret_cast<SDK::TNameEntryArray**>(pGNameAddress + 7 + pGNameOffset);
		}
	};
}
