/**
 * @file camera_controller.h
 * @brief カメラ操作コンポーネント
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include "component.h"

class Transform;

/**
 * @brief マウス入力でカメラのトランスフォームを操作するコンポーネント
 *
 * 右クリックドラッグで回転、WASD キーで移動を行う。
 */
class CameraController : public Component
{
public:
    /**
     * @brief デシリアライズ用コンストラクタ
     * @param id         コンポーネント ID
     * @param pTransform 操作対象のトランスフォーム
     */
    CameraController(uint64_t id, Transform* pTransform);
    /**
     * @brief 通常生成用コンストラクタ
     * @param pTransform 操作対象のトランスフォーム
     */
    CameraController(Transform* pTransform);
    ~CameraController();
    void Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    void Show() override;
    /** @brief JSON にシリアライズして返す */
    Json Serialize() const override;
    /**
     * @brief JSON から CameraController を生成して返す
     * @param j          JSON オブジェクト
     * @param pTransform 操作対象のトランスフォーム
     */
    static std::unique_ptr<CameraController> Deserialize(const Json& j, Transform* pTransform);

private:
    CameraController() = delete;
    Transform* const m_pTransform = nullptr; ///< 操作対象のトランスフォーム
    POINT m_anchorPos;                       ///< ドラッグ開始時のマウス座標
    float m_rotX;                            ///< X 軸回転累積量
    float m_rotY;                            ///< Y 軸回転累積量

    const static float m_speed;       ///< 移動速度定数
    const static float m_sensitivity; ///< 回転感度定数
};
