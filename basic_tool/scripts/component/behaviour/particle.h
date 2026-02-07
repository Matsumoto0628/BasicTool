#pragma once
#include "component.h"

class Transform;
class Rigidbody;
class Line;

class Particle : public Component
{
public:
	Particle(Transform* pTransform, Rigidbody* pRb, Line* pLine);
	~Particle();
	void Initialize() override;
	void Start() override;
	void Update() override;
	void Terminate() override;
	void Finalize() override;

private:
	Particle() = delete;
	Transform* m_pTransform;
	Rigidbody* m_pRb;
	Line* m_pLine;
};