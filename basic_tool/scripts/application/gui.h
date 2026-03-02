#pragma once
#include <string>
#include <vector>
#include <memory>

class RenderContext;
class GameObject;

class Gui 
{
public:
	Gui(HWND hWnd, const RenderContext* const pContext, const std::vector<std::unique_ptr<GameObject>>& pGameObjects);
	~Gui();
	void Initialize();
	void Update();
	void Finalize();

private:
	Gui() = delete;
	void drawMainMenu();
	void drawHierarchy();
	void drawInspector();
	void drawHierarchyNode(GameObject* pGameObject);

	HWND m_hWnd = nullptr; 
	const RenderContext* const m_pContext = nullptr;
	const std::vector<std::unique_ptr<GameObject>>& m_pGameObjects;
	const GameObject* m_pSelectGameObject = nullptr;
};