#include <DirectXMath.h>

// 定数
static const float PI = 3.14159265358979323846f;

float DegToRad(float deg)
{
    return deg * PI / 180.0f;
}

float RadToDeg(float rad)
{
    return rad * 180.0f / PI;
}

Vec4 EulerToQuaternion(const Vec3& euler)
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

Vec4 AxisToQuaternion(const Vec3& axis, float rad)
{
    DirectX::XMVECTOR axisVec = DirectX::XMVectorSet(
        axis.X(),
        axis.Y(),
        axis.Y(),
        0.0f // 0.0fはベクトルを表す(1.0fはポイント)
    ); 

    DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationAxis(axisVec, rad);

    return Vec4(
        DirectX::XMVectorGetX(quat),
        DirectX::XMVectorGetY(quat),
        DirectX::XMVectorGetZ(quat),
        DirectX::XMVectorGetW(quat)
    );
}