#pragma once
#include <memory>

class RenderContext;

// TODO: imguiのテスト
// TODO: カメラとライトを実装
// TODO: Transformクラスで位置回転スケール、親子関係を実装
class Renderable
{
public:
	virtual ~Renderable() = default; // virtualにしないと派生のデストラクタが呼び出されない
	virtual bool Initialize() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0; // Draw()かDrawIndexed()を呼び出す
	virtual void Terminate() = 0;
	virtual void Finalize() = 0;

protected:
	Renderable(std::shared_ptr<RenderContext> pContext);
	Renderable(std::shared_ptr<RenderContext> pContext, D3D11_PRIMITIVE_TOPOLOGY topology);
	virtual bool initDepthStencil();
	virtual bool initBlend();
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

	D3D11_PRIMITIVE_TOPOLOGY m_topology;

	std::shared_ptr<RenderContext> m_pContext = nullptr;

	// 定数
	static const Vec4 BLEND_FACTOR;

private:
	Renderable() = delete; // 必ずRenderContextを渡して初期化
};