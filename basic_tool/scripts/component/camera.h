#pragma once
#include "component.h"
#include "transform.h"

class RenderContext;

class Camera : public Component
{
public:
	Camera(Transform* pTransform, unsigned int width, unsigned int height);
	~Camera();
	void Initialize() override;
	void Start() override;
	void Update() override;
	void Terminate() override;
	void Finalize() override;
	const Mat4x4& GetView() const { return m_pTransform->GetMatrix().Inverse(); };
	const Mat4x4& GetProj() const { return m_proj; };

private:
	Camera() = delete;
	Mat4x4 m_view;
	Mat4x4 m_proj;
	float m_aspect;
	float m_nearZ = 0.0f;
	float m_farZ = 0.0f;
	float m_fov = 0.0f;
	Transform* m_pTransform = nullptr;
};