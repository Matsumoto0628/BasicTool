/**
 * @file game_time.h
 * @brief フレーム時間管理クラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include <chrono>

/**
 * @brief フレーム間の経過時間（デルタタイム）を管理する静的ユーティリティクラス
 *
 * 毎フレーム Update() を呼び出すことで GetDeltaTime() が最新値を返す。
 */
class GameTime
{
    using Clock     = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;

public:
    /** @brief フレームごとにデルタタイムを更新する（毎フレーム呼び出し必須） */
    static void Update();
    /** @brief 直前フレームからの経過時間を秒単位で返す */
    static float GetDeltaTime() { return s_deltaTime; }

private:
    GameTime() = delete;
    ~GameTime() = delete;
    static TimePoint s_prevTime;  ///< 前フレームの時刻
    static float     s_deltaTime; ///< フレーム間経過時間（秒）
};
