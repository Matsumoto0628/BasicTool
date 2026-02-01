#pragma once
#include "renderable.h"

class Empty : public Renderable
{
public:
    Empty();
    ~Empty() override;
    bool Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Terminate() override;
    void Finalize() override;

protected:
    bool initDepthStencil() override;
    bool initBlend() override;
    void initInputLayout() override;
};