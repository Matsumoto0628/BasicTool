#pragma once

class Vec3;
class Vec4;

float DegToRad(float deg);
float RadToDeg(float rad);
Vec4 EulerToQuaternion(const Vec3& euler);
Vec4 AxisToQuaternion(const Vec3& axis, float rad);

static const float PI;