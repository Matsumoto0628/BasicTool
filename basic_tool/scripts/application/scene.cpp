#include "scene.h"
#include "render_context.h"
#include "camera.h"

Scene::Scene(HWND hWnd, const RenderContext* const pContext)
	: m_hWnd{ hWnd }, m_pContext{ pContext }
{
}

GameObject& Scene::Instantiate(std::string name)
{
	std::unique_ptr<GameObject> pGameObject = std::make_unique<GameObject>(name);
	pGameObject->Initialize();
	pGameObject->Start();
	GameObject& ref = *pGameObject;
	m_pGameObjects.push_back(std::move(pGameObject));
	return ref;
}

void Scene::destroy()
{
	for (auto& pGameObject : m_pGameObjects) 
	{
		pGameObject->Terminate();
		pGameObject->Finalize();
	}

	auto predicate = [](const auto& pGameObject) {
		return pGameObject->GetIsDestroy();
	};

	std::erase_if(m_pGameObjects, predicate);
}