#pragma once
#include "component.h"

class RenderContext;
class Camera;
class Transform;

class Renderer : public Component
{
public:
	virtual ~Renderer() = default; // virtualにしないと派生のデストラクタが呼び出されない

protected:
	Renderer(RenderContext* pContext, Camera* pCamera, Transform* pTransform); // 派生クラスから呼び出す
	Renderer(RenderContext* pContext, Camera* pCamera, Transform* pTransform, D3D11_PRIMITIVE_TOPOLOGY topology);
	virtual bool initDepthStencil();
	virtual bool initBlend();
	virtual bool initRasterizer();
	virtual bool initVertexShader();
	virtual bool initInputLayout(ID3DBlob* vsBlob);
	virtual bool initPixelShader();
	virtual bool initVertexBuffer() = 0;
	virtual bool initIndexBuffer() = 0;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendState = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizerState = nullptr;

	D3D11_PRIMITIVE_TOPOLOGY m_topology;

	RenderContext* m_pContext = nullptr;
	Camera* m_pCamera = nullptr;
	Transform* m_pTransform = nullptr;

private:
	Renderer() = delete; // 必ずRenderContextを渡して初期化
};