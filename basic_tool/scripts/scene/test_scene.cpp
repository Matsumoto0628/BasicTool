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
	std::unique_ptr<Renderer> pSquare = std::make_unique<Square>(m_pContext, m_pCamera);
	pSquare->Initialize();

	m_pRenderers.push_back(std::move(pSquare));
}

void TestScene::Start()
{
	for (auto& pRenderer : m_pRenderers) 
	{
		pRenderer->Start();
	}

	Transform* pTransform = &(m_pRenderers[0]->GetTransform());
	pTransform->SetPosition({0,0,10});
}

void TestScene::Update()
{
	for (auto& pRenderer : m_pRenderers)
	{
		pRenderer->Update();
	}

	static float time = 0;
	time += 0.017f;
	Transform* pTransform = &(m_pRenderers[0]->GetTransform());
	pTransform->SetRotation(EulerToQuaternion({ 0,time,0 }));

	for (auto& pRenderer : m_pRenderers)
	{
		pRenderer->Draw();
	}
}

void TestScene::Terminate()
{
	for (auto& pRenderer : m_pRenderers)
	{
		pRenderer->Terminate();
	}
}

void TestScene::Finalize()
{
	for (auto& pRenderer : m_pRenderers)
	{
		pRenderer->Finalize();
	}
}
