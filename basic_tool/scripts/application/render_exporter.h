#pragma once

class RenderContext;

class RenderExporter 
{
public:
	explicit RenderExporter(RenderContext* pContext);
	~RenderExporter();
	bool Initialize();
	void Update();
	void Finalize();
	void Export(UINT numW, UINT numH);

private:
	RenderExporter() = delete;
	bool initSheet();
	void initBox();
	void updateSheet();
	bool save(ID3D11Texture2D* const texture, const std::wstring& fileName);
	const RenderContext* const m_pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pSheetTexture = nullptr;
	bool m_isExport = false;
	int m_sheetIndex = 0;
	float m_sheetTimer = 0.0f;
	UINT m_sheetNumW = 1;
	UINT m_sheetNumH = 1;
	D3D11_BOX m_box;

	// 定数
	static const UINT WIDTH = 500;
	static const UINT HEIGHT = 500;
	static const UINT CROP_X = 230;
	static const UINT CROP_Y = 20;
	static const float SHEET_INTERVAL;
};