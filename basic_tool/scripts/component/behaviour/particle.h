#pragma once
#include "component.h"

class Transform;
class Rigidbody;
class Line;

class Particle : public Component
{
public:
	Particle(uint64_t id, Transform* pTransform, Rigidbody* pRb, Line* pLine, Transform* pCameraTransform);
	Particle(Transform* pTransform, Rigidbody* pRb, Line* pLine, Transform* pCameraTransform);
	~Particle();
	void Initialize() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void Show() override;
	Json Serialize() const override;
	static std::unique_ptr<Particle> Deserialize(const Json& j, Transform* pTransform);

private:
	Particle() = delete;
	Transform* const m_pTransform = nullptr;
	Rigidbody* const m_pRb = nullptr;
	Line* const m_pLine = nullptr;
	const Transform* const m_pCameraTransform = nullptr;
};