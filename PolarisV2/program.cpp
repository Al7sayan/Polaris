#include "program.h"
#include "sdk_utils.h"
#include "base_plate.h"
#include "error_utils.h"
#include "ui_renderer.h"

#include <windows.h>
#include <cstdio>
#include <MinHook.h>
#pragma comment(lib, "libMinHook.x64.lib")

polaris::Program* gpProgram;

namespace polaris
{
	Program::Program()
	{
		if (gpProgram != nullptr || MH_Initialize() != MH_OK)
			ErrorUtils::ThrowException(L"Initializing failed. Please restart Fortnite and try again!");
		gpProgram = this;

		SDKUtils::InitConsole();
		std::cout << "Welcome to Polaris!" << std::endl;

		new UIRenderer;

		m_pMainTable = new PehTable;
		m_pMainTable->PushPlate(new BasePlate);
	}

	Program::~Program()
	{
		std::cout << "Disposing!" << std::endl;
		
		m_pMainTable->Pop();
	}
}
