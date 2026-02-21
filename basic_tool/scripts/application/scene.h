#pragma once
#include <memory>
#include <vector>
#include "game_object.h"

class RenderContext;
class Rigidbody;

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
	Scene(HWND hWnd, const RenderContext* const pContext);
	HWND m_hWnd = nullptr;
	const RenderContext* const m_pContext = nullptr;
	std::vector<std::unique_ptr<GameObject>> m_pGameObjects;
	std::vector<Rigidbody*> m_pRigidbodies;
	
private:
	Scene() = delete;
};