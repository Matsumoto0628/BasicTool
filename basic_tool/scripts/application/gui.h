#pragma once
#include <string>
#include <vector>

class RenderContext;

class Gui 
{
public:
	Gui(HWND hWnd, RenderContext* pContext);
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

	HWND m_hWnd = nullptr; 
	RenderContext* m_pContext = nullptr;

	struct Params 
	{
		float intensity = 1.0f;
		float radius = 0.5f;
		bool enableGlow = true;
	};
	Params m_params;

	struct SceneObject {
		std::string name;
		std::vector<SceneObject> children;
		SceneObject(std::string name) { SceneObject::name = name; }
	};
	void drawHierarchyNode(SceneObject& obj, int& selectedIndex, int& index);
	std::vector<SceneObject> m_sceneObjects;
	int m_selectedIndex;
};