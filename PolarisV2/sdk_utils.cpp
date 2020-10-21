#include "sdk_utils.h"
#include "error_utils.h"

namespace polaris
{
	namespace utilities
	{
		BOOL SDKUtils::MaskCompare(PVOID pBuffer, LPCSTR lpPattern, LPCSTR lpMask)
		{
			for (auto value = static_cast<PBYTE>(pBuffer); *lpMask; ++lpPattern, ++lpMask, ++value)
			{
				if (*lpMask == 'x' && *reinterpret_cast<LPCBYTE>(lpPattern) != *value)
					return false;
			}

			return true;
		}
		PBYTE SDKUtils::FindPattern(PVOID pBase, DWORD dwSize, LPCSTR lpPattern, LPCSTR lpMask)
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

		uintptr_t SDKUtils::BaseAddress()
		{
			return reinterpret_cast<uintptr_t>(GetModuleHandle(0));
		}
		PBYTE SDKUtils::FindPattern(LPCSTR lpPattern, LPCSTR lpMask)
		{
			MODULEINFO info = { 0 };

			GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));

			PBYTE result = SDKUtils::FindPattern(info.lpBaseOfDll, info.SizeOfImage, lpPattern, lpMask);
			if (!result)
				ErrorUtils::ThrowException(L"Pattern was not found.");

			return result;
		}
		SDK::AActor* SDKUtils::FindActor(SDK::UClass* pClass, int iSkip)
		{
			for (int i = 0, j = 0; i < globals::gpActors->Num(); i++)
			{
				SDK::AActor* pActor = globals::gpActors->operator[](i);

				if (pActor != nullptr)
				{
					if (pActor->IsA(pClass))
					{
						if (j >= iSkip)
							return pActor;
						else
						{
							j++;
							continue;
						}
					}
				}
			}

			return nullptr;
		}

		VOID SDKUtils::InitConsole()
		{
			AllocConsole();

			FILE* pFile;
			freopen_s(&pFile, "CONOUT$", "w", stdout);
		}
		VOID SDKUtils::InitSdk()
		{
			uint64_t GEngineAddr = reinterpret_cast<uint64_t>(SDKUtils::FindPattern
			(
				"\x48\x89\x74\x24\x20\xE8\x00\x00\x00\x00\x48\x8B\x4C\x24\x40\x48\x89\x05",
				"xxxxxx????xxxxxxxx"
			));
			auto GEngine = (SDK::UEngine**)(GEngineAddr + 22 + *(int32_t*)(GEngineAddr + 18));

			globals::gpWorld = &(*GEngine)->GameViewport->World;
		}
		VOID SDKUtils::InitGlobals()
		{
			globals::gpLevel = (*globals::gpWorld)->PersistentLevel;
			globals::gpGameInstance = (*globals::gpWorld)->OwningGameInstance;
			globals::gpLocalPlayers = globals::gpGameInstance->LocalPlayers;
			globals::gpLocalPlayer = globals::gpLocalPlayers[0];
			globals::gpPlayerController = globals::gpLocalPlayer->PlayerController;
		}
		VOID SDKUtils::InitPatches()
		{
		}
	}
}