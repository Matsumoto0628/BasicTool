#pragma once

class Gui 
{
public:
	Gui(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~Gui();
	void Initialize();
	void Start();
	void Update();
	void Draw();
	void Terminate();
	void Finalize();

private:
	HWND m_hWnd = nullptr; // HWNDを参照として持つのは危険かもしれない
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;
};