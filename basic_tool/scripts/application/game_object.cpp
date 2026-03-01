#include "game_object.h"
#include "game_random.h"
#include "scene_manager.h"
#include "camera.h"
#include "particle_controller.h"
#include "particle.h"
#include "camera_controller.h"
#include "rigidbody.h"
#include "line.h"
#include "sphere.h"
#include "sprite.h"
#include "square.h"
#include "triangle_test.h"

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

void GameObject::Draw()
{
	for (auto& pComponent : m_pComponents)
	{
		if (pComponent->GetEnabled())
		{
			pComponent->Draw();
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
	j["components"] = Json::array();
	for (const auto& pComponent : m_pComponents)
	{
		j["components"].push_back(pComponent->Serialize());
	}
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

	for (const auto& componentJson : j["components"])
	{
		deserializeComponent(componentJson);
	}

	for (const auto& childJson : j["children"])
	{
		auto& child = SceneManager::GetCurrentScene()->Instantiate(childJson.at("id").get<uint64_t>(), childJson.at("name").get<std::string>());
		child.Deserialize(childJson);
		child.GetTransform().SetParent(&m_transform);
	}
}

void GameObject::deserializeComponent(const Json& j)
{
	Component::Type type = j.at("type").get<Component::Type>();
	switch (type)
	{
	case Component::Type::None:
		break;
	case Component::Type::Camera:
	{
		auto component = Camera::Deserialize(j, &m_transform);
		component->Initialize();
		m_pComponents.push_back(std::move(component));
		break;
	}
	case Component::Type::Rigidbody:
	{
		auto component = Rigidbody::Deserialize(j, &m_transform);
		component->Initialize();
		m_pComponents.push_back(std::move(component));
		break;
	}
	case Component::Type::Triangle:
	{
		auto component = TriangleTest::Deserialize(j, SceneManager::GetCurrentScene()->GetContext(), &m_transform);
		component->Initialize();
		m_pComponents.push_back(std::move(component));
		break;
	}
	case Component::Type::Square:
	{
		auto component = Square::Deserialize(j, SceneManager::GetCurrentScene()->GetContext(), &m_transform);
		component->Initialize();
		m_pComponents.push_back(std::move(component));
		break;
	}
	case Component::Type::Line:
	{
		auto component = Line::Deserialize(j, SceneManager::GetCurrentScene()->GetContext());
		component->Initialize();
		m_pComponents.push_back(std::move(component));
		break;
	}
	case Component::Type::Sphere:
	{
		auto component = Sphere::Deserialize(j, SceneManager::GetCurrentScene()->GetContext(), &m_transform);
		component->Initialize();
		m_pComponents.push_back(std::move(component));
		break;
	}
	case Component::Type::CameraController:
	{
		auto component = CameraController::Deserialize(j, &m_transform);
		component->Initialize();
		m_pComponents.push_back(std::move(component));
		break;
	}
	case Component::Type::ParticleController:
	{
		auto component = ParticleController::Deserialize(j, &m_transform, SceneManager::GetCurrentScene()->GetContext());
		component->Initialize();
		m_pComponents.push_back(std::move(component));
		break;
	}
	case Component::Type::Particle:
	{
		auto component = Particle::Deserialize(j, &m_transform);
		component->Initialize();
		m_pComponents.push_back(std::move(component));
		break;
	}
	case Component::Type::Sprite:
	{
		auto component = Sprite::Deserialize(j, SceneManager::GetCurrentScene()->GetContext(), &m_transform);
		component->Initialize();
		m_pComponents.push_back(std::move(component));
		break;
	}
	default:
		break;
	}
}