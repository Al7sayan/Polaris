#include "program.h"
#include "util.h"
#include "base_plate.h"

#include <windows.h>
#include <cstdio>
#include <MinHook.h>
#pragma comment(lib, "libMinHook.x64.lib")

polaris::Program* gpProgram;

namespace polaris
{
	// NOTE: We'd be better doing this off in a separate thread
	// to avoid lag on the game thread.
	DWORD UpdateThread(LPVOID lpParam)
	{
		while (1)
		{
			gpProgram->m_pMainTable->Update();
			Sleep(1000 / 60);
		}

		return 0;
	}

	Program::Program()
	{
		if (gpProgram != nullptr || MH_Initialize() != MH_OK)
		{
			MessageBoxA(NULL, "An unknown error has occured. Please relaunch Fortnite and try again!", "Error", MB_ICONERROR);
			ExitProcess(EXIT_FAILURE);
		}
		gpProgram = this;

		Util::InitConsole();
		std::cout << "Welcome to Polaris!" << std::endl;

		m_pMainTable = new PehTable;
		m_pMainTable->PushPlate(new BasePlate);

		CreateThread(0, 0, UpdateThread, 0, 0, 0);
	}

	Program::~Program()
	{
		std::cout << "Disposing!" << std::endl;
		
		m_pMainTable->Pop();
	}
}
