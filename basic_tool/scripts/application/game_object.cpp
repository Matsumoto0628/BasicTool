#include "game_object.h"
#include "game_random.h"

GameObject::GameObject(std::string name, bool isSerialize)
	: m_transform{ this },
	m_isSerialize{ isSerialize },
	m_id{ GameRandom::GetUUID() },
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
