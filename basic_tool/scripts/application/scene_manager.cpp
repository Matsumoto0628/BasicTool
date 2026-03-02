#include "scene_manager.h"
#include "test_scene.h"

Scene* SceneManager::s_pCurrentScene = nullptr;
std::map<SceneManager::SceneType, std::unique_ptr<Scene>> SceneManager::s_pScenes;

SceneManager::SceneManager(HWND hWnd, RenderContext* pContext)
{
	s_pScenes.emplace(SceneType::Test, std::make_unique<TestScene>(hWnd, pContext));
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	for (auto& [key, value] : s_pScenes)
	{
		value->Initialize();
	}

	Transit(SceneType::Test);
}

void SceneManager::Update()
{
	if (s_pCurrentScene)
	{
		s_pCurrentScene->Update();
	}
}

void SceneManager::Draw()
{
	if (s_pCurrentScene)
	{
		s_pCurrentScene->Draw();
	}
}

void SceneManager::Finalize()
{
	for (auto& [key, value] : s_pScenes)
	{
		value->Finalize();
	}
}

void SceneManager::Transit(SceneType sceneType)
{
	if (s_pCurrentScene)
	{
		s_pCurrentScene->Terminate();
	}
	s_pCurrentScene = s_pScenes[sceneType].get();
	s_pCurrentScene->Start();
}

