#include "particle.h"
#include "transform.h"
#include "rigidbody.h"
#include "line.h"
#include "game_random.h"
#include "game_object.h"
#include "scene_manager.h"
#include "game_time.h"
#include "sprite.h"

Particle::Particle(uint64_t id, Transform* pTransform, Rigidbody* pRb, Line* pLine, Transform* pCameraTransform, Sprite* pSprite, float lifeTime)
	: Component{ id, Type::Particle },
	m_pTransform{ pTransform },
	m_pRb{ pRb },
	m_pLine{ pLine },
	m_pCameraTransform{ pCameraTransform },
	m_pSprite{pSprite},
	m_lifeTime{ lifeTime },
	m_color{m_pSprite->GetColor()}
{
}

Particle::Particle(Transform* pTransform, Rigidbody* pRb, Line* pLine, Transform* pCameraTransform, Sprite* pSprite, float lifeTime)
	: Particle{ GameRandom::GetUUID(), pTransform, pRb, pLine, pCameraTransform, pSprite, lifeTime }
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
	drawLine();

	// ビルボード
	{
		const Vec3 dir = (m_pCameraTransform->GetPosition() - m_pTransform->GetPosition()).Normalize();
		m_pTransform->SetRotation(LookRotation(dir, {0,1,0}));
	}
}

void Particle::Draw()
{
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
		{"id", GetID()},
		{"type", GetType()},
		{"rigidbody", m_pRb->GetID()},
		{"line", m_pLine->GetID()},
		{"camera_game_object", m_pCameraTransform->GetGameObject().GetID()},
		{"sprite", m_pSprite->GetID()}
	};
}

std::unique_ptr<Particle> Particle::Deserialize(const Json& j, Transform* pTransform)
{
	auto pComponent = std::make_unique<Particle>(
		j.at("id").get<uint64_t>(),
		pTransform,
		SceneManager::GetCurrentScene()->FindComponent<Rigidbody>(j.at("rigidbody").get<uint64_t>()),
		SceneManager::GetCurrentScene()->FindComponent<Line>(j.at("line").get<uint64_t>()),
		&SceneManager::GetCurrentScene()->FindGameObject(j.at("camera_game_object").get<uint64_t>())->GetTransform(),
		SceneManager::GetCurrentScene()->FindComponent<Sprite>(j.at("sprite").get<uint64_t>()),
		3.0f // 仮で置いておく
	);
	return pComponent;
}

void Particle::Restart()
{
	m_timer = 0;
	m_pSprite->SetEnabled(true);
	m_pSprite->SetColor(m_color);
}

void Particle::UpdateLife() 
{
	m_timer += GameTime::GetDeltaTime();
	if (m_timer >= m_lifeTime)
	{
		m_pSprite->SetEnabled(false);
	}
	const Vec4 color = { m_color.X(), m_color.Y(), m_color.Z(), m_color.W() * (1 - (m_timer / m_lifeTime)) };
	m_pSprite->SetColor(color);
}

void Particle::drawLine()
{
	m_pLine->SetEnabled(m_isLine);
	if (m_isLine)
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
}
