#pragma once
#include "scene.h"

class RenderContext;
class Camera;

class TestScene : public Scene 
{
public:
	TestScene(RenderContext* pContext);
	~TestScene();
	void Initialize() override;
	void Start() override;
	void Update() override;
	void Terminate() override;
	void Finalize() override;

private:
	TestScene() = delete;
};