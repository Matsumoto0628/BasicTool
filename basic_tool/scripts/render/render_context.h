#pragma once

class RenderContext
{
public:
    explicit RenderContext(HWND hWnd);
    ~RenderContext();
    bool Initialize();
    void Start();
    void Update();
    void Draw();
    void Terminate();
    void Finalize();
    void Swap();
    ID3D11Device* GetDevice() const { return m_pDevice.Get(); }
    ID3D11DeviceContext* GetDeviceContext() const { return m_pDeviceContext.Get(); }
    unsigned int GetWidth() const { return m_screenWidth; }
    unsigned int GetHeight() const { return m_screenHeight; }

private:
    bool initDeviceAndSwapChain();
    bool initBackBuffer();
    bool initDepthStencilView();
    void initViewPort();

    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;
    D3D11_VIEWPORT m_viewPort[1];

    static const UINT FEATURE_LEVELS_NUM = 2;
    D3D_FEATURE_LEVEL m_pFeatureLevels[FEATURE_LEVELS_NUM] = {};
    D3D_FEATURE_LEVEL m_featureLevelsSupported;

    static const UINT BACK_BUFFER_NUM = 3;
    UINT m_screenWidth = 0;
    UINT m_screenHeight = 0;

    HWND m_hWnd = nullptr;

    static const Vec4 BACK_BUFFER_COLOR;
};