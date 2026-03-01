#include "particle_controller.h"
#include "scene_manager.h"
#include "sprite.h"
#include "line.h"
#include "game_random.h"
#include "particle.h"
#include "transform.h"
#include "rigidbody.h"
#include "game_input.h"
#include "camera.h"

ParticleController::ParticleController(
	uint64_t id, 
	Transform* const pTransform, 
	const RenderContext* const pContext, 
	const Camera* const pCamera, 
	Transform* const pCameraTransform
)
	: Component{ id, Type::ParticleController },
	m_pTransform{ pTransform },
	m_pContext{ pContext },
	m_pCamera{ pCamera },
	m_pCameraTransform{ pCameraTransform }
{
}

ParticleController::ParticleController(
	Transform* const pTransform,
	const RenderContext* const pContext,
	const Camera* const pCamera, 
	Transform* const pCameraTransform
)
	: ParticleController{ GameRandom::GetUUID(), pTransform, pContext, pCamera, pCameraTransform }
{
}

ParticleController::~ParticleController()
{
}

void ParticleController::Initialize()
{
}

void ParticleController::Start()
{
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		auto& gameObject = SceneManager::GetCurrentScene()->Instantiate("Particle");
		gameObject.GetTransform().SetParent(m_pTransform);
		gameObject.GetTransform().SetLocalScale({ 0.1f,0.1f,0.1f });
		gameObject.GetTransform().SetLocalPosition({ 0,0,0 });
		gameObject.AddComponent<Sprite>(m_pContext, m_pCamera, &gameObject.GetTransform(), Vec4{ 30,30,30,1 });
		auto& line = gameObject.AddComponent<Line>(m_pContext, m_pCamera, Vec4{ 1,0,0,1 });
		auto& rb = gameObject.AddComponent<Rigidbody>(&gameObject.GetTransform());
		rb.AddForce({
			GameRandom::GetRange(-1.0f, 1.0f),
			GameRandom::GetRange(-1.0f, 1.0f),
			GameRandom::GetRange(-1.0f, 1.0f)
			});
		m_pRigidbodies.push_back(&rb);
		gameObject.AddComponent<Particle>(&gameObject.GetTransform(), &rb, &line, m_pCameraTransform);
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
			pParticle->GetTransform().SetLocalPosition({ 0,0,0 });
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

	if (GameInput::GetKeyDown(VK_SPACE))
	{
		for (auto& pRigidbody : m_pRigidbodies)
		{
			pRigidbody->SetEnabled(!pRigidbody->GetEnabled());
		}
	}
}

void ParticleController::Finalize()
{
}

void ParticleController::Show()
{
}

Json ParticleController::Serialize() const
{
	return {
		{"id", m_id},
		{"type", m_type},
		{"camera", m_pCamera->GetID()},
		{"camera_game_object", m_pCameraTransform->GetGameObject().GetID()}
	};
}

std::unique_ptr<ParticleController> ParticleController::Deserialize(const Json& j, Transform* const pTransform, const RenderContext* const pContext)
{
	auto pComponent = std::make_unique<ParticleController>(
		j.at("id").get<uint64_t>(),
		pTransform,
		pContext,
		static_cast<const Camera* const>(SceneManager::GetCurrentScene()->FindComponent(j.at("camera").get<uint64_t>())),
		&SceneManager::GetCurrentScene()->FindGameObject(j.at("camera_game_object").get<uint64_t>())->GetTransform()
	);
	return pComponent;
}