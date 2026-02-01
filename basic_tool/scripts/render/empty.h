#pragma once
#include "renderable.h"
#include <memory>

class RenderContext;

class Empty : public Renderable
{
public:
    Empty(std::shared_ptr<RenderContext> pContext);
    Empty(std::shared_ptr<RenderContext> pContext, D3D11_PRIMITIVE_TOPOLOGY topology);
    ~Empty() override;
    bool Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Terminate() override;
    void Finalize() override;
};