#include "test_scene.h"
#include "render_context.h"
#include "camera.h"
#include <memory>
#include <vector>
#include "sprite.h"
#include "line.h"
#include "rigidbody.h"
#include "particle.h"
#include "game_random.h"
#include "camera_controller.h"

TestScene::TestScene(RenderContext* pContext)
	: Scene(pContext)
{
}

TestScene::~TestScene() 
{
	m_pContext = nullptr;
}

void TestScene::Initialize()
{
	std::unique_ptr<GameObject> pCameraGameObject = std::make_unique<GameObject>();
	auto& camera = pCameraGameObject->AddComponent<Camera>(&pCameraGameObject->GetTransform(), m_pContext->GetWidth(), m_pContext->GetHeight());
	pCameraGameObject->AddComponent<CameraController>(&pCameraGameObject->GetTransform());
	pCameraGameObject->GetTransform().SetPosition({ 0,0,-10 });
	pCameraGameObject->Initialize();
	
	for (int i = 0; i < 100; i++)
	{
		std::unique_ptr<GameObject> pSpriteGameObject = std::make_unique<GameObject>();
		pSpriteGameObject->GetTransform().SetScale({ 0.1f,0.1f,0.1f });
		pSpriteGameObject->AddComponent<Sprite>(m_pContext, &camera, &pSpriteGameObject->GetTransform(), Vec4{1.5,1.5,1.5,1});
		auto& line = pSpriteGameObject->AddComponent<Line>(m_pContext, &camera, Vec4{ 1,0,0,1 });
		auto& rb = pSpriteGameObject->AddComponent<Rigidbody>(&pSpriteGameObject->GetTransform());
		rb.AddForce({ 
			GameRandom::GetRange(-1.0f, 1.0f),
			GameRandom::GetRange(-1.0f, 1.0f),
			GameRandom::GetRange(-1.0f, 1.0f)
		});
		m_pRigidbodies.push_back(&rb);
		pSpriteGameObject->AddComponent<Particle>(&pSpriteGameObject->GetTransform(), &rb, &line, &pCameraGameObject->GetTransform());
		pSpriteGameObject->Initialize();
		m_pGameObjects.push_back(std::move(pSpriteGameObject));
	}

	m_pGameObjects.push_back(std::move(pCameraGameObject));
}

void TestScene::Start()
{
	for (auto& pGameObject : m_pGameObjects)
	{
		pGameObject->Start();
	}
}

void TestScene::Update()
{
	for (auto& pGameObject : m_pGameObjects)
	{
		pGameObject->Update();
	}

	for (auto& pRigidbody : m_pRigidbodies)
	{
		//pRigidbody->AddForce({0,-9.8f,0});
	}
}

void TestScene::Terminate()
{
	for (auto& pGameObject : m_pGameObjects)
	{
		pGameObject->Terminate();
	}
}

void TestScene::Finalize()
{
	for (auto& pGameObject : m_pGameObjects)
	{
		pGameObject->Finalize();
	}
}
