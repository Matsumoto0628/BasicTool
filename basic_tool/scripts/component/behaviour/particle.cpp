#include "particle.h"
#include "transform.h"
#include "rigidbody.h"
#include "line.h"
#include "game_random.h"
#include "game_object.h"
#include "scene_manager.h"

Particle::Particle(uint64_t id, Transform* pTransform, Rigidbody* pRb, Line* pLine, Transform* pCameraTransform)
	: Component{ id, Type::Particle },
	m_pTransform{ pTransform },
	m_pRb{ pRb },
	m_pLine{ pLine },
	m_pCameraTransform{ pCameraTransform }
{
}

Particle::Particle(Transform* pTransform, Rigidbody* pRb, Line* pLine, Transform* pCameraTransform)
	: Particle{ GameRandom::GetUUID(), pTransform, pRb, pLine, pCameraTransform }
{
}

Particle::~Particle()
{
}

void Particle::Initialize()
{
}

void Particle::Start()
{
}

void Particle::Update()
{
	// デバッグ表示
	{
		const Vec3 worldVel = m_pRb->GetVelocity();
		Vec3 localVel = worldVel;
		const auto* const parent = m_pTransform->GetParent();

		if (parent)
		{
			if (parent->GetRotation().Length() != 0) // 回転0のときはワールドの速度ベクトルを表示
			{
				localVel = parent->GetRotation().RotateVec3(worldVel);
			}
		}

		const Vec3 start = m_pTransform->GetPosition();
		m_pLine->SetLine(start, start + localVel.Normalize());
	}

	// ビルボード
	{
		const Vec3 dir = (m_pCameraTransform->GetPosition() - m_pTransform->GetPosition()).Normalize();
		m_pTransform->SetRotation(LookRotation(dir, {0,1,0}));
	}
}

void Particle::Finalize()
{
}

void Particle::Show()
{
}

Json Particle::Serialize() const
{
	return {
		{"id", m_id},
		{"type", m_type},
		{"rigidbody", m_pRb->GetID()},
		{"line", m_pLine->GetID()},
		{"camera_game_object", m_pCameraTransform->GetGameObject().GetID()}
	};
}

std::unique_ptr<Particle> Particle::Deserialize(const Json& j, Transform* pTransform)
{
	auto pComponent = std::make_unique<Particle>(
		j.at("id").get<uint64_t>(),
		pTransform,
		static_cast<Rigidbody*>(SceneManager::GetCurrentScene()->FindComponent(j.at("rigidbody").get<uint64_t>())),
		static_cast<Line*>(SceneManager::GetCurrentScene()->FindComponent(j.at("line").get<uint64_t>())),
		&SceneManager::GetCurrentScene()->FindGameObject(j.at("camera_game_object").get<uint64_t>())->GetTransform()
	);
	return pComponent;
}