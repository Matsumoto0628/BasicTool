#pragma once
#include <vector>
#include "component.h"
#include <nlohmann/json.hpp>
using Json = nlohmann::ordered_json;

class GameObject;

class Transform
{
public:
	Transform(GameObject* const pGameObject);
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
	const std::vector<Transform*>& GetChildren() const { return m_pChildren; }
	const Transform* const GetParent() const { return m_pParent; }
	GameObject& GetGameObject() const { return *m_pGameObject; }
	void SetPosition(const Vec3& position);
	void SetRotation(const Vec4& rotation);
	void SetScale(const Vec3& scale);
	void SetLocalPosition(const Vec3& position);
	void SetLocalRotation(const Vec4& rotation);
	void SetLocalScale(const Vec3& scale);
	void SetParent(Transform* pParent);
	void Show();
	Json Serialize() const;

private:
	Transform() = delete;
	void applyWorld();
	void applyLocal();
	void applyMatrix();
	void applyEulerAngles(const Vec3& eulerDeg);
	Vec3 m_position;
	Vec4 m_rotation;
	Vec3 m_scale;
	Vec3 m_localPosition;
	Vec4 m_localRotation;
	Vec3 m_localScale;
	Mat4x4 m_matrix;
	Mat4x4 m_world;
	Mat4x4 m_local;
	Vec3 m_eulerAngles;

	std::vector<Transform*> m_pChildren;
	Transform* m_pParent = nullptr;

	GameObject* const m_pGameObject = nullptr;
};