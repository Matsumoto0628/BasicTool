#include "game_object.h"

GameObject::GameObject() 
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