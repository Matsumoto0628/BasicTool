#include "rigidbody.h"
#include "transform.h"
#include "game_time.h"

Rigidbody::Rigidbody(Transform* const pTransform) 
    : m_pTransform{ pTransform },
    m_velocity{0, 0, 0},
    m_acceleration{0, 0, 0},
    m_mass{1.0f},
    m_force{0, 0, 0}
{

}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Initialize()
{
}

void Rigidbody::Update()
{
    if (m_mass > 0)
    {
        m_acceleration = m_force / m_mass;
    }
    else
    {
        m_acceleration = { 0, 0, 0 };
    }

    m_velocity += m_acceleration * GameTime::GetDeltaTime();
    if (m_pTransform->GetParent())
    {
        m_pTransform->SetLocalPosition(m_pTransform->GetLocalPosition() + m_velocity * GameTime::GetDeltaTime());
    }
    else 
    {
        m_pTransform->SetPosition(m_pTransform->GetPosition() + m_velocity * GameTime::GetDeltaTime());
    }

    m_force = { 0, 0, 0 };
}

void Rigidbody::Finalize()
{
}

void Rigidbody::Show()
{
}

void Rigidbody::AddForce(const Vec3& force)
{
    m_force += force;
}

void Rigidbody::SetVelocity(const Vec3& vel)
{
    m_velocity = vel;
}