#include "scene.h"
#include "render_context.h"
#include "camera.h"
#include <nlohmann/json.hpp>

using Json = nlohmann::ordered_json;

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

GameObject& Scene::Instantiate(uint64_t id, std::string name)
{
	std::unique_ptr<GameObject> pGameObject = std::make_unique<GameObject>(id, name, !s_isRuntime);
	pGameObject->Initialize();
	if (s_isRuntime) pGameObject->Start();
	GameObject& ref = *pGameObject;
	m_pGameObjects.push_back(std::move(pGameObject));
	return ref;
}

GameObject* const Scene::FindGameObject(uint64_t id) const
{
	for (auto& pGameObject : m_pGameObjects)
	{
		if (pGameObject->GetID() == id)
		{
			return pGameObject.get();
		}
	}
	return nullptr;
}

Component* const Scene::FindComponent(uint64_t id) const
{
	for (auto& pGameObject : m_pGameObjects)
	{
		for (auto& pComponent : *pGameObject->GetComponents())
		{
			if (pComponent->GetID() == id)
			{
				return pComponent.get();
			}
		}
	}
	return nullptr;
}

void Scene::destroy()
{
	for (auto& pGameObject : m_pGameObjects)
	{
		if (!pGameObject->GetIsDestroy())
		{
			pGameObject->Finalize();
		}
	}

	auto predicate = [](const auto& pGameObject) {
		return pGameObject->GetIsDestroy();
		};

	std::erase_if(m_pGameObjects, predicate);
}