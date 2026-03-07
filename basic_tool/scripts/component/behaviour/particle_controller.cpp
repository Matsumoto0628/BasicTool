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
#include <filesystem>

const float ParticleController::SIZE_MULTIPLIER = 0.1f;

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
		auto& sprite = gameObject.AddComponent<Sprite>(m_pContext, m_pCamera, &gameObject.GetTransform(), Vec4{ m_color.X() * m_intensity, m_color.Y() * m_intensity, m_color.Z() * m_intensity, m_color.W() });
		auto& line = gameObject.AddComponent<Line>(m_pContext, m_pCamera, Vec4{ 1,0,0,1 });
		auto& rb = gameObject.AddComponent<Rigidbody>(&gameObject.GetTransform());
		auto& particle = gameObject.AddComponent<Particle>(&gameObject.GetTransform(), &rb, &line, m_pCameraTransform, &sprite, m_lifeTime);
		gameObject.GetTransform().SetParent(m_pTransform);
		gameObject.GetTransform().SetLocalScale({ m_size * SIZE_MULTIPLIER, m_size * SIZE_MULTIPLIER ,m_size * SIZE_MULTIPLIER });
		particle.Reset();
		m_pParticles.push_back(&gameObject);
	}

	Play();
}

void ParticleController::Update()
{
	if (!m_isPause)
	{
		m_timer += GameTime::GetDeltaTime();
		if (m_timer >= m_duration) // 時間経過で再生し直す
		{
			m_timer = 0;
			Play();
		}
		
		if (!m_isBurst) 
		{
			m_playTimer += GameTime::GetDeltaTime();
			if (m_playTimer >= m_span && m_idx <= PARTICLE_COUNT - 1)
			{
				m_playTimer = 0;
				play(m_idx);
				m_idx++;
			}
		}

		// 非ポーズのときだけ寿命を更新する
		for (auto& pParticleGameObject : m_pParticles)
		{
			auto pParticle = pParticleGameObject->FindComponent<Particle>(Component::Type::Particle);
			if (pParticle)
			{
				pParticle->UpdateLife();
			}

			auto pRb = pParticleGameObject->FindComponent<Rigidbody>(Component::Type::Rigidbody);
			if (pRb)
			{
				pRb->AddForce({ 0,m_gravity,0 });
			}
		}
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
	int r = m_color.X() * 255;
	int g = m_color.Y() * 255;
	int b = m_color.Z() * 255;
	bool isChangeSize = false;
	bool isChangeColor = false;
	bool isChangeIntensity = false;
	bool isChangeLifeTime = false;

	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Particle", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100);

		ImGui::Text("Duration");
		ImGui::NextColumn();
		ImGui::DragFloat("##Duration", &m_duration, 0.01f, 0.1f, 10.0f);
		ImGui::NextColumn();

		ImGui::Text("Texture");
		ImGui::NextColumn();
		if (ImGui::Button(std::filesystem::path(m_texturePath).filename().string().c_str()))
		{
			applyTexture();
		}
		ImGui::NextColumn();

		ImGui::Text("Size");
		ImGui::NextColumn();
		isChangeSize = ImGui::DragFloat("##Size", &m_size, 0.01f, 0.1f, 10.0f);
		ImGui::NextColumn();

		ImGui::Text("IsBurst");
		ImGui::NextColumn();
		ImGui::Checkbox("##IsBurst", &m_isBurst);
		ImGui::NextColumn();

		if (m_isBurst) 
		{
			ImGui::Text("BurstCount");
			ImGui::NextColumn();
			ImGui::DragInt("##BurstCount", &m_burstCount, 0.1f, 1, 100);
			ImGui::NextColumn();
		}
		else
		{
			ImGui::Text("Span");
			ImGui::NextColumn();
			ImGui::DragFloat("##Span", &m_span, 0.01f, 0.01f, 1.0f);
			ImGui::NextColumn();
		}

		isChangeColor = showColor("Color", r, g, b);

		ImGui::Text("Intensity");
		ImGui::NextColumn();
		isChangeIntensity = ImGui::DragFloat("##Intensity", &m_intensity, 0.1f, 1.0f, 10.0f);
		ImGui::NextColumn();

		ImGui::Text("LifeTime");
		ImGui::NextColumn();
		isChangeLifeTime = ImGui::DragFloat("##LifeTime", &m_lifeTime, 0.1f, 0.1f, 10.0f);
		ImGui::NextColumn();

		ImGui::Text("Gravity");
		ImGui::NextColumn();
		ImGui::DragFloat("##Gravity", &m_gravity, 0.1f, -10.0f, 10.0f);
		ImGui::NextColumn();

		ImGui::Text("Spread");
		ImGui::NextColumn();
		ImGui::DragFloat("##Spread", &m_spread, 0.1f, 0.1f, 10.0f);
		ImGui::NextColumn();

		ImGui::Columns(1);
	}

	ImGui::PopID();

	if (isChangeSize) 
	{
		for (auto pParticle : m_pParticles) 
		{
			pParticle->GetTransform().SetLocalScale({ m_size * SIZE_MULTIPLIER, m_size * SIZE_MULTIPLIER ,m_size * SIZE_MULTIPLIER });
		}
	}

	if (isChangeColor || isChangeIntensity) 
	{
		m_color = Vec4{ static_cast<float>(r) / 255, static_cast<float>(g) / 255, static_cast<float>(b) / 255, m_color.W()};
		for (auto& pParticleGameObject : m_pParticles)
		{
			auto pParticle = pParticleGameObject->FindComponent<Particle>(Component::Type::Particle);
			if (pParticle)
			{
				pParticle->SetColor({ m_color.X() * m_intensity, m_color.Y() * m_intensity, m_color.Z() * m_intensity, m_color.W() });
			}
		}
	}

	if (isChangeLifeTime) 
	{
		for (auto& pParticleGameObject : m_pParticles)
		{
			auto pParticle = pParticleGameObject->FindComponent<Particle>(Component::Type::Particle);
			if (pParticle)
			{
				pParticle->SetLifeTime(m_lifeTime);
			}
		}
	}
}

