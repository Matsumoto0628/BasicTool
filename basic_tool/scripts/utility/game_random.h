/**
 * @file game_random.h
 * @brief 乱数生成ユーティリティクラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include <random>

/**
 * @brief 乱数生成と UUID 発行を行う静的ユーティリティクラス
 *
 * メルセンヌ・ツイスタを内部で保持し、整数・浮動小数点・UUID を生成する。
 */
class GameRandom
{
public:
    /**
     * @brief 整数の乱数を範囲 [min, max] で返す
     * @param min 最小値（含む）
     * @param max 最大値（含む）
     */
    static int GetRange(int min, int max);
    /**
     * @brief 浮動小数点の乱数を範囲 [min, max] で返す
     * @param min 最小値
     * @param max 最大値
     */
    static float GetRange(float min, float max);
    /** @brief 64 ビットの UUID（一意な識別子）を生成して返す */
    static uint64_t GetUUID();

private:
    GameRandom() = delete;
    ~GameRandom() = delete;
    static std::mt19937    s_rng;     ///< 乱数エンジン（メルセンヌ・ツイスタ 32 ビット）
    static std::mt19937_64 s_rngUUID; ///< UUID 用乱数エンジン（メルセンヌ・ツイスタ 64 ビット）
};
