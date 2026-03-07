#pragma once
#include <string>
#include <vector>
#include <memory>

class RenderContext;
class GameObject;

class Gui 
{
public:
	Gui(HWND hWnd, const RenderContext* const pContext, const std::vector<std::unique_ptr<GameObject>>* ppGameObjects);
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
	void drawSerializePopup();
	std::string openFileDialog();
	void checkOpenFile();
	void checkExit();
	void drawExportPopup();
	void drawPlaybackControl();

	HWND m_hWnd = nullptr; 
	const RenderContext* const m_pContext = nullptr;
	const std::vector<std::unique_ptr<GameObject>>* m_ppGameObjects;
	const GameObject* m_pSelectGameObject = nullptr;
	char m_serializeFileName[256] = "scene";
	char m_exportFileName[256] = "sheet";
	int m_exportNumW = 3;
	int m_exportNumH = 3;
	bool m_openPopupRequestd = false;
	bool m_openFileRequested = false;
	bool m_exitRequested = false;
	bool m_openExportRequested = false;
	bool m_isPause = false;
};