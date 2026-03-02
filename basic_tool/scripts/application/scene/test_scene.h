#pragma once
#include "scene.h"

class RenderContext;
class Gui;

class TestScene : public Scene 
{
public:
	TestScene(HWND hWnd, const RenderContext* const pContext);
	~TestScene();
	void Initialize() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void Terminate() override;
	void Finalize() override;

private:
	TestScene() = delete;
	void serialize();
	void deserialize();
	std::unique_ptr<Gui> m_pGui = nullptr;
};