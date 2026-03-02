#include "camera.h"
#include "render_context.h"
#include "game_random.h"

Camera::Camera(uint64_t id, const Transform* const pTransform, float aspect)
	: Component{ id, Type::Camera },
	m_pTransform{ pTransform },
	m_aspect{ aspect },
	m_nearZ{ 0.1f },
	m_farZ{ 1000.0f },
	m_fov{ DegToRad(30.0f) }
{
}

Camera::Camera(const Transform* const pTransform, unsigned int width, unsigned int height)
	: Camera(GameRandom::GetUUID(), pTransform, static_cast<float>(width) / static_cast<float>(height))
{
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
}

void Camera::Start()
{
	m_proj = Mat4x4::PerspectiveFovLH(m_fov, m_aspect, m_nearZ, m_farZ);
}

void Camera::Update() 
{
}

void Camera::Draw()
{
}

void Camera::Finalize() 
{
}

void Camera::Show()
{
}

Json Camera::Serialize() const
{
	return {
		{"id", GetID()},
		{"type", GetType()},
		{"aspect", m_aspect},
		{"near_z", m_nearZ},
		{"far_z", m_farZ},
		{"fov", m_fov}
	};
}

std::unique_ptr<Camera> Camera::Deserialize(const Json& j, const Transform* const pTransform)
{
	auto pComponent = std::make_unique<Camera>(j.at("id").get<uint64_t>(), pTransform, j.at("aspect").get<float>());
	pComponent->m_nearZ = j.at("near_z").get<float>();
	pComponent->m_farZ = j.at("far_z").get<float>();
	pComponent->m_fov = j.at("fov").get<float>();
	pComponent->m_proj = Mat4x4::PerspectiveFovLH(pComponent->m_fov, pComponent->m_aspect, pComponent->m_nearZ, pComponent->m_farZ);
	return pComponent;
}
