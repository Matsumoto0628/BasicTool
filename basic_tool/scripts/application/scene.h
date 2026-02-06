#pragma once
#include <memory>
#include <vector>
#include "game_object.h"

class RenderContext;

class Scene
{
public:
	virtual ~Scene() = default;
	virtual void Initialize() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Terminate() = 0;
	virtual void Finalize() = 0;

protected:
	Scene(RenderContext* pContext);
	RenderContext* m_pContext;
	std::vector<std::unique_ptr<GameObject>> m_pGameObjects;
	
private:
	Scene() = delete;
};