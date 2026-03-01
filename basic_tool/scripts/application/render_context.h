#pragma once
#include <string>

class RenderContext
{
public:
    explicit RenderContext(HWND hWnd);
    ~RenderContext();
    bool Initialize();
    void Update();
    void Finalize();
    void ClearRtv();
    void SetRtvHDR();
    void SetRtv();
    void PostEffect();
    void Swap();
    void CheckSave();
    ID3D11Device* GetDevice() const { return m_pDevice.Get(); }
    ID3D11DeviceContext* GetDeviceContext() const { return m_pDeviceContext.Get(); }
    unsigned int GetWidth() const { return m_screenWidth; }
    unsigned int GetHeight() const { return m_screenHeight; }

private:
    bool initDeviceAndSwapChain();
    bool initBackBuffer();
    bool initDepthStencilView();
    void initViewPort();
    bool initHDR();
    bool initBloom();
    bool initConstantBufferBlur();
    bool initVertexShader();
    bool initPixelShader();
    bool initSampler();
    void updateConstantBufferBlur(const Vec2& dir);
    void drawFullscreen();
    bool saveBackBuffer(const std::wstring& fileName);
    bool initExport();

    struct ConstantBufferBlur // Vecは使えない
    {
        float texelSize[2];
        float dir[2];
    };

    // 定数
    static const UINT BACK_BUFFER_NUM = 3;
    static const UINT FEATURE_LEVELS_NUM = 2;
    static const Vec4 BACK_BUFFER_COLOR;

    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetViewHDR = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceViewHDR = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetViewBloomA = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetViewBloomB = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceViewBloomA = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceViewBloomB = nullptr;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShaderFullscreen = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShaderExtract = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShaderBlur = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShaderComposite = nullptr;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerLinear = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBufferBlur = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetViewExport = nullptr;
    D3D11_VIEWPORT m_viewPort[1];
    D3D11_VIEWPORT m_viewPortBloom[1];
    D3D_FEATURE_LEVEL m_pFeatureLevels[FEATURE_LEVELS_NUM] = {};
    D3D_FEATURE_LEVEL m_featureLevelsSupported;
    UINT m_screenWidth = 0;
    UINT m_screenHeight = 0;
    HWND m_hWnd = nullptr;
    bool m_isExport = false;
};