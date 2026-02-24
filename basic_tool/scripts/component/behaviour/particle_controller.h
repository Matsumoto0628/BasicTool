#pragma once
#include "component.h"
#include <vector>

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
	void Finalize() override;
	void Show() override;
	Json Serialize() const override;
	static std::unique_ptr<ParticleController> Deserialize(const Json& j, Transform* const pTransform, const RenderContext* const pContext);
private:
	Transform* const m_pTransform = nullptr;
	const RenderContext* const m_pContext = nullptr;
	const Camera* const m_pCamera = nullptr;
	Transform* const m_pCameraTransform = nullptr;
	std::vector<GameObject*> m_pParticles;
	std::vector<Rigidbody*> m_pRigidbodies;

	// 定数
	static const int PARTICLE_COUNT = 100;
};