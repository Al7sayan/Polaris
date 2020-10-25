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
	namespace utilities
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
			static SDK::AActor* FindActor(SDK::UClass* pClass, int iSkip = 0); //!< FindActor returns an actor by its class.
			static VOID InitConsole(); //!< InitConsole initializes a new Console window if it doesn't exist yet.
			static VOID InitSdk(); //!< InitSdk loads the SDK's properties.
			static VOID InitGlobals(); //!< InitGlobals puts the SDK's properties in globals.
			static VOID InitPatches(); //!< InitPatches patches Fortnite to circumvent errors.
		};
	}
}

#endif // !SDK_UTILS_H
