#include "test_scene.h"
#include "render_context.h"
#include "camera.h"
#include <memory>
#include <vector>
#include "sphere.h" // 実体を持たないとrenderableに変換できない

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
	std::unique_ptr<Renderable> pSphereA = std::make_unique<Sphere>(m_pContext, m_pCamera, Vec4{ 1, 0, 0, 1 });
	pSphereA->Initialize();

	std::unique_ptr<Renderable> pSphereB = std::make_unique<Sphere>(m_pContext, m_pCamera, Vec4{ 0, 1, 0, 1 });
	pSphereB->Initialize();

	m_pRenderables.push_back(std::move(pSphereA));
	m_pRenderables.push_back(std::move(pSphereB));
}

void TestScene::Start()
{
	for (auto& pRenderable : m_pRenderables) 
	{
		pRenderable->Start();
	}

	Transform* pTransformA = &(m_pRenderables[0]->GetTransform());
	pTransformA->SetPosition({0,0,10});
	Transform* pTransformB = &(m_pRenderables[1]->GetTransform());
	pTransformB->SetParent(pTransformA);
	pTransformB->SetLocalPosition({ 3,0,0 });
	pTransformA->SetScale({ 0.5f,0.5f,0.5f });
}

void TestScene::Update()
{
	for (auto& pRenderable : m_pRenderables)
	{
		pRenderable->Update();
	}

	static float time = 0;
	time += 0.017f;
	Transform* pTransformA = &(m_pRenderables[0]->GetTransform());
	pTransformA->SetRotation(EulerToQuaternion({0, time, 0}));
}

void TestScene::Draw()
{
	for (auto& pRenderable : m_pRenderables)
	{
		pRenderable->Draw();
	}
}

void TestScene::Terminate()
{
	for (auto& pRenderable : m_pRenderables)
	{
		pRenderable->Terminate();
	}
}

void TestScene::Finalize()
{
	for (auto& pRenderable : m_pRenderables)
	{
		pRenderable->Finalize();
	}
}
