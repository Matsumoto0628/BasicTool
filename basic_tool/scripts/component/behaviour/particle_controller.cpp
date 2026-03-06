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
#include "imgui.h"
#include <windows.h>
#include <commdlg.h>
#include "game_time.h"

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
		auto& sprite = gameObject.AddComponent<Sprite>(m_pContext, m_pCamera, &gameObject.GetTransform(), Vec4{ 30,30,30,1 });
		auto& line = gameObject.AddComponent<Line>(m_pContext, m_pCamera, Vec4{ 1,0,0,1 });
		auto& rb = gameObject.AddComponent<Rigidbody>(&gameObject.GetTransform());
		rb.SetVelocity({
			GameRandom::GetRange(-1.0f, 1.0f),
			GameRandom::GetRange(-1.0f, 1.0f),
			GameRandom::GetRange(-1.0f, 1.0f)
		});
		m_pRigidbodies.push_back(&rb);
		gameObject.AddComponent<Particle>(&gameObject.GetTransform(), &rb, &line, m_pCameraTransform, &sprite, 2.0f);
		m_pParticles.push_back(&gameObject);
	}
}

void ParticleController::Update()
{
	if (!m_isPause)
	{
		m_timer += GameTime::GetDeltaTime();
		if (m_timer >= m_duration) // 時間を経過で再生し直す
		{
			Play();
		}

		// 非ポーズのときだけ寿命を更新する
		for (auto& pParticle : m_pParticles)
		{
			auto particle = pParticle->FindComponent<Particle>(Component::Type::Particle);
			if (particle)
			{
				particle->UpdateLife();
			}
		}
	}

	for (auto& pRigidbody : m_pRigidbodies)
	{
		//pRigidbody->AddForce({0,-9.8f,0});
	}
}

void ParticleController::Draw()
{
}

void ParticleController::Finalize()
{
}

void ParticleController::Show()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Particle", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Change Texture"))
		{
			std::wstring path = openTextureDialog();

			if (!path.empty())
			{
				for (auto& pParticle : m_pParticles)
				{
					auto sprite = pParticle->FindComponent<Sprite>(Component::Type::Sprite);
					if (sprite)
					{
						sprite->SetTexture(path);
					}
				}
			}
		}
	}

	ImGui::PopID();
}

Json ParticleController::Serialize() const
{
	return {
		{"id", GetID()},
		{"type", GetType()},
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
		SceneManager::GetCurrentScene()->FindComponent<Camera>(j.at("camera").get<uint64_t>()),
		&SceneManager::GetCurrentScene()->FindGameObject(j.at("camera_game_object").get<uint64_t>())->GetTransform()
	);
	return pComponent;
}

void ParticleController::Play()
{
	m_timer = 0;
	m_isPause = false;

	for (auto& pParticle : m_pParticles)
	{
		pParticle->GetTransform().SetPosition({ 0,0,0 });
		pParticle->GetTransform().SetLocalPosition({ 0,0,0 });
		auto particle = pParticle->FindComponent<Particle>(Component::Type::Particle);
		if (particle)
		{
			particle->Restart();
		}
	}

	for (auto& pRigidbody : m_pRigidbodies)
	{
		pRigidbody->SetEnabled(true);

		pRigidbody->SetVelocity({
			GameRandom::GetRange(-1.0f, 1.0f),
			GameRandom::GetRange(-1.0f, 1.0f),
			GameRandom::GetRange(-1.0f, 1.0f)
		});
	}
}

void ParticleController::Pause()
{
	m_isPause = true;
	for (auto& pRigidbody : m_pRigidbodies)
	{
		pRigidbody->SetEnabled(false);
	}
}

void ParticleController::Resume()
{
	m_isPause = false;
	for (auto& pRigidbody : m_pRigidbodies)
	{
		pRigidbody->SetEnabled(true);
	}
}

std::wstring ParticleController::openTextureDialog()
{
	wchar_t fileName[MAX_PATH] = L"";

	OPENFILENAMEW ofn{};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = SceneManager::GetCurrentScene()->GetWnd();
	ofn.lpstrFilter = L"PNG Files (*.png)\0*.png\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = L"png";

	if (GetOpenFileNameW(&ofn))
	{
		return std::wstring(fileName);
	}

	return L"";
}
