#pragma once
#include <vector>
#include "component.h"

class Transform
{
public:
	Transform();
	~Transform();
	const Vec3& GetPosition() const { return m_position; }
	const Vec4& GetRotation() const { return m_rotation; }
	const Vec3& GetScale() const { return m_scale; }
	const Vec3& GetLocalPosition() const { return m_localPosition; }
	const Vec4& GetLocalRotation() const { return m_localRotation; }
	const Vec3& GetLocalScale() const { return m_localScale; }
	const Mat4x4& GetMatrix() const { return m_matrix; }
	const Vec3& GetForward() const { return m_rotation.RotateVec3({ 0,0,1 }); }
	const Vec3& GetUp() const { return m_rotation.RotateVec3({ 0,1,0 }); }
	const Vec3& GetRight() const { return m_rotation.RotateVec3({ 1,0,0 }); }
	void SetPosition(const Vec3& position);
	void SetRotation(const Vec4& rotation);
	void SetScale(const Vec3& scale);
	void SetLocalPosition(const Vec3& position);
	void SetLocalRotation(const Vec4& rotation);
	void SetLocalScale(const Vec3& scale);
	void SetParent(Transform* pParent);
	void Show();

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