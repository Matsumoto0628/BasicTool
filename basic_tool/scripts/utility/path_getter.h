/**
 * @file path_getter.h
 * @brief ファイルパス取得ユーティリティ
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include <filesystem>
#include <window.h>

/**
 * @brief ファイルパス取得に関するユーティリティ関数をまとめた名前空間
 */
namespace PathGetter
{
    /**
     * @brief 実行ファイルが存在するディレクトリのパスを返す
     * @return 実行ファイルのディレクトリパス
     */
    static std::filesystem::path GetExeDirectory()
    {
        wchar_t path[MAX_PATH];
        GetModuleFileNameW(nullptr, path, MAX_PATH);

        return std::filesystem::path(path).parent_path();
    }
}
