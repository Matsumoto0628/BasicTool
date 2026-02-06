#pragma once
#include <DirectXMath.h>

// DirectXMathをラップすることで、SIMDを維持して高速に計算
// 3成分ベクトル
class Vec3 
{
public:
    Vec3();
    ~Vec3();
    Vec3(float x, float y, float z);
    
    Vec3 operator+(const Vec3& rhs) const;
    Vec3& operator+=(const Vec3& rhs);
    Vec3 operator-(const Vec3& rhs) const;
    Vec3& operator-=(const Vec3& rhs);
    Vec3 operator*(float s) const;
    Vec3& operator*=(float s);
    Vec3 operator/(float s) const;
    Vec3& operator/=(float s);
    
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