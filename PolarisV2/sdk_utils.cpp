#include "sdk_utils.h"
#include "error_utils.h"

namespace polaris
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
		if(!result)
			ErrorUtils::ThrowException(L"Pattern was not found.");

		return result;
	}
	SDK::AActor* SDKUtils::FindActor(SDK::UClass* pClass, int iSkip)
	{
		for (int i = 0, j = 0; i < gpActors->Num(); i++)
		{
			SDK::AActor* pActor = gpActors->operator[](i);

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

				globals::gpLocalPlayer->ViewportClient->ViewportConsole = pConsole;
			}
			else
			{
				// redirect the console to a nullptr if bIsEnabled already exists
				globals::gpLocalPlayer->ViewportClient->ViewportConsole = nullptr;
			}
		}
		VOID SDKUtils::InitSdk()
		{
			globals::gpWorld = reinterpret_cast<SDK::UWorld**>(BaseAddress() + 0x4740328); // cant use SDK::GWorld, because its not in a header.
		}
	}

	std::string SDKUtils::GetConcatPath(const std::string& sFirst, const std::string& sSecond)
	{
		std::string sTemp = sFirst;

				DWORD dwTemp;
				VirtualProtect(pAbilityPatchAddress, 16, dwProtection, &dwTemp);
			}
		}
		else
			return(sFirst + sSecond);
	}
	std::string SDKUtils::GetFontsDir()
	{
		char cWinDir[MAX_PATH];
		if (!GetWindowsDirectoryA(cWinDir, sizeof(cWinDir)))
			ErrorUtils::ThrowException(L"No additional info was found for the exception.");

		return GetConcatPath(cWinDir, "Fonts");
	}
}