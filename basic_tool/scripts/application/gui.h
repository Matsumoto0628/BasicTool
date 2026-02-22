#pragma once
#include <string>
#include <vector>
#include <memory>

class RenderContext;
class GameObject;

class Gui 
{
public:
	Gui(HWND hWnd, const RenderContext* const pContext, std::vector<std::unique_ptr<GameObject>>* ppGameObjects);
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
	std::vector<std::unique_ptr<GameObject>>* m_ppGameObjects = nullptr;
	const GameObject* m_pSelectGameObject = nullptr;
};