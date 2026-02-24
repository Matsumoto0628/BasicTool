#include "scene.h"
#include "render_context.h"
#include "camera.h"
#include <fstream>
#include <nlohmann/json.hpp>

using Json = nlohmann::ordered_json;

bool Scene::s_isRuntime = false;

Scene::Scene(HWND hWnd, const RenderContext* const pContext)
	: m_hWnd{ hWnd }, m_pContext{ pContext }
{
}

void Scene::serialize()
{
	Json j;
	j["test_scene"] = Json::array();
	for (auto& pGameObject : m_pGameObjects)
	{
		if (!pGameObject->GetTransform().GetParent() && pGameObject->GetIsSerialize())
		{
			j["test_scene"].push_back(pGameObject->Serialize());
		}
	}

	std::ofstream file{ "test_scene.json" };
	file << j.dump(4);
}

void Scene::deserialize()
{
	std::ifstream file{ "test_scene.json" };
	Json j;
	file >> j;
	for (const auto& gameObjectJson : j["test_scene"])
	{
		auto& gameObject = Instantiate(gameObjectJson.at("id").get<uint64_t>(), gameObjectJson.at("name").get<std::string>());
		gameObject.Deserialize(gameObjectJson);
	}
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