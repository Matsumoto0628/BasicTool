/**
 * @file camera.h
 * @brief カメラコンポーネント
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include "component.h"
#include "transform.h"

class RenderContext;

/**
 * @brief ビュー行列・プロジェクション行列を管理するカメラコンポーネント
 */
class Camera : public Component
{
public:
    /**
     * @brief デシリアライズ用コンストラクタ
     * @param id         コンポーネント ID
     * @param pTransform トランスフォームへのポインタ
     * @param aspect     アスペクト比
     */
    Camera(uint64_t id, const Transform* const pTransform, float aspect);
    /**
     * @brief 通常生成用コンストラクタ
     * @param pTransform トランスフォームへのポインタ
     * @param width      描画幅（ピクセル）
     * @param height     描画高さ（ピクセル）
     */
    Camera(const Transform* const pTransform, unsigned int width, unsigned int height);
    ~Camera();
    void Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    void Show() override;
    /** @brief JSON にシリアライズして返す */
    Json Serialize() const override;
    /**
     * @brief JSON からカメラを生成して返す
     * @param j          JSON オブジェクト
     * @param pTransform トランスフォームへのポインタ
     */
    static std::unique_ptr<Camera> Deserialize(const Json& j, const Transform* const pTransform);
    /** @brief ビュー行列（トランスフォームの逆行列）を返す */
    const Mat4x4& GetView() const { return m_pTransform->GetMatrix().Inverse(); };
    /** @brief プロジェクション行列を返す */
    const Mat4x4& GetProj() const { return m_proj; };

private:
    Camera() = delete;
    Mat4x4 m_view;                             ///< ビュー行列
    Mat4x4 m_proj;                             ///< プロジェクション行列
    float  m_aspect;                           ///< アスペクト比
    float  m_nearZ = 0.0f;                     ///< ニアクリップ距離
    float  m_farZ  = 0.0f;                     ///< ファークリップ距離
    float  m_fov   = 0.0f;                     ///< 垂直視野角（ラジアン）
    const Transform* const m_pTransform = nullptr; ///< 参照するトランスフォーム
};
