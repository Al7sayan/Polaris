#include "program.h"

#include <Windows.h>
#include <cstdio>

#include "util.h"
#include "base_plate.h"

polaris::Program* gpProgram;

namespace polaris
{
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
		Util::InitConsole();

		std::cout << "Welcome to Polaris!" << std::endl;

		m_pMainTable = new Table;
		m_pMainTable->PushPlate(new BasePlate);

		CreateThread(0, 0, UpdateThread, 0, 0, 0);
	}

	Program::~Program()
	{
		std::cout << "Disposing!" << std::endl;
		
		m_pMainTable->Pop();
	}
}
