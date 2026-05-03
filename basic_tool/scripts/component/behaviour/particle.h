/**
 * @file particle.h
 * @brief 単一パーティクルコンポーネント
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include "component.h"

class Transform;
class Rigidbody;
class Line;
class Sprite;

/**
 * @brief 単一パーティクルの寿命・描画・物理を管理するコンポーネント
 *
 * Line（軌跡描画）または Sprite（ビルボード描画）のいずれかを切り替えて使用できる。
 */
class Particle : public Component
{
public:
    /**
     * @brief デシリアライズ用コンストラクタ
     * @param id               コンポーネント ID
     * @param pTransform       このパーティクルのトランスフォーム
     * @param pRb              剛体コンポーネント
     * @param pLine            軌跡描画用ライン
     * @param pCameraTransform カメラのトランスフォーム（ビルボード用）
     * @param pSprite          スプライト描画コンポーネント
     * @param lifeTime         生存時間（秒）
     */
    Particle(uint64_t id, Transform* pTransform, Rigidbody* pRb, Line* pLine, Transform* pCameraTransform, Sprite* pSprite, float lifeTime);
    /**
     * @brief 通常生成用コンストラクタ
     * @param pTransform       このパーティクルのトランスフォーム
     * @param pRb              剛体コンポーネント
     * @param pLine            軌跡描画用ライン
     * @param pCameraTransform カメラのトランスフォーム（ビルボード用）
     * @param pSprite          スプライト描画コンポーネント
     * @param lifeTime         生存時間（秒）
     */
    Particle(Transform* pTransform, Rigidbody* pRb, Line* pLine, Transform* pCameraTransform, Sprite* pSprite, float lifeTime);
    ~Particle();
    void Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    void Show() override;
    /** @brief JSON にシリアライズして返す */
    Json Serialize() const override;
    /**
     * @brief JSON から Particle を生成して返す
     * @param j          JSON オブジェクト
     * @param pTransform 操作対象のトランスフォーム
     */
    static std::unique_ptr<Particle> Deserialize(const Json& j, Transform* pTransform);
    /**
     * @brief ライン描画モードを設定する
     * @param which true でライン、false でスプライト
     */
    void SetIsLine(bool which) { m_isLine = which; }
    /** @brief ライン描画モードかどうかを返す */
    bool GetIsLine() { return m_isLine; }
    /** @brief パーティクルの状態をリセットして再利用可能にする */
    void Reset();
    /** @brief 生存時間を更新し、寿命を超えたら非アクティブにする */
    void UpdateLife();
    /**
     * @brief パーティクルの色を設定する
     * @param col RGBA カラー
     */
    void SetColor(const Vec4& col);
    /**
     * @brief 生存時間を設定する
     * @param lifeTime 生存時間（秒）
     */
    void SetLifeTime(float lifeTime) { m_lifeTime = lifeTime; }

private:
    Particle() = delete;
    void drawLine();
    Transform* const  m_pTransform       = nullptr; ///< このパーティクルのトランスフォーム
    Rigidbody* const  m_pRb              = nullptr; ///< 剛体コンポーネント
    Line* const       m_pLine            = nullptr; ///< 軌跡描画用ライン
    const Transform* const m_pCameraTransform = nullptr; ///< カメラトランスフォーム
    bool              m_isLine           = false;   ///< ライン描画モードフラグ
    float             m_lifeTime         = 3.0f;    ///< 生存時間（秒）
    float             m_timer            = 0;       ///< 経過時間
    Sprite* const     m_pSprite          = nullptr; ///< スプライト描画コンポーネント
    Vec4              m_color;                      ///< パーティクルカラー
};