Json ParticleController::Serialize() const
{
	return {
		{"id", GetID()},
		{"type", GetType()},
		{"camera", m_pCamera->GetID()},
		{"camera_game_object", m_pCameraTransform->GetGameObject().GetID()},

		{ "duration", m_duration },
		{"size", m_size},
		{"isBurst", m_isBurst},
		{"burstCount", m_burstCount},
		{"span", m_span},
		{"intensity", m_intensity},
		{"lifeTime", m_lifeTime},
		{"gravity", m_gravity},
		{"spread", m_spread},

		{"texturePath", m_texturePath},

		{"color", {
			m_color.X(),
			m_color.Y(),
			m_color.Z(),
			m_color.W()
		}}
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

	pComponent->m_duration = j.value("duration", 1.0f);
	pComponent->m_size = j.value("size", 1.0f);
	pComponent->m_isBurst = j.value("isBurst", false);
	pComponent->m_burstCount = j.value("burstCount", 30);
	pComponent->m_span = j.value("span", 0.1f);
	pComponent->m_intensity = j.value("intensity", 1.0f);
	pComponent->m_lifeTime = j.value("lifeTime", 1.0f);
	pComponent->m_gravity = j.value("gravity", 0.0f);
	pComponent->m_spread = j.value("spread", 1.0f);

	pComponent->m_texturePath = j.value("texturePath", "SlimeBall.png");

	if (j.contains("color"))
	{
		auto c = j["color"];
		pComponent->m_color = Vec4{
			c[0].get<float>(),
			c[1].get<float>(),
			c[2].get<float>(),
			c[3].get<float>()
		};
	}

	return pComponent;
}

void ParticleController::Play()
{
	m_isPause = false;
	m_idx = 0;
	m_timer = 0;
	m_playTimer = 0;

	// パーティクルをリセット
	for (auto& pParticleGameObject : m_pParticles)
	{
		auto pParticle = pParticleGameObject->FindComponent<Particle>(Component::Type::Particle);
		if (pParticle)
		{
			pParticle->Reset();
		}
	}

	if (m_isBurst) 
	{
		playBurst();
	}
}

void ParticleController::Pause()
{
	m_isPause = true;
	for (auto& pParticle : m_pParticles)
	{
		auto pRb = pParticle->FindComponent<Rigidbody>(Component::Type::Rigidbody);
		if (pRb)
		{
			pRb->SetEnabled(false);
		}
	}
}

void ParticleController::Resume()
{
	m_isPause = false;
	for (auto& pParticle : m_pParticles)
	{
		auto pRb = pParticle->FindComponent<Rigidbody>(Component::Type::Rigidbody);
		if (pRb)
		{
			pRb->SetEnabled(true);
		}
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

void ParticleController::applyTexture()
{
	std::wstring path = openTextureDialog();

	if (!path.empty())
	{
		m_texturePath = std::filesystem::path(path).string();

		for (auto& pParticle : m_pParticles)
		{
			auto pSprite = pParticle->FindComponent<Sprite>(Component::Type::Sprite);
			if (pSprite)
			{
				pSprite->SetTexture(path);
			}
		}
	}
}

void ParticleController::play(int idx)
{
	idx = max(idx, 0);
	idx = min(idx, PARTICLE_COUNT - 1);
	GameObject* pParticleGameObject = m_pParticles[idx];

	// パーティクルをリセット
	auto pParticle = pParticleGameObject->FindComponent<Particle>(Component::Type::Particle);
	if (pParticle)
	{
		pParticle->Reset();
	}

	// スプライトを表示
	auto pSprite = pParticleGameObject->FindComponent<Sprite>(Component::Type::Sprite);
	if (pSprite)
	{
		pSprite->SetEnabled(true);
	}

	// 力を加える
	auto pRb = pParticleGameObject->FindComponent<Rigidbody>(Component::Type::Rigidbody);
	if (pRb)
	{
		pRb->SetEnabled(true);

		pRb->SetVelocity({
			GameRandom::GetRange(-1.0f, 1.0f) * m_spread,
			GameRandom::GetRange(-1.0f, 1.0f) * m_spread,
			GameRandom::GetRange(-1.0f, 1.0f)* m_spread
		});
	}
}

void ParticleController::playBurst() 
{
	for (int i = 0; i < m_burstCount; i++) 
	{
		play(i);
	}
}

bool ParticleController::showColor(const char* label, int& r, int& g, int& b, float width)
{
	bool changed = false;

	ImGui::PushID(label);

	// 左カラム
	ImGui::Text(label);
	ImGui::NextColumn();
	ImGui::PushItemWidth(width);

	ImGui::Text("R");
	ImGui::SameLine();
	changed |= ImGui::DragInt("##r", &r, 0.3f, 0, 255);

	ImGui::Text("G");
	ImGui::SameLine();
	changed |= ImGui::DragInt("##g", &g, 0.3f, 0, 255);

	ImGui::Text("B");
	ImGui::SameLine();
	changed |= ImGui::DragInt("##b", &b, 0.3f, 0, 255);
	ImGui::NextColumn();

	ImGui::PopItemWidth();
	ImGui::PopID();

	return changed;
}