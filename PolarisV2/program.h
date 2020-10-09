#pragma once

#include "table.h"

namespace polaris
{
	class Program
	{
	public:
		Table* m_pMainTable;

		Program();
		~Program();
	};
}

extern polaris::Program* gpProgram;
