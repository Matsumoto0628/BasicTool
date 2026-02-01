#pragma once
#include "vec4.h"

class Renderable
{
public:
	virtual ~Renderable() = default; // virtualにしないと派生のデストラクタが呼び出されない
	virtual bool Initialize() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Terminate() = 0;
	virtual void Finalize() = 0;

protected:
	Renderable() = default; // Renderable自体を生成することを禁止する
	virtual bool initDepthStencil() = 0; // ほぼ共通なので普通の仮想関数でいいかも
	virtual bool initBlend() = 0;
	virtual void initInputLayout() = 0;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendState = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState = nullptr;

	D3D11_PRIMITIVE_TOPOLOGY m_topology;

	// 定数
	static const Vec4 BLEND_FACTOR;
};