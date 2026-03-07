#pragma once
#include <DirectXMath.h>

// 定数
const float PI = 3.14159265358979323846f;

inline float DegToRad(float deg) { return deg * PI / 180.0f; }
inline float RadToDeg(float rad) { return rad * 180.0f / PI; }

inline Vec4 EulerToQuaternion(const Vec3& euler)
{
    DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYaw(
        euler.X(),
        euler.Y(),
        euler.Z()
    );

    return Vec4(
        DirectX::XMVectorGetX(quat),
        DirectX::XMVectorGetY(quat),
        DirectX::XMVectorGetZ(quat),
        DirectX::XMVectorGetW(quat)
    );
}

inline Vec4 AxisToQuaternion(const Vec3& axis, float rad)
{
    DirectX::XMVECTOR axisVec = DirectX::XMVectorSet(
        axis.X(),
        axis.Y(),
        axis.Z(),
        0.0f
    );

    DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationAxis(axisVec, rad);

    return Vec4(
        DirectX::XMVectorGetX(quat),
        DirectX::XMVectorGetY(quat),
        DirectX::XMVectorGetZ(quat),
        DirectX::XMVectorGetW(quat)
    );
}

inline Vec3 QuaternionToEuler(const Vec4& q)
{
    DirectX::XMVECTOR quat = DirectX::XMVectorSet(q.X(), q.Y(), q.Z(), q.W());

    // 回転行列に変換
    DirectX::XMMATRIX m = DirectX::XMMatrixRotationQuaternion(quat);

    float pitch, yaw, roll;

    // LH座標系用の抽出
    pitch = asinf(-DirectX::XMVectorGetZ(m.r[1]));               // X回転
    yaw = atan2f(DirectX::XMVectorGetZ(m.r[0]), DirectX::XMVectorGetZ(m.r[2])); // Y回転
    roll = atan2f(DirectX::XMVectorGetX(m.r[1]), DirectX::XMVectorGetY(m.r[1])); // Z回転

    return Vec3(pitch, yaw, roll);
}

inline Vec4 LookRotation(const Vec3& forward, const Vec3& up)
{
    DirectX::XMVECTOR f = DirectX::XMVectorSet(forward.X(), forward.Y(), forward.Z(), 0);
    DirectX::XMVECTOR u = DirectX::XMVectorSet(up.X(), up.Y(), up.Z(), 0);

    DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(
        DirectX::XMVectorZero(), // eye
        f,                       // forward
        u                        // up
    );

    DirectX::XMMATRIX rot = DirectX::XMMatrixInverse(nullptr, view);
    DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationMatrix(rot);

    return Vec4(
        DirectX::XMVectorGetX(quat),
        DirectX::XMVectorGetY(quat),
        DirectX::XMVectorGetZ(quat),
        DirectX::XMVectorGetW(quat)
    );
}