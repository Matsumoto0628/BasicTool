#include "test_scene.h"
#include "render_context.h"
#include "camera.h"
#include <memory>
#include <vector>
#include "camera_controller.h"
#include "particle_controller.h"
#include "gui.h"

TestScene::TestScene(HWND hWnd, const RenderContext* const pContext)
	: Scene{ hWnd, pContext }
{
}

TestScene::~TestScene() 
{
}

void TestScene::Initialize()
{
}

void TestScene::Start()
{
	{
		m_pGui = std::make_unique<Gui>(m_hWnd, m_pContext, &m_pGameObjects);
		m_pGui->Initialize();
	}

	{
		auto& cameraGameObject = Instantiate("Camera");
		auto& camera = cameraGameObject.AddComponent<Camera>(&cameraGameObject.GetTransform(), m_pContext->GetWidth(), m_pContext->GetHeight());
		//cameraGameObject.AddComponent<CameraController>(&cameraGameObject.GetTransform());
		cameraGameObject.GetTransform().SetPosition({ 0,0,-10 });

		auto& particleControllerGameObject = Instantiate("ParticleController");
		particleControllerGameObject.AddComponent<ParticleController>(&particleControllerGameObject.GetTransform(), m_pContext, &camera, &cameraGameObject.GetTransform());
	}
}

void TestScene::Update()
{
	for (auto& pGameObject : m_pGameObjects)
	{
		pGameObject->Update();
	}

	m_pGui->Update();

	destroy();
}

void TestScene::Terminate()
{
	m_pGui->Finalize();
	for (auto& pGameObject : m_pGameObjects)
	{
		pGameObject->Finalize();
	}
}

void TestScene::Finalize()
{
}
