#pragma once

#include <Windows.h>
#include <string>
#include <cstdio>

namespace polaris
{
	class Util
	{
	public:
		static VOID InitConsole()
		{
			AllocConsole();

			FILE* pFile;
			freopen_s(&pFile, "CONOUT$", "w", stdout);
		}

		static std::string GetConcatPath(const std::string& sFirst, const std::string& sSecond)
		{
			std::string sTemp = sFirst;

			if (sFirst[sFirst.length()] != '\\')
			{
				sTemp += '\\';

				return(sTemp + sSecond);
			}
			else
				return(sFirst + sSecond);
		}

		static std::string GetFontsDir()
		{
			char cWinDir[MAX_PATH];
			if (!GetWindowsDirectoryA(cWinDir, sizeof(cWinDir)))
			{
				// TODO(Cyuubi): This should be seperated elsewhere, maybe an error handling class?
				MessageBoxA(NULL, "An unknown error has occured. Please relaunch Fortnite and try again!", "Error", MB_ICONERROR);
				ExitProcess(EXIT_FAILURE);
			}

			return GetConcatPath(cWinDir, "Fonts");
		}
	};
}
