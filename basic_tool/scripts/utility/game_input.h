/**
 * @file game_input.h
 * @brief キーボード入力管理クラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once

/**
 * @brief キーボード入力の状態を管理する静的ユーティリティクラス
 *
 * 毎フレーム Update() を呼び出すことで、押した瞬間・離した瞬間・押し続けを判定できる。
 */
class GameInput
{
public:
    /**
     * @brief キーが押された瞬間かどうかを返す
     * @param key 仮想キーコード
     */
    static bool GetKeyDown(int key);
    /**
     * @brief キーが離された瞬間かどうかを返す
     * @param key 仮想キーコード
     */
    static bool GetKeyUp(int key);
    /**
     * @brief キーが押し続けられているかどうかを返す
     * @param key 仮想キーコード
     */
    static bool GetKey(int key);
    /** @brief フレームごとにキー状態を更新する（毎フレーム呼び出し必須） */
    static void Update();

private:
    GameInput() = delete;
    ~GameInput() = delete;
    static const int KEY_COUNT = 256;      ///< 管理するキー数
    static bool s_prevKeys[KEY_COUNT];     ///< 前フレームのキー状態
    static bool s_currentKeys[KEY_COUNT];  ///< 現フレームのキー状態
};
