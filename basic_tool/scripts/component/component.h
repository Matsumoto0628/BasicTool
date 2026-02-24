#pragma once
#include <nlohmann/json.hpp>
using Json = nlohmann::ordered_json;

// 仮想関数はテーブルによってパフォーマンスが良くなくなる可能性があるが、
// std::vectorで一括で管理するため純粋仮想関数を使用する
class Component
{
public:
	virtual ~Component();
	virtual void Initialize() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Finalize() = 0;
	virtual void Show() = 0;
	virtual Json Serialize() const = 0;
	virtual void Deserialize(const Json& j) = 0;
	void SetEnabled(bool flag) { enabled = flag; }
	bool GetEnabled() { return enabled; }
	enum class Type
	{
		None,
		Camera,
		Rigidbody,
		Triangle,
		Square,
		Line,
		Sphere,
		CameraController,
		ParticleController,
		Particle,
		Sprite
	};

protected:
	Component(Type type);
	Component(uint64_t id, Type type);
	bool enabled = true;
	const uint64_t m_id = -1;
	const Type m_type = Type::None;
};

NLOHMANN_JSON_SERIALIZE_ENUM(Component::Type, {
	{Component::Type::None, "None"},
	{Component::Type::Camera, "Camera"},
	{Component::Type::Rigidbody, "Rigidbody"},
	{Component::Type::Triangle, "Triangle"},
	{Component::Type::Square, "Square"},
	{Component::Type::Line, "Line"},
	{Component::Type::Sphere, "Sphere"},
	{Component::Type::CameraController, "CameraController"},
	{Component::Type::ParticleController, "ParticleController"},
	{Component::Type::Particle, "Particle"},
	{Component::Type::Sprite, "Sprite"},
})