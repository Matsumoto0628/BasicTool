#include "particle.h"
#include "transform.h"
#include "rigidbody.h"
#include "line.h"

Particle::Particle(Transform* pTransform, Rigidbody* pRb, Line* pLine)
	: m_pTransform(pTransform),
	m_pRb(pRb),
	m_pLine(pLine)
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
	Vec3 start = m_pTransform->GetPosition();
	m_pLine->SetLine(start, start + m_pRb->GetVelocity().Normalize());
}

void Particle::Terminate()
{
}

void Particle::Finalize()
{
}