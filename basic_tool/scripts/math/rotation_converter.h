#pragma once

float DegToRad(float deg);
float RadToDeg(float rad);
const Vec4& EulerToQuaternion(const Vec3& euler);
const Vec4& AxisToQuaternion(const Vec3& axis, float rad);
extern const float PI; // グローバルを宣言だけ書くときはextern