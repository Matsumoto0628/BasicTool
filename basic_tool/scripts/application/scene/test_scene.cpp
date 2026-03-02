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
		m_pGui = std::make_unique<Gui>(getWnd(), getContext(), &getGameObjects());
		m_pGui->Initialize();
	}

	{
		auto& cameraGameObject = Instantiate("Camera");
		auto& camera = cameraGameObject.AddComponent<Camera>(&cameraGameObject.GetTransform(), getContext()->GetWidth(), getContext()->GetHeight());
		//cameraGameObject.AddComponent<CameraController>(&cameraGameObject.GetTransform());
		cameraGameObject.GetTransform().SetPosition({ 0,0,-10 });

		auto& particleControllerGameObject = Instantiate("ParticleController");
		particleControllerGameObject.AddComponent<ParticleController>(&particleControllerGameObject.GetTransform(), getContext(), &camera, &cameraGameObject.GetTransform());
	}

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

	if (GameInput::GetKeyDown('S') && GameInput::GetKey(VK_CONTROL)) 
	{
		serialize();
	}

	if (GameInput::GetKeyDown('D') && GameInput::GetKey(VK_CONTROL))
	{
		deserialize();
	}

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

void TestScene::serialize()
{
	Json j;
	j["test_scene"] = Json::array();
	for (auto& pGameObject : getGameObjects())
	{
		if (!pGameObject->GetTransform().GetParent() && pGameObject->GetIsSerialize())
		{
			j["test_scene"].push_back(pGameObject->Serialize());
		}
	}

	std::ofstream file{ "test_scene.json" };
	file << j.dump(4);
}

void TestScene::deserialize()
{
	Terminate();

	{
		m_pGui = std::make_unique<Gui>(getWnd(), getContext(), &getGameObjects());
		m_pGui->Initialize();
	}

	std::ifstream file{ "test_scene.json" };
	Json j;
	file >> j;
	for (const auto& gameObjectJson : j["test_scene"])
	{
		auto& gameObject = Instantiate(gameObjectJson.at("id").get<uint64_t>(), gameObjectJson.at("name").get<std::string>());
		gameObject.Deserialize(gameObjectJson);
	}

	setIsRuntime(true);

	for (auto& pGameObject : getGameObjects())
	{
		pGameObject->Start();
	}
}