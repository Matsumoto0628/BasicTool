#include "scene.h"
#include "render_context.h"
#include "camera.h"
#include <fstream>
#include <nlohmann/json.hpp>
using Json = nlohmann::ordered_json;

bool Scene::s_isRuntime = false;

Scene::Scene(HWND hWnd, RenderContext* const pContext)
	: m_hWnd{ hWnd }, m_pContext{ pContext }
{
}

GameObject& Scene::Instantiate(std::string name, bool isSerialize)
{
	const bool serializable = (!s_isRuntime || isSerialize); // 非実行中　または 明示的にtrueなら
	std::unique_ptr<GameObject> pGameObject = std::make_unique<GameObject>(name, serializable);
	pGameObject->Initialize();
	if (s_isRuntime) pGameObject->Start();
	GameObject& ref = *pGameObject;
	m_pGameObjects.push_back(std::move(pGameObject));
	return ref;
}

GameObject& Scene::Instantiate(uint64_t id, std::string name, bool isSerialize)
{
	const bool serializable = (!s_isRuntime || isSerialize); // 非実行中　または 明示的にtrueなら
	std::unique_ptr<GameObject> pGameObject = std::make_unique<GameObject>(id, name, serializable);
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

GameObject* const Scene::FindGameObject(std::string name) const
{
	for (auto& pGameObject : m_pGameObjects)
	{
		if (pGameObject->GetName() == name)
		{
			return pGameObject.get();
		}
	}
	return nullptr;
}

void Scene::Serialize(std::string sceneName)
{
	Json j;
	j[sceneName] = Json::array();
	for (auto& pGameObject : getGameObjects())
	{
		if (!pGameObject->GetTransform().GetParent() && pGameObject->GetIsSerialize())
		{
			j[sceneName].push_back(pGameObject->Serialize());
		}
	}

	std::ofstream file{ sceneName + ".json"};
	file << j.dump(4);
}

void Scene::Deserialize(std::string sceneName)
{
	Terminate();
	Initialize();

	deserialize(sceneName);

	Start();
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

void Scene::deserialize(std::string sceneName)
{
	std::ifstream file{ sceneName + ".json" };
	Json j;
	file >> j;
	for (const auto& gameObjectJson : j[sceneName])
	{
		auto& gameObject = Instantiate(gameObjectJson.at("id").get<uint64_t>(), gameObjectJson.at("name").get<std::string>());
		gameObject.Deserialize(gameObjectJson);
	}
}