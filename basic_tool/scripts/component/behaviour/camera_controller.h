#pragma once
#include "component.h"

class Transform;

class CameraController : public Component
{
public:
	CameraController(uint64_t id, Transform* pTransform);
	CameraController(Transform* pTransform);
	~CameraController();
	void Initialize() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void Show() override;
	Json Serialize() const override;
	static std::unique_ptr<CameraController> Deserialize(const Json& j, Transform* pTransform);

private:
	CameraController() = delete;
	Transform* const m_pTransform = nullptr;
	POINT m_anchorPos;
	float m_rotX;
	float m_rotY;

	// 定数
	const static float m_speed;
	const static float m_sensitivity;
};