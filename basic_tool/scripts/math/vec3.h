/**
 * @file vec3.h
 * @brief 3次元ベクトルクラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include <DirectXMath.h>
#include <nlohmann/json.hpp>
using Json = nlohmann::ordered_json;

/**
 * @brief 3成分ベクトルクラス
 *
 * DirectXMath をラップすることで、SIMD を維持して高速に計算する。
 * nlohmann/json によるシリアライズ／デシリアライズにも対応している。
 */
class Vec3
{
public:
    /** @brief デフォルトコンストラクタ（全要素 0 で初期化） */
    Vec3();
    ~Vec3();
    /**
     * @brief 各成分を指定するコンストラクタ
     * @param x X 成分
     * @param y Y 成分
     * @param z Z 成分
     */
    Vec3(float x, float y, float z);

    /** @brief ベクトルの加算 */
    const Vec3 operator+(const Vec3& rhs) const;
    /** @brief ベクトルの加算代入 */
    const Vec3& operator+=(const Vec3& rhs);
    /** @brief ベクトルの減算 */
    const Vec3 operator-(const Vec3& rhs) const;
    /** @brief ベクトルの減算代入 */
    const Vec3& operator-=(const Vec3& rhs);
    /** @brief スカラー乗算 */
    const Vec3 operator*(float s) const;
    /** @brief スカラー乗算代入 */
    const Vec3& operator*=(float s);
    /** @brief スカラー除算 */
    const Vec3 operator/(float s) const;
    /** @brief スカラー除算代入 */
    const Vec3& operator/=(float s);

    /** @brief X 成分を返す */
    float X() const;
    /** @brief Y 成分を返す */
    float Y() const;
    /** @brief Z 成分を返す */
    float Z() const;
    /** @brief ベクトルの長さ（ノルム）を返す */
    float Length() const;
    /** @brief 正規化したベクトルを返す */
    Vec3 Normalize() const;
    /**
     * @brief float 配列に変換して出力する
     * @param out 出力先配列（要素数 3 以上）
     */
    void ToFloat3(float out[3]) const;

private:
    Vec3(const DirectX::XMVECTOR& v); // DirectXMathのコンストラクタは自分だけ使う
    DirectX::XMVECTOR vec;
};

/** @brief Vec3 を JSON にシリアライズする */
inline void to_json(Json& j, const Vec3& v)
{
    j = { v.X(), v.Y(), v.Z()};
}

/** @brief JSON から Vec3 にデシリアライズする */
inline void from_json(const Json& j, Vec3& v)
{
    float x = j.at(0).get<float>();
    float y = j.at(1).get<float>();
    float z = j.at(2).get<float>();

    v = Vec3(x, y, z);
}
