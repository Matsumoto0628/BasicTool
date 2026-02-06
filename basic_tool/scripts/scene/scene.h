#pragma once
#include <memory>
#include <vector>
#include "renderable.h"

class RenderContext;
class Camera;

class Scene
{
public:
	virtual ~Scene() = default;
	virtual void Initialize() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Terminate() = 0;
	virtual void Finalize() = 0;

protected:
	Scene(RenderContext* pContext, Camera* pCamera);
	RenderContext* m_pContext;
	Camera* m_pCamera;
	std::vector<std::unique_ptr<Renderable>> m_pRenderables;
	
private:
	Scene() = delete;
};