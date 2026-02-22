#include "particle_controller.h"
#include "scene_manager.h"
#include "sprite.h"
#include "line.h"
#include "game_random.h"
#include "particle.h"
#include "transform.h"
#include "rigidbody.h"
#include "game_input.h"

ParticleController::ParticleController(
	Transform* const pTransform,
	const RenderContext* const pContext,
	const Camera* const pCamera, 
	Transform* const pCameraTransform
)
	: m_pTransform{ pTransform },
	m_pContext{ pContext },
	m_pCamera{ pCamera },
	m_pCameraTransform{ pCameraTransform }
{
}

ParticleController::~ParticleController()
{
}

void ParticleController::Initialize()
{
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		auto& gameObject = SceneManager::GetCurrentScene()->Instantiate("Particle");
		gameObject.GetTransform().SetScale({ 0.1f,0.1f,0.1f });
		gameObject.AddComponent<Sprite>(m_pContext, m_pCamera, &gameObject.GetTransform(), Vec4{ 1.5,1.5,1.5,1 });
		auto& line = gameObject.AddComponent<Line>(m_pContext, m_pCamera, Vec4{ 1,0,0,1 });
		auto& rb = gameObject.AddComponent<Rigidbody>(&gameObject.GetTransform());
		rb.AddForce({
			GameRandom::GetRange(-1.0f, 1.0f),
			GameRandom::GetRange(-1.0f, 1.0f),
			GameRandom::GetRange(-1.0f, 1.0f)
		});
		m_pRigidbodies.push_back(&rb);
		gameObject.AddComponent<Particle>(&gameObject.GetTransform(), &rb, &line, m_pCameraTransform);
		gameObject.GetTransform().SetParent(m_pTransform);
		m_pParticles.push_back(&gameObject);
	}
}

void ParticleController::Update()
{
	for (auto& pRigidbody : m_pRigidbodies)
	{
		//pRigidbody->AddForce({0,-9.8f,0});
	}

	if (GameInput::GetKeyDown('R'))
	{
		for (auto& pParticle : m_pParticles)
		{
			pParticle->GetTransform().SetPosition({ 0,0,0 });
		}

		for (auto& pRigidbody : m_pRigidbodies)
		{
			pRigidbody->AddForce({
				GameRandom::GetRange(-1.0f, 1.0f),
				GameRandom::GetRange(-1.0f, 1.0f),
				GameRandom::GetRange(-1.0f, 1.0f)
			});
		}
	}
}

void ParticleController::Finalize()
{
}

void ParticleController::Show()
{
}
