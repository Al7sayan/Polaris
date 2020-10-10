#ifndef GLOBALS_H
#define GLOBALS_H

#include "SDK.hpp"

namespace polaris
{
	extern SDK::UWorld** gpWorld;
	extern SDK::ULevel* gpLevel;
	extern SDK::TArray<SDK::AActor*>* gpActors;

	extern SDK::UGameInstance* gpGameInstance;

	extern SDK::TArray<SDK::ULocalPlayer*> gpLocalPlayers;
	extern SDK::ULocalPlayer* gpLocalPlayer;

	extern SDK::APlayerController* gpPlayerController;
};

#endif // !GLOBALS_H