#include "test_scene.h"
#include "render_context.h"
#include "camera.h"
#include <memory>
#include <vector>
#include "sphere.h"
#include "square.h"

TestScene::TestScene(RenderContext* pContext, Camera* pCamera)
	: Scene(pContext, pCamera)
{
}

TestScene::~TestScene() 
{
	m_pContext = nullptr;
	m_pCamera = nullptr;
}

void TestScene::Initialize()
{
	std::unique_ptr<GameObject> pGameObject = std::make_unique<GameObject>();
	pGameObject->Initialize();

	m_pGameObjects.push_back(std::move(pGameObject));
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
