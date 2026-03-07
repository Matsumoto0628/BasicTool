#include "test_scene.h"
#include "render_context.h"
#include "camera.h"
#include <memory>
#include <vector>
#include "camera_controller.h"
#include "particle_controller.h"
#include "gui.h"
#include "game_input.h"

TestScene::TestScene(HWND hWnd, RenderContext* const pContext)
	: Scene{ hWnd, pContext }
{
}

TestScene::~TestScene() 
{
}

void TestScene::Initialize()
{
	m_pGui = std::make_unique<Gui>(GetWnd(), getContext(), &getGameObjects());
	m_pGui->Initialize();
}

void TestScene::Setup()
{
	{
		auto& cameraGameObject = Instantiate("Camera");
		auto& camera = cameraGameObject.AddComponent<Camera>(&cameraGameObject.GetTransform(), getContext()->GetWidth(), getContext()->GetHeight());
		cameraGameObject.GetTransform().SetPosition({ 0,0,-50 });

		auto& particleControllerGameObject = Instantiate("ParticleController");
		particleControllerGameObject.AddComponent<ParticleController>(&particleControllerGameObject.GetTransform(), getContext(), &camera, &cameraGameObject.GetTransform());
	}
}

void TestScene::Start()
{
	setIsRuntime(true);

	for (auto& pGameObject : getGameObjects())
	{
		pGameObject->Start();
	}
}

void TestScene::Update()
{
	for (auto& pGameObject : getGameObjects())
	{
		pGameObject->Update();
	}

	m_pGui->Update();

	destroy();
}

void TestScene::Draw()
{
	for (auto& pGameObject : getGameObjects())
	{
		pGameObject->Draw();
	}
}

void TestScene::Terminate()
{
	m_pGui->Finalize();
	for (auto& pGameObject : getGameObjects())
	{
		pGameObject->Finalize();
	}

	m_pGui.reset();
	clearGameObjects();

	setIsRuntime(false);
}

void TestScene::Finalize()
{
}