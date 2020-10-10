#ifndef SDK_UTILS_H
#define SDK_UTILS_H

#include "SDK.hpp"
#include "globals.h"

#include <inttypes.h>
#include <Windows.h>
#include <psapi.h>
#include <stdio.h>
#include <iostream>

namespace polaris
{
	//!  Utilities for the SDK.
	/*!
	* Serves as easy access to many SDK utilities like finding a pattern.
	*/
	class SDKUtils
	{
	private:
		static BOOL MaskCompare(PVOID pBuffer, LPCSTR lpPattern, LPCSTR lpMask);
		static PBYTE FindPattern(PVOID pBase, DWORD dwSize, LPCSTR lpPattern, LPCSTR lpMask);

	public:
		static uintptr_t BaseAddress(); //!< BaseAddress returns the base offset of the game.
		static PBYTE FindPattern(LPCSTR lpPattern, LPCSTR lpMask); //!< FindPattern finds a pattern in the game's EXE.
		static SDK::AActor* FindActor(SDK::UClass* pClass, int iSkip = 0);

		static VOID InitConsole(); //!< InitConsole initializes a new Console window if it doesn't exist yet.
		static VOID InitSdk(); //!< InitSdk loads the SDK's properties so we can actually do stuff.
		static VOID InitGlobals(); //!< InitGlobals puts the SDK's properties in globals so we can do stuff easier.

		static std::string GetConcatPath(const std::string& sFirst, const std::string& sSecond); //!< GetConcatPath gets the Concat path.
		static std::string GetFontsDir(); //!< GetFontsDir finds and returns the path of the Windows Font directory.
	};
}

#endif // !SDK_UTILS_H
