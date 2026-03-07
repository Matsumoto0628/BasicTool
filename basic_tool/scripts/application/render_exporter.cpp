#include "render_exporter.h"
#include "DirectXTex.h"
#include <wincodec.h>
#include "render_context.h"
#include <string>
#include "game_time.h"
#include "game_input.h"
#include <windows.h>

// 定数
const float RenderExporter::SHEET_INTERVAL = 0.3f;

RenderExporter::RenderExporter(RenderContext* pContext)
    : m_pContext{pContext}
{
}

RenderExporter::~RenderExporter()
{
}

bool RenderExporter::Initialize()
{
    bool result = initSheet();
    if (!result) 
    {
        return false;
    }

    initBox();

    return true;
}

void RenderExporter::Update()
{
    if (m_isExport) 
    {
        m_sheetTimer += GameTime::GetDeltaTime();
        if (m_sheetTimer >= SHEET_INTERVAL)
        {
            m_sheetTimer = 0;
            updateSheet();
        }
    }
}

void RenderExporter::Finalize()
{
}

void RenderExporter::Export(std::wstring sheetName, UINT numW, UINT numH)
{
    if (!m_isExport)
    {
        m_isExport = true;
        m_sheetNumW = max(numW, 1);
        m_sheetNumH = max(numH, 1);
        m_sheetName = sheetName;
        initSheet();
    }
}

bool RenderExporter::initSheet()
{
    const UINT sheetW = WIDTH * m_sheetNumW;
    const UINT sheetH = HEIGHT * m_sheetNumH;

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = sheetW;
    desc.Height = sheetH;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = 0;

    return SUCCEEDED(m_pContext->GetDevice()->CreateTexture2D(&desc, nullptr, &m_pSheetTexture));
}

void RenderExporter::initBox()
{
    m_box.left = CROP_X;
    m_box.top = CROP_Y;
    m_box.front = 0;
    m_box.right = CROP_X + WIDTH;
    m_box.bottom = CROP_Y + HEIGHT;
    m_box.back = 1;
}

void RenderExporter::updateSheet()
{
    const UINT sheetNum = m_sheetNumH * m_sheetNumW;

    if (m_sheetIndex >= sheetNum)
    {
        return;
    }

    int x = m_sheetIndex % m_sheetNumW;
    int y = m_sheetIndex / m_sheetNumW;
    m_pContext->GetDeviceContext()->CopySubresourceRegion(
        m_pSheetTexture.Get(),
        0,
        x * WIDTH,
        y * HEIGHT,
        0,
        m_pContext->GetExportTexture(),
        0,
        &m_box
    );

    m_sheetIndex++;
    if (m_sheetIndex == sheetNum)
    {
        m_sheetIndex = 0;
        m_isExport = false;
        m_pContext->GetDeviceContext()->Flush();

        // exeと同じ階層のexportフォルダに保存
        {
            std::filesystem::path exeDir = getExeDirectory();
            std::filesystem::path exportDir = exeDir / L"export";
            std::filesystem::create_directories(exportDir);
            std::filesystem::path filePath = exportDir / (m_sheetName + L".png");
            save(m_pSheetTexture.Get(), filePath.wstring());
        }
    }
}

bool RenderExporter::save(ID3D11Texture2D* const texture, const std::wstring& fileName)
{
    D3D11_TEXTURE2D_DESC desc;
    texture->GetDesc(&desc);
    
    D3D11_TEXTURE2D_DESC stagingDesc = desc;
    stagingDesc.Usage = D3D11_USAGE_STAGING;
    stagingDesc.BindFlags = 0;
    stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    stagingDesc.MiscFlags = 0;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> stagingTexture;

    // テクスチャ作成してコピーする
    {
        HRESULT hr = m_pContext->GetDevice()->CreateTexture2D(
            &stagingDesc,
            nullptr,
            &stagingTexture);
        if (FAILED(hr))
        {
            return false;
        }

        m_pContext->GetDeviceContext()->CopyResource(
            stagingTexture.Get(),
            texture);
    }
    
    m_pContext->GetDeviceContext()->Flush();

    // テクスチャから画像を出力
    {
        DirectX::ScratchImage image;
        HRESULT hr = DirectX::CaptureTexture(
            m_pContext->GetDevice(),
            m_pContext->GetDeviceContext(),
            stagingTexture.Get(),
            image);

        if (FAILED(hr))
        {
            return false;
        }

        hr = DirectX::SaveToWICFile(
            image.GetImages(),
            image.GetImageCount(),
            DirectX::WIC_FLAGS_NONE,
            GUID_ContainerFormatPng,
            fileName.c_str());

        return SUCCEEDED(hr);
    }
}

std::filesystem::path RenderExporter::getExeDirectory()
{
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(nullptr, path, MAX_PATH);

    return std::filesystem::path(path).parent_path();
}