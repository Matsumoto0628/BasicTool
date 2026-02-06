#pragma once

float DegToRad(float deg);
float RadToDeg(float rad);
Vec4 EulerToQuaternion(const Vec3& euler);
Vec4 AxisToQuaternion(const Vec3& axis, float rad);
extern const float PI; // グローバルを宣言だけ書くときはextern