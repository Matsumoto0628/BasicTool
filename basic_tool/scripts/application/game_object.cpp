#include "game_object.h"
#include "game_random.h"
#include "scene_manager.h"

GameObject::GameObject(std::string name, bool isSerialize)
	: m_transform{ this },
	m_isSerialize{ isSerialize },
	m_id{ GameRandom::GetUUID() },
	m_name{ name }
{
}

GameObject::GameObject(uint64_t id, std::string name, bool isSerialize)
	: m_transform{ this },
	m_isSerialize{ isSerialize },
	m_id{ id },
	m_name{ name }
{
}

GameObject::~GameObject()
{
}

void GameObject::Initialize() 
{
	for (auto& pComponent : m_pComponents)
	{
		pComponent->Initialize();
	}
}

void GameObject::Start()
{
	for (auto& pComponent : m_pComponents)
	{
		pComponent->Start();
	}
}

void GameObject::Update() 
{
	for (auto& pComponent : m_pComponents)
	{
		if (pComponent->GetEnabled()) 
		{
			pComponent->Update();
		}
	}
}

void GameObject::Finalize()
{
	for (auto& pComponent : m_pComponents)
	{
		pComponent->Finalize();
	}
}

void GameObject::Destroy()
{
	m_isDestroy = true;

	for (auto& pChild : m_transform.GetChildren()) 
	{
		pChild->GetGameObject().Destroy();
	}
}

Json GameObject::Serialize() const
{
	Json j;
	j["id"] = m_id;
	j["name"] = m_name;
	j["transform"] = m_transform.Serialize();
	j["children"] = Json::array();
	for (const auto& child : m_transform.GetChildren()) 
	{
		if (child->GetGameObject().m_isSerialize) 
		{
			j["children"].push_back(child->GetGameObject().Serialize());
		}
	}

	return j;
}

void GameObject::Deserialize(const Json& j)
{
	m_transform.Deserialize(j.at("transform"));

	for (const auto& childJson : j["children"])
	{
		auto& child = SceneManager::GetCurrentScene()->Instantiate(childJson.at("id").get<uint64_t>(), childJson.at("name").get<std::string>());
		child.Deserialize(childJson);
		child.GetTransform().SetParent(&m_transform);
	}
}
