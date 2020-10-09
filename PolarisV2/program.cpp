#include "program.h"

#include <Windows.h>
#include <cstdio>

#include "util.h"

namespace polaris
{
	Program::Program()
	{
		Util::InitConsole();

		printf("Initializing!\n");
	}

	Program::~Program()
	{
		printf("Disposing!\n");
	}
}
