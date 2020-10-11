#include "directory_utils.h"
#include "error_utils.h"
#include "sdk_utils.h"

#include <windows.h>

namespace polaris
{
    std::string DirectoryUtils::GetFontsDir()
    {
        char winDir[MAX_PATH];
        if (!GetWindowsDirectoryA(winDir, sizeof(winDir)))
            ErrorUtils::ThrowException(L"An unknown platform exception occured.");

        return SDKUtils::GetConcatPath(winDir, "Fonts");
    }
}