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

private:
	CameraController() = delete;
	Transform* m_pTransform;

	// 定数
	const static float m_speed;
};