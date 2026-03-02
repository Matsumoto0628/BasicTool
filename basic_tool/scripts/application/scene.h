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
	virtual void Draw() = 0;
	virtual void Terminate() = 0;
	virtual void Finalize() = 0;
	GameObject& Instantiate(std::string name, bool isSerialize = false);
	GameObject& Instantiate(uint64_t id, std::string name, bool isSerialize = false);
	GameObject* const FindGameObject(uint64_t id) const;
	Component* const FindComponent(uint64_t id) const;
	const RenderContext* const GetContext() const { return m_pContext; }

protected:
	Scene(HWND hWnd, const RenderContext* const pContext);
	void destroy();
	const HWND& getWnd() const { return m_hWnd; }
	const RenderContext* const getContext() const { return m_pContext; }
	const std::vector<std::unique_ptr<GameObject>>& getGameObjects() const { return m_pGameObjects; }
	void clearGameObjects() { m_pGameObjects.clear(); }
	bool getIsRuntime() const { return s_isRuntime; }
	void setIsRuntime(bool which) { s_isRuntime = which; }
	
private:
	Scene() = delete;
	HWND m_hWnd = nullptr;
	const RenderContext* const m_pContext = nullptr;
	std::vector<std::unique_ptr<GameObject>> m_pGameObjects;
	static bool s_isRuntime; // クラスで共通で持つ
};