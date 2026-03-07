#pragma once
#include <DirectXMath.h>
#include <nlohmann/json.hpp>
using Json = nlohmann::ordered_json;

// 4成分ベクトル
class Vec4 
{
public:
    Vec4();
    ~Vec4();
    Vec4(float x, float y, float z, float w);

    const Vec4 operator+(const Vec4& rhs) const;
    const Vec4 operator-(const Vec4& rhs) const;
    const Vec4 operator*(float s) const;
    
    float X() const;
    float Y() const;
    float Z() const;
    float W() const;
    float Length() const;
    Vec4 Normalize() const;
    Vec4 MultiplyQuaternion(const Vec4& rhs) const;
    Vec3 RotateVec3(const Vec3& rhs) const;
    void ToFloat4(float out[4]) const;

private:
    Vec4(const DirectX::XMVECTOR& v);
    DirectX::XMVECTOR vec;
};

inline void to_json(Json& j, const Vec4& v)
{
    j = { v.X(), v.Y(), v.Z(), v.W()};
}

inline void from_json(const Json& j, Vec4& v)
{
    float x = j.at(0).get<float>();
    float y = j.at(1).get<float>();
    float z = j.at(2).get<float>();
    float w = j.at(3).get<float>();

    v = Vec4(x, y, z, w);
}