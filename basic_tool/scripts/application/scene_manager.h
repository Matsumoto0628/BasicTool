#pragma once
#include "scene.h"
#include <map>
#include <memory>

class RenderContext;

class SceneManager 
{
public:
	SceneManager(HWND hWnd, RenderContext* pContext);
	~SceneManager();
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	enum class SceneType
	{
		Test,
	};
	static void Transit(SceneType sceneType);
	static Scene* const GetCurrentScene() { return s_pCurrentScene; }

private:
	SceneManager() = delete;
	static Scene* s_pCurrentScene;
	static std::map<SceneType, std::unique_ptr<Scene>> s_pScenes;
};