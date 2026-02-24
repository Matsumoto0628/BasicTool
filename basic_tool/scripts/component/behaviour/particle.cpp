#include "particle.h"
#include "transform.h"
#include "rigidbody.h"
#include "line.h"

Particle::Particle(Transform* pTransform, Rigidbody* pRb, Line* pLine, Transform* pCameraTransform)
	: Component{ Type::Particle },
	m_pTransform{ pTransform },
	m_pRb{pRb},
	m_pLine{pLine},
	m_pCameraTransform{ pCameraTransform }
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
	};
}

void Particle::Deserialize(const Json& j)
{
}
