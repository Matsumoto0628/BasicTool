#include "rigidbody.h"
#include "transform.h"
#include "game_time.h"
#include "game_object.h"
#include "scene_manager.h"
#include "game_random.h"

Rigidbody::Rigidbody(uint64_t id, Transform* const pTransform)
    : Component{ id, Type::Rigidbody },
    m_pTransform{ pTransform },
    m_velocity{ 0, 0, 0 },
    m_acceleration{ 0, 0, 0 },
    m_mass{ 1.0f },
    m_force{ 0, 0, 0 }
{
}

Rigidbody::Rigidbody(Transform* const pTransform)
	: Rigidbody{ GameRandom::GetUUID(), pTransform }
{

}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Initialize()
{
}

void Rigidbody::Start()
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

void Rigidbody::Draw()
{
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

Json Rigidbody::Serialize() const
{
	return {
        {"id", GetID()},
		{"type", GetType()},
        {"mass", m_mass}
    };
}

std::unique_ptr<Rigidbody> Rigidbody::Deserialize(const Json& j, Transform* const pTransform)
{
	auto pComponent = std::make_unique<Rigidbody>(j.at("id").get<uint64_t>(), pTransform);
	pComponent->m_mass = j.at("mass").get<float>();
	return pComponent;
}
