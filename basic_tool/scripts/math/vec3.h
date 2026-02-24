#pragma once
#include <DirectXMath.h>
#include <nlohmann/json.hpp>
using Json = nlohmann::ordered_json;

// DirectXMathをラップすることで、SIMDを維持して高速に計算
// 3成分ベクトル
class Vec3 
{
public:
    Vec3();
    ~Vec3();
    Vec3(float x, float y, float z);
    
    const Vec3 operator+(const Vec3& rhs) const;
    const Vec3& operator+=(const Vec3& rhs);
    const Vec3 operator-(const Vec3& rhs) const;
    const Vec3& operator-=(const Vec3& rhs);
    const Vec3 operator*(float s) const;
    const Vec3& operator*=(float s);
    const Vec3 operator/(float s) const;
    const Vec3& operator/=(float s);
    
    float X() const;
    float Y() const;
    float Z() const;
    float Length() const;
    Vec3 Normalize() const;
    void ToFloat3(float out[3]) const;

private:
    Vec3(const DirectX::XMVECTOR& v); // DirectXMathのコンストラクタは自分だけ使う
    DirectX::XMVECTOR vec;
};

inline void to_json(Json& j, const Vec3& v)
{
    j = { v.X(), v.Y(), v.Z()};
}

inline void from_json(const Json& j, Vec3& v)
{
    float x = j.at(0).get<float>();
    float y = j.at(1).get<float>();
    float z = j.at(2).get<float>();

    v = Vec3(x, y, z);
}