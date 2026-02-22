#pragma once
#include "component.h"

class Transform;
class Rigidbody;
class Line;

class Particle : public Component
{
public:
	Particle(Transform* pTransform, Rigidbody* pRb, Line* pLine, Transform* pCameraTransform);
	~Particle();
	void Initialize() override;
	void Update() override;
	void Finalize() override;
	void Show() override;

private:
	Particle() = delete;
	Transform* const m_pTransform = nullptr;
	Rigidbody* const m_pRb = nullptr;
	Line* const m_pLine = nullptr;
	const Transform* const m_pCameraTransform = nullptr;
};