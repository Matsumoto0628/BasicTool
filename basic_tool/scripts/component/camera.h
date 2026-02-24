#pragma once
#include "component.h"
#include "transform.h"

class RenderContext;

class Camera : public Component
{
public:
	Camera(uint64_t id, const Transform* const pTransform, float aspect);
	Camera(const Transform* const pTransform, unsigned int width, unsigned int height);
	~Camera();
	void Initialize() override;
	void Start() override;
	void Update() override;
	void Finalize() override;
	void Show() override;
	Json Serialize() const override;
	static std::unique_ptr<Camera> Deserialize(const Json& j, const Transform* const pTransform);
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
	const Transform* const m_pTransform = nullptr;
};