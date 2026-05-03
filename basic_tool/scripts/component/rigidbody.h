/**
 * @file rigidbody.h
 * @brief 剛体物理コンポーネント
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include <memory>
#include "component.h"

class Transform;

/**
 * @brief 質量・速度・力を扱う剛体物理コンポーネント
 *
 * 毎フレームの Update() で加速度を積分して位置を更新する。
 */
class Rigidbody : public Component
{
public:
    /**
     * @brief デシリアライズ用コンストラクタ
     * @param id         コンポーネント ID
     * @param pTransform 操作対象のトランスフォーム
     */
    Rigidbody(uint64_t id, Transform* const pTransform);
    /**
     * @brief 通常生成用コンストラクタ
     * @param pTransform 操作対象のトランスフォーム
     */
    Rigidbody(Transform* const pTransform);
    ~Rigidbody();
    void Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    void Show() override;
    /** @brief JSON にシリアライズして返す */
    Json Serialize() const override;
    /**
     * @brief JSON から Rigidbody を生成して返す
     * @param j          JSON オブジェクト
     * @param pTransform 操作対象のトランスフォーム
     */
    static std::unique_ptr<Rigidbody> Deserialize(const Json& j, Transform* const pTransform);
    /**
     * @brief 継続的な力を加える（質量考慮、毎フレーム累積）
     * @param force 力ベクトル
     */
    void AddForce(const Vec3& force);
    /**
     * @brief 瞬間的な力（インパルス）を加える（即座に速度へ変換）
     * @param force インパルスベクトル
     */
    void AddImpulse(const Vec3& force);
    /**
     * @brief 速度を直接設定する
     * @param vel 速度ベクトル
     */
    void SetVelocity(const Vec3& vel);
    /** @brief 現在の速度を返す */
    const Vec3& GetVelocity() const { return m_velocity; }
    /** @brief 質量を返す */
    float GetMass() const { return m_mass; }

private:
    Rigidbody() = delete;
    Vec3   m_velocity;                         ///< 速度
    Vec3   m_acceleration;                     ///< 加速度
    float  m_mass;                             ///< 質量
    Vec3   m_force;                            ///< 累積力
    Transform* const m_pTransform = nullptr;   ///< 操作対象のトランスフォーム
};
