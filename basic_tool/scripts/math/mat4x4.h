/**
 * @file mat4x4.h
 * @brief 4x4 行列クラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include <DirectXMath.h>

/**
 * @brief 4x4 行列クラス
 *
 * DirectXMath をラップし、ワールド・ビュー・プロジェクション行列の計算を提供する。
 * ファクトリメソッドで各種変換行列を生成できる。
 */
class Mat4x4
{
public:
    /** @brief 単位行列で初期化するデフォルトコンストラクタ */
    Mat4x4();
    ~Mat4x4();

    /** @brief 行列同士の積 */
    const Mat4x4 operator*(const Mat4x4& rhs) const;

    /** @brief 転置行列を返す */
    Mat4x4 Transpose() const;
    /** @brief 逆行列を返す */
    Mat4x4 Inverse() const;
    /**
     * @brief float[4][4] 配列に変換して出力する
     * @param out 出力先配列
     */
    void ToFloat4x4(float out[4][4]) const;
    /**
     * @brief 行列を位置・回転・スケールに分解して出力する
     * @param outPos      位置ベクトル
     * @param outRot      回転クォータニオン
     * @param outScale    スケールベクトル
     */
    void ToPosRotScale(Vec3& outPos, Vec4& outRot, Vec3& outScale) const;

    /** @brief 単位行列を生成する */
    static Mat4x4 Identity();
    /**
     * @brief スケール行列を生成する
     * @param x X スケール
     * @param y Y スケール
     * @param z Z スケール
     */
    static Mat4x4 Scale(float x, float y, float z);
    /**
     * @brief クォータニオンから回転行列を生成する
     * @param x クォータニオン X
     * @param y クォータニオン Y
     * @param z クォータニオン Z
     * @param w クォータニオン W
     */
    static Mat4x4 Rotation(float x, float y, float z, float w);
    /**
     * @brief 平行移動行列を生成する
     * @param x X 移動量
     * @param y Y 移動量
     * @param z Z 移動量
     */
    static Mat4x4 Translation(float x, float y, float z);
    /**
     * @brief 左手座標系の透視投影行列を生成する
     * @param fov    垂直視野角（ラジアン）
     * @param aspect アスペクト比
     * @param nearZ  ニアクリップ距離
     * @param farZ   ファークリップ距離
     */
    static Mat4x4 PerspectiveFovLH(float fov, float aspect, float nearZ, float farZ);

private:
    Mat4x4(const DirectX::XMMATRIX& m);
    DirectX::XMMATRIX mat;
};
