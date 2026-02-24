#include "scene.h"
#include "render_context.h"
#include "camera.h"

bool Scene::s_isRuntime = false;

Scene::Scene(HWND hWnd, const RenderContext* const pContext)
	: m_hWnd{ hWnd }, m_pContext{ pContext }
{
}

GameObject& Scene::Instantiate(std::string name)
{
	std::unique_ptr<GameObject> pGameObject = std::make_unique<GameObject>(name, !s_isRuntime);
	pGameObject->Initialize();
	if (s_isRuntime) pGameObject->Start();
	GameObject& ref = *pGameObject;
	m_pGameObjects.push_back(std::move(pGameObject));
	return ref;
}

void Scene::destroy()
{
	for (auto& pGameObject : m_pGameObjects) 
	{
		pGameObject->Finalize();
	}

	auto predicate = [](const auto& pGameObject) {
		return pGameObject->GetIsDestroy();
	};

	std::erase_if(m_pGameObjects, predicate);
}