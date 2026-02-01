#include "vec2.h"

Vec2::Vec2()
    : vec(DirectX::XMVectorZero())
{
}

Vec2::~Vec2() 
{
}

Vec2::Vec2(float x, float y)
    : vec(DirectX::XMVectorSet(x, y, 0.0f, 0.0f))
{
}

Vec2::Vec2(const DirectX::XMVECTOR& v)
    : vec(v)
{
}

Vec2 Vec2::operator+(const Vec2& rhs) const 
{
    return Vec2(DirectX::XMVectorAdd(vec, rhs.vec));
}

Vec2 Vec2::operator-(const Vec2& rhs) const 
{
    return Vec2(DirectX::XMVectorSubtract(vec, rhs.vec));
}

Vec2 Vec2::operator*(float s) const 
{
    return Vec2(DirectX::XMVectorScale(vec, s));
}

float Vec2::X() const 
{ 
    return DirectX::XMVectorGetX(vec); 
}

float Vec2::Y() const 
{ 
    return DirectX::XMVectorGetY(vec); 
}

float Vec2::Length() const 
{
    return DirectX::XMVectorGetX(DirectX::XMVector2Length(vec));
}

Vec2 Vec2::Normalize() const 
{
    return Vec2(DirectX::XMVector2Normalize(vec));
}

void Vec2::ToFloat2(float out[2]) const
{
    DirectX::XMStoreFloat2(reinterpret_cast<DirectX::XMFLOAT2*>(out), vec);
}