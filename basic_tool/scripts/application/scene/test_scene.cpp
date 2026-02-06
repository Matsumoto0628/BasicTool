#include "test_scene.h"
#include "render_context.h"
#include "camera.h"
#include <memory>
#include <vector>
#include "square.h"
#include "rigidbody.h"

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
	pCameraGameObject->GetTransform().SetPosition({ 0,0,-10 });
	pCameraGameObject->Initialize();
	m_pGameObjects.push_back(std::move(pCameraGameObject));

	std::unique_ptr<GameObject> pSphereGameObject = std::make_unique<GameObject>();
	pSphereGameObject->AddComponent<Square>(m_pContext, &camera, &pSphereGameObject->GetTransform());
	auto& rb = pSphereGameObject->AddComponent<Rigidbody>(&pSphereGameObject->GetTransform());
	m_pRigidbodies.push_back(&rb);
	pSphereGameObject->Initialize();
	m_pGameObjects.push_back(std::move(pSphereGameObject));
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
		pRigidbody->AddForce({0,-9.8f,0});
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
