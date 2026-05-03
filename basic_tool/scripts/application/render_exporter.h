/**
 * @file render_exporter.h
 * @brief スプライトシートエクスポートクラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once

class RenderContext;

/**
 * @brief レンダリング結果をスプライトシートとして PNG ファイルに書き出すクラス
 *
 * 指定したフレーム数分のレンダリング画像を一定間隔でキャプチャし、
 * 指定した行列数のシートにまとめて保存する。
 */
class RenderExporter
{
public:
    /**
     * @brief コンストラクタ
     * @param pContext レンダリングコンテキスト
     */
    explicit RenderExporter(RenderContext* pContext);
    ~RenderExporter();
    /** @brief シートテクスチャとコピー領域を初期化する */
    bool Initialize();
    /** @brief エクスポート中はフレームをシートに逐次コピーする */
    void Update();
    /** @brief リソースを解放する */
    void Finalize();
    /**
     * @brief スプライトシートのエクスポートを開始する
     * @param fileName 出力ファイル名（拡張子なし）
     * @param numW     横方向のフレーム数
     * @param numH     縦方向のフレーム数
     */
    void Export(std::wstring fileName, UINT numW, UINT numH);

private:
    RenderExporter() = delete;
    bool initSheet();
    void initBox();
    void updateSheet();
    bool save(ID3D11Texture2D* const texture, const std::wstring& sheetName);

    const RenderContext* const                   m_pContext     = nullptr; ///< レンダリングコンテキスト
    Microsoft::WRL::ComPtr<ID3D11Texture2D>      m_pSheetTexture = nullptr; ///< シートテクスチャ
    bool                                          m_isExport     = false;   ///< エクスポート中フラグ
    int                                           m_sheetIndex   = 0;       ///< 現在のシートインデックス
    float                                         m_sheetTimer   = 0.0f;    ///< フレーム間隔タイマー
    UINT                                          m_sheetNumW    = 1;       ///< 横分割数
    UINT                                          m_sheetNumH    = 1;       ///< 縦分割数
    D3D11_BOX                                     m_box;                    ///< コピー領域
    std::wstring                                  m_sheetName    = L"sheet"; ///< 出力ファイル名

    static const UINT  WIDTH         = 500; ///< キャプチャ幅
    static const UINT  HEIGHT        = 500; ///< キャプチャ高さ
    static const UINT  CROP_X        = 230; ///< クロップ開始 X
    static const UINT  CROP_Y        = 20;  ///< クロップ開始 Y
    static const float SHEET_INTERVAL;     ///< フレーム間隔（秒）
};
