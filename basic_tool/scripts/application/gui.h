#pragma once
#include <string>
#include <vector>
#include <memory>

class RenderContext;
class GameObject;

class Gui 
{
public:
	Gui(HWND hWnd, RenderContext* pContext, std::vector<std::unique_ptr<GameObject>>* ppGameObjects);
	~Gui();
	void Initialize();
	void Start();
	void Update();
	void Terminate();
	void Finalize();

private:
	Gui() = delete;
	void drawMainMenu();
	void drawHierarchy();
	void drawInspector();
	void drawHierarchyNode(GameObject* pGameObject);

	HWND m_hWnd = nullptr; 
	RenderContext* m_pContext = nullptr;
	std::vector<std::unique_ptr<GameObject>>* m_ppGameObjects = nullptr;
	GameObject* m_pSelectGameObject = nullptr;
};