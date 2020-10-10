#include "globals.h"

namespace polaris
{
	SDK::UWorld** gpWorld;
	SDK::ULevel* gpLevel;
	SDK::TArray<SDK::AActor*>* gpActors;

	SDK::UGameInstance* gpGameInstance;

	SDK::TArray<SDK::ULocalPlayer*> gpLocalPlayers;
	SDK::ULocalPlayer* gpLocalPlayer;

	SDK::APlayerController* gpPlayerController;
}