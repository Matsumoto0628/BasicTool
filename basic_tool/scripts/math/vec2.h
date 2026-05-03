/**
 * @file vec2.h
 * @brief 2次元ベクトルクラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include <DirectXMath.h>

/**
 * @brief 2成分ベクトルクラス
 *
 * DirectXMath をラップし、SIMD 演算を維持しながら 2D ベクトル演算を提供する。
 */
class Vec2
{
public:
    /** @brief デフォルトコンストラクタ（全要素 0 で初期化） */
    Vec2();
    ~Vec2();
    /**
     * @brief 各成分を指定するコンストラクタ
     * @param x X 成分
     * @param y Y 成分
     */
    Vec2(float x, float y);

    /** @brief ベクトルの加算 */
    const Vec2 operator+(const Vec2& rhs) const;
    /** @brief ベクトルの減算 */
    const Vec2 operator-(const Vec2& rhs) const;
    /** @brief スカラー乗算 */
    const Vec2 operator*(float s) const;

    /** @brief X 成分を返す */
    float X() const;
    /** @brief Y 成分を返す */
    float Y() const;
    /** @brief ベクトルの長さ（ノルム）を返す */
    float Length() const;
    /** @brief 正規化したベクトルを返す */
    Vec2 Normalize() const;
    /**
     * @brief float 配列に変換して出力する
     * @param out 出力先配列（要素数 2 以上）
     */
    void ToFloat2(float out[2]) const;

private:
    Vec2(const DirectX::XMVECTOR& v);
    DirectX::XMVECTOR vec;
};
