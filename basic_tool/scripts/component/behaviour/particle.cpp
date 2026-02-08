#include "particle.h"
#include "transform.h"
#include "rigidbody.h"
#include "line.h"

Particle::Particle(Transform* pTransform, Rigidbody* pRb, Line* pLine, Transform* pCameraTransform)
	: m_pTransform(pTransform),
	m_pRb(pRb),
	m_pLine(pLine),
	m_pCameraTransform(pCameraTransform)
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
		Vec3 start = m_pTransform->GetPosition();
		m_pLine->SetLine(start, start + m_pRb->GetVelocity().Normalize());
	}

	// ビルボード
	{
		Vec3 dir = (m_pCameraTransform->GetPosition() - m_pTransform->GetPosition()).Normalize();
		m_pTransform->SetRotation(LookRotation(dir, {0,1,0}));
	}
}

void Particle::Terminate()
{
}

void Particle::Finalize()
{
}