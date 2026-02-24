#include "camera.h"
#include "render_context.h"

Camera::Camera(const Transform* const pTransform, unsigned int width, unsigned int height)
	: Component{ Type::Camera },
	m_pTransform{ pTransform },
	m_aspect{ (float)width / (float)height },
	m_nearZ{0.1f},
	m_farZ{1000.0f},
	m_fov{DegToRad(30.0f)}
{
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	m_proj = Mat4x4::PerspectiveFovLH(m_fov, m_aspect, m_nearZ, m_farZ);
}

void Camera::Start()
{
}

void Camera::Update() 
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
		{"type", m_type},
		{"aspect", m_aspect},
		{"nearZ", m_nearZ},
		{"farZ", m_farZ},
		{"fov", m_fov}
	};
}

void Camera::Deserialize(const Json& j)
{
}
