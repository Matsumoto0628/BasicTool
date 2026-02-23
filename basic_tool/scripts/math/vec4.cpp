#include "vec4.h"
Vec4::Vec4()
    : vec{ DirectX::XMVectorZero() }
{
}

Vec4::~Vec4() 
{
}

Vec4::Vec4(float x, float y, float z, float w)
    : vec(DirectX::XMVectorSet(x, y, z, w))
{
}

Vec4::Vec4(const DirectX::XMVECTOR& v)
    : vec(v)
{
}

const Vec4 Vec4::operator+(const Vec4& rhs) const 
{
    return Vec4(DirectX::XMVectorAdd(vec, rhs.vec));
}

const Vec4 Vec4::operator-(const Vec4& rhs) const 
{
    return Vec4(DirectX::XMVectorSubtract(vec, rhs.vec));
}

const Vec4 Vec4::operator*(float s) const 
{
    return Vec4(DirectX::XMVectorScale(vec, s));
}

float Vec4::X() const 
{ 
    return DirectX::XMVectorGetX(vec); 
}

float Vec4::Y() const 
{ 
    return DirectX::XMVectorGetY(vec); 
}

float Vec4::Z() const 
{ 
    return DirectX::XMVectorGetZ(vec); 
}

float Vec4::W() const 
{ 
    return DirectX::XMVectorGetW(vec); 
}

float Vec4::Length() const 
{
    return DirectX::XMVectorGetX(DirectX::XMVector4Length(vec));
}

Vec4 Vec4::Normalize() const 
{
    return Vec4(DirectX::XMVector4Normalize(vec));
}

// クォータニオン用の関数
// Quaternion専用のクラスを作るべきかも
Vec4 Vec4::MultiplyQuaternion(const Vec4& rhs) const
{
    return Vec4(DirectX::XMQuaternionMultiply(vec, rhs.vec));
}

Vec3 Vec4::RotateVec3(const Vec3& rhs) const
{
    DirectX::XMVECTOR target = DirectX::XMVectorSet(rhs.X(), rhs.Y(), rhs.Z(), 0.f);
    DirectX::XMVECTOR rotated = DirectX::XMVector3Rotate(target, vec);
    return Vec3(
        DirectX::XMVectorGetX(rotated),
        DirectX::XMVectorGetY(rotated),
        DirectX::XMVectorGetZ(rotated)
    );
}

void Vec4::ToFloat4(float out[4]) const
{
    DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(out), vec);
}