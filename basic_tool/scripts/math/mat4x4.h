#pragma once
#include <DirectXMath.h>

class Mat4x4 
{
public:
    ~Mat4x4();

    Mat4x4 operator*(const Mat4x4& rhs) const;

    Mat4x4 Transpose() const;
    Mat4x4 Inverse() const;
    void ToFloat4x4(float out[4][4]) const;

    // ファクトリメソッド
    // Vecクラスには依存しない
    static Mat4x4 Identity();
    static Mat4x4 Scale(float x, float y, float z);
    static Mat4x4 Rotation(float x, float y, float z, float w);
    static Mat4x4 Translation(float x, float y, float z);
    static Mat4x4 PerspectiveFovLH(float fov, float aspect, float nearZ, float farZ);

private:
    Mat4x4() = delete; // 必ずファクトリメソッドでインスタンスを生成
    Mat4x4(const DirectX::XMMATRIX& m);
    DirectX::XMMATRIX mat;
};