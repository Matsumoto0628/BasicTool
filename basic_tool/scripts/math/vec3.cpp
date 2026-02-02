Vec3::Vec3()
	: vec(DirectX::XMVectorZero())
{
}

Vec3::~Vec3() 
{
}

Vec3::Vec3(float x, float y, float z)
	: vec(DirectX::XMVectorSet(x, y, z, 0.0f))
{
}

Vec3::Vec3(const DirectX::XMVECTOR& v)
	: vec(v)
{
}

Vec3 Vec3::operator+(const Vec3& rhs) const 
{
	return Vec3(DirectX::XMVectorAdd(vec, rhs.vec));
}

Vec3 Vec3::operator-(const Vec3& rhs) const
{
	return Vec3(DirectX::XMVectorSubtract(vec, rhs.vec));
}

Vec3 Vec3::operator*(float s) const
{
	return Vec3(DirectX::XMVectorScale(vec, s));
}

float Vec3::X() const 
{
	return DirectX::XMVectorGetX(vec);
}

float Vec3::Y() const
{
	return DirectX::XMVectorGetY(vec);
}

float Vec3::Z() const
{
	return DirectX::XMVectorGetZ(vec);
}

float Vec3::Length() const 
{
	return DirectX::XMVectorGetX(DirectX::XMVector3Length(vec));
}

Vec3 Vec3::Normalize() const
{
	return Vec3(DirectX::XMVector3Normalize(vec));
}

void Vec3::ToFloat3(float out[3]) const
{
	DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(out), vec);
}