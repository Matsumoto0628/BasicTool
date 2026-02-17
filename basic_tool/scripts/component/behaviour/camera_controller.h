#pragma once
#include "component.h"

class Transform;

class CameraController : public Component
{
public:
	CameraController(Transform* pTransform);
	~CameraController();
	void Initialize() override;
	void Start() override;
	void Update() override;
	void Terminate() override;
	void Finalize() override;
	void Show() override;

private:
	CameraController() = delete;
	Transform* m_pTransform;
	POINT m_anchorPos;
	float m_rotX;
	float m_rotY;

	// 定数
	const static float m_speed;
	const static float m_sensitivity;
};