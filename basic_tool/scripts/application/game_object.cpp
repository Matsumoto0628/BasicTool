#include "game_object.h"
#include "game_random.h"

GameObject::GameObject(std::string name)
	: m_transform{ this },
	m_id{ GameRandom::GetUUID() },
	m_name{ name }
{
}

GameObject::~GameObject()
{
}

void GameObject::Initialize() 
{
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
		pComponent->Update();
	}
}

void GameObject::Terminate()
{
	for (auto& pComponent : m_pComponents)
	{
		pComponent->Terminate();
	}
}

void GameObject::Finalize()
{
	for (auto& pComponent : m_pComponents)
	{
		pComponent->Finalize();
	}
}