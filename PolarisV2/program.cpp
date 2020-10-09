#include "program.h"

#include <Windows.h>
#include <cstdio>

#include "util.h"
#include "table.h"
#include "idle_plate.h"

namespace polaris
{
	Table* mainTable;

	DWORD UpdateThread(LPVOID lpParam)
	{
		while (1)
		{
			mainTable->Update();
			Sleep(1000 / 60);
		}

		return 0;
	}

	Program::Program()
	{
		Util::InitConsole();

		printf("Initializing!\n");

		mainTable = new Table;
		mainTable->PushPlate(new IdlePlate);

		CreateThread(0, 0, UpdateThread, 0, 0, 0);
	}

	Program::~Program()
	{
		printf("Disposing!\n");
		
		mainTable->Pop();
	}
}
