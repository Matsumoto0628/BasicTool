/**
 * @file particle_controller.h
 * @brief パーティクルシステム制御コンポーネント
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include "component.h"
#include <vector>
#include <string>

class Camera;
class Transform;
class RenderContext;
class Rigidbody;
class GameObject;

/**
 * @brief 複数のパーティクルをプールして管理するパーティクルシステムコンポーネント
 *
 * 連続放出とバースト放出の 2 モードに対応し、テクスチャ・色・重力等を
 * インスペクターから調整できる。
 */
class ParticleController : public Component
{
public:
    /**
     * @brief デシリアライズ用コンストラクタ
     * @param id               コンポーネント ID
     * @param pTransform       エミッターのトランスフォーム
     * @param pContext         レンダリングコンテキスト
     * @param pCamera          使用するカメラ
     * @param pCameraTransform カメラのトランスフォーム
     */
    ParticleController(
        uint64_t id,
        Transform* const pTransform,
        const RenderContext* const pContext,
        const Camera* const pCamera,
        Transform* const pCameraTransform
    );
    /**
     * @brief 通常生成用コンストラクタ
     * @param pTransform       エミッターのトランスフォーム
     * @param pContext         レンダリングコンテキスト
     * @param pCamera          使用するカメラ
     * @param pCameraTransform カメラのトランスフォーム
     */
    ParticleController(
        Transform* const pTransform,
        const RenderContext* const pContext,
        const Camera* const pCamera,
        Transform* const pCameraTransform
    );
    ~ParticleController();
    void Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    void Show() override;
    /** @brief JSON にシリアライズして返す */
    Json Serialize() const override;
    /**
     * @brief JSON から ParticleController を生成して返す
     * @param j          JSON オブジェクト
     * @param pTransform エミッターのトランスフォーム
     * @param pContext   レンダリングコンテキスト
     */
    static std::unique_ptr<ParticleController> Deserialize(const Json& j, Transform* const pTransform, const RenderContext* const pContext);
    /** @brief パーティクルの放出を開始する */
    void Play();
    /** @brief パーティクルの放出を一時停止する */
    void Pause();
    /** @brief パーティクルの放出を再開する */
    void Resume();

private:
    std::wstring openTextureDialog();
    void applyTexture(std::wstring path);
    void play(int idx);
    void playBurst();
    bool showColor(const char* label, int& r, int& g, int& b, float width = 60.0f);

    Transform* const           m_pTransform       = nullptr; ///< エミッターのトランスフォーム
    const RenderContext* const m_pContext          = nullptr; ///< レンダリングコンテキスト
    const Camera* const        m_pCamera           = nullptr; ///< カメラ
    Transform* const           m_pCameraTransform  = nullptr; ///< カメラトランスフォーム
    std::vector<GameObject*>   m_pParticles;                  ///< パーティクルオブジェクトプール
    float m_duration   = 6;       ///< 放出継続時間（秒）
    float m_timer      = 0;       ///< 経過時間
    float m_isPause    = false;   ///< 一時停止フラグ
    int   m_idx        = 0;       ///< 現在のプールインデックス
    float m_span       = 0.01f;   ///< 連続放出の間隔（秒）
    float m_playTimer  = 0;       ///< 放出タイマー
    bool  m_isBurst    = false;   ///< バースト放出フラグ
    float m_size       = 1.0f;    ///< パーティクルサイズ
    int   m_burstCount = 30;      ///< バースト放出数
    Vec4  m_color      = {1,1,1,1}; ///< パーティクルカラー
    float m_intensity  = 10.0f;  ///< 発光強度
    float m_lifeTime   = 2.0f;   ///< パーティクル寿命（秒）
    float m_gravity    = 3.0f;   ///< 重力加速度
    float m_spread     = 1.0f;   ///< 拡散範囲
    std::wstring m_texturePath = L"SlimeBall.png"; ///< テクスチャパス

    static const int   PARTICLE_COUNT   = 100; ///< プールサイズ
    static const float SIZE_MULTIPLIER;        ///< サイズ倍率定数
};
