#pragma once
#include "transform.h"

class Camera 
{
public:
	Camera(unsigned int width, unsigned int height);
	~Camera();
	void Initialize();
	const Mat4x4& GetView() const { return m_transform.GetMatrix().Inverse(); };
	const Mat4x4& GetProj() const { return m_proj; };

private:
	Camera() = delete;
	void initProj();
	Transform m_transform;
	Mat4x4 m_view;
	Mat4x4 m_proj;
	float m_aspect;
	float m_nearZ = 0.0f;
	float m_farZ = 0.0f;
	float m_fov = 0.0f;
};