#pragma once
#include <filesystem>
#include <window.h>

namespace PathGetter 
{
    static std::filesystem::path GetExeDirectory()
    {
        wchar_t path[MAX_PATH];
        GetModuleFileNameW(nullptr, path, MAX_PATH);

        return std::filesystem::path(path).parent_path();
    }
}