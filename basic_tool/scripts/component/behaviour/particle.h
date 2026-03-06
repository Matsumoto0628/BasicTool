#pragma once
#include "component.h"

class Transform;
class Rigidbody;
class Line;
class Sprite;

class Particle : public Component
{
public:
	Particle(uint64_t id, Transform* pTransform, Rigidbody* pRb, Line* pLine, Transform* pCameraTransform, Sprite* pSprite, float lifeTime);
	Particle(Transform* pTransform, Rigidbody* pRb, Line* pLine, Transform* pCameraTransform, Sprite* pSprite, float lifeTime);
	~Particle();
	void Initialize() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void Show() override;
	Json Serialize() const override;
	static std::unique_ptr<Particle> Deserialize(const Json& j, Transform* pTransform);
	void SetIsLine(bool which) { m_isLine = which; }
	bool GetIsLine() { return m_isLine; }
	void Restart();
	void UpdateLife();

private:
	Particle() = delete;
	void drawLine();
	Transform* const m_pTransform = nullptr;
	Rigidbody* const m_pRb = nullptr;
	Line* const m_pLine = nullptr;
	const Transform* const m_pCameraTransform = nullptr;
	bool m_isLine = false;
	float m_lifeTime = 3.0f;
	float m_timer = 0;
	Sprite* const m_pSprite = nullptr;
	const Vec4 m_color;
};