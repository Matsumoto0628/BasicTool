#pragma once
#include <memory>
#include <vector>
#include <string>
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
	GameObject& Instantiate(std::string name);

protected:
	Scene(HWND hWnd, const RenderContext* const pContext);
	void destroy();
	HWND m_hWnd = nullptr;
	const RenderContext* const m_pContext = nullptr;
	std::vector<std::unique_ptr<GameObject>> m_pGameObjects;
	std::vector<Rigidbody*> m_pRigidbodies;
	
private:
	Scene() = delete;
};