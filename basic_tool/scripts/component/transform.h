#pragma once
#include <vector>

class Transform
{
public:
	Transform();
	~Transform();
	const Vec3& Position() const { return m_position; }
	const Vec4& Rotation() const { return m_rotation; }
	const Vec3& Scale() const { return m_scale; }
	const Vec3& LocalPosition() const { return m_localPosition; }
	const Vec4& LocalRotation() const { return m_localRotation; }
	const Vec3& LocalScale() const { return m_localScale; }
	const Mat4x4& Matrix() const { return m_matrix; }
	void SetPosition(const Vec3& position);
	void SetRotation(const Vec4& rotation);
	void SetScale(const Vec3& scale);
	void SetLocalPosition(const Vec3& position);
	void SetLocalRotation(const Vec4& rotation);
	void SetLocalScale(const Vec3& scale);
	void SetParent(Transform* pParent);

private:
	void setWorld();
	void setLocal();
	void setMatrix();
	Vec3 m_position;
	Vec4 m_rotation;
	Vec3 m_scale;
	Vec3 m_localPosition;
	Vec4 m_localRotation;
	Vec3 m_localScale;
	Mat4x4 m_matrix;
	Mat4x4 m_world;
	Mat4x4 m_local;

	std::vector<Transform*> m_pChildren;
	Transform* m_pParent;
};