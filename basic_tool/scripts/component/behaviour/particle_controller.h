#pragma once
#include "component.h"
#include <vector>
#include <string>

class Camera;
class Transform;
class RenderContext;
class Rigidbody;
class GameObject;

class ParticleController : public Component
{
public:
	ParticleController(
		uint64_t id,
		Transform* const pTransform,
		const RenderContext* const pContext,
		const Camera* const pCamera,
		Transform* const pCameraTransform
	);
	ParticleController(
		Transform* const pTransform, 
		const RenderContext* const pContext,
		const Camera* const pCamera,
		Transform* const pCameraTransform
	);
	~ParticleController();
	void Initialize() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void Show() override;
	Json Serialize() const override;
	static std::unique_ptr<ParticleController> Deserialize(const Json& j, Transform* const pTransform, const RenderContext* const pContext);
	void Play();
	void Pause();
	void Resume();
private:
	std::wstring openTextureDialog();
	void applyTexture(std::wstring path);
	void play(int idx);
	void playBurst();
	bool showColor(const char* label, int& r, int& g, int& b, float width = 60.0f);
	Transform* const m_pTransform = nullptr;
	const RenderContext* const m_pContext = nullptr;
	const Camera* const m_pCamera = nullptr;
	Transform* const m_pCameraTransform = nullptr;
	std::vector<GameObject*> m_pParticles;
	float m_duration = 6;
	float m_timer = 0;
	float m_isPause = false;
	int m_idx = 0;
	float m_span = 0.01f;
	float m_playTimer = 0;
	bool m_isBurst = false;
	float m_size = 1.0f;
	int m_burstCount = 30;
	Vec4 m_color = {1,1,1,1};
	float m_intensity = 10.0f;
	float m_lifeTime = 2.0f;
	float m_gravity = 3.0f;
	float m_spread = 1.0f;
	std::wstring m_texturePath = L"SlimeBall.png";

	// 定数
	static const int PARTICLE_COUNT = 100;
	static const float SIZE_MULTIPLIER;
};