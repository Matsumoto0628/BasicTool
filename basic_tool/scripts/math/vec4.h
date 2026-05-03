/**
 * @file vec4.h
 * @brief 4次元ベクトル／クォータニオンクラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include <DirectXMath.h>
#include <nlohmann/json.hpp>
using Json = nlohmann::ordered_json;

/**
 * @brief 4成分ベクトルクラス
 *
 * 位置の同次座標や、クォータニオンによる回転表現として使用する。
 * nlohmann/json によるシリアライズ／デシリアライズにも対応している。
 */
class Vec4
{
public:
    /** @brief デフォルトコンストラクタ（全要素 0 で初期化） */
    Vec4();
    ~Vec4();
    /**
     * @brief 各成分を指定するコンストラクタ
     * @param x X 成分
     * @param y Y 成分
     * @param z Z 成分
     * @param w W 成分
     */
    Vec4(float x, float y, float z, float w);

    /** @brief ベクトルの加算 */
    const Vec4 operator+(const Vec4& rhs) const;
    /** @brief ベクトルの減算 */
    const Vec4 operator-(const Vec4& rhs) const;
    /** @brief スカラー乗算 */
    const Vec4 operator*(float s) const;

    /** @brief X 成分を返す */
    float X() const;
    /** @brief Y 成分を返す */
    float Y() const;
    /** @brief Z 成分を返す */
    float Z() const;
    /** @brief W 成分を返す */
    float W() const;
    /** @brief ベクトルの長さ（ノルム）を返す */
    float Length() const;
    /** @brief 正規化したベクトルを返す */
    Vec4 Normalize() const;
    /**
     * @brief クォータニオン同士の積（合成回転）を返す
     * @param rhs 右辺のクォータニオン
     */
    Vec4 MultiplyQuaternion(const Vec4& rhs) const;
    /**
     * @brief クォータニオンで Vec3 を回転させた結果を返す
     * @param rhs 回転させるベクトル
     */
    Vec3 RotateVec3(const Vec3& rhs) const;
    /**
     * @brief float 配列に変換して出力する
     * @param out 出力先配列（要素数 4 以上）
     */
    void ToFloat4(float out[4]) const;

private:
    Vec4(const DirectX::XMVECTOR& v);
    DirectX::XMVECTOR vec;
};

/** @brief Vec4 を JSON にシリアライズする */
inline void to_json(Json& j, const Vec4& v)
{
    j = { v.X(), v.Y(), v.Z(), v.W()};
}

/** @brief JSON から Vec4 にデシリアライズする */
inline void from_json(const Json& j, Vec4& v)
{
    float x = j.at(0).get<float>();
    float y = j.at(1).get<float>();
    float z = j.at(2).get<float>();
    float w = j.at(3).get<float>();

    v = Vec4(x, y, z, w);
}
