#include "test_scene.h"
#include "render_context.h"
#include "camera.h"
#include <memory>
#include <vector>
#include "camera_controller.h"
#include "particle_controller.h"
#include "gui.h"
#include "game_input.h"
#include <fstream>
#include <nlohmann/json.hpp>
using Json = nlohmann::ordered_json;

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

	s_isRuntime = true;

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

	if (GameInput::GetKeyDown('S') && GameInput::GetKey(VK_CONTROL)) 
	{
		Json j;
		j["test_scene"] = Json::array();
		for (auto& pGameObject : m_pGameObjects) 
		{
			if (!pGameObject->GetTransform().GetParent() && pGameObject->GetIsSerialize()) 
			{
				j["test_scene"].push_back(pGameObject->Serialize());
			}
		}

		std::ofstream file("test_scene.json");
		file << j.dump(4);
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

	s_isRuntime = false;
}

void TestScene::Finalize()
{
}
