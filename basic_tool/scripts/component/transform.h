/**
 * @file transform.h
 * @brief ゲームオブジェクトの位置・回転・スケール管理クラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include <vector>
#include "component.h"
#include <nlohmann/json.hpp>
using Json = nlohmann::ordered_json;

class GameObject;

/**
 * @brief ゲームオブジェクトの空間情報（位置・回転・スケール）を管理するクラス
 *
 * 親子関係に対応しており、ローカル座標とワールド座標の両方を保持する。
 */
class Transform
{
public:
    /**
     * @brief コンストラクタ
     * @param pGameObject このトランスフォームが属するゲームオブジェクト
     */
    Transform(GameObject* const pGameObject);
    ~Transform();

    /** @brief ワールド位置を返す */
    const Vec3& GetPosition() const { return m_position; }
    /** @brief ワールド回転（クォータニオン）を返す */
    const Vec4& GetRotation() const { return m_rotation; }
    /** @brief ワールドスケールを返す */
    const Vec3& GetScale() const { return m_scale; }
    /** @brief ローカル位置を返す */
    const Vec3& GetLocalPosition() const { return m_localPosition; }
    /** @brief ローカル回転（クォータニオン）を返す */
    const Vec4& GetLocalRotation() const { return m_localRotation; }
    /** @brief ローカルスケールを返す */
    const Vec3& GetLocalScale() const { return m_localScale; }
    /** @brief ワールド変換行列を返す */
    const Mat4x4& GetMatrix() const { return m_matrix; }
    /** @brief ワールド空間での前方向ベクトルを返す */
    const Vec3& GetForward() const { return m_rotation.RotateVec3({ 0,0,1 }); }
    /** @brief ワールド空間での上方向ベクトルを返す */
    const Vec3& GetUp() const { return m_rotation.RotateVec3({ 0,1,0 }); }
    /** @brief ワールド空間での右方向ベクトルを返す */
    const Vec3& GetRight() const { return m_rotation.RotateVec3({ 1,0,0 }); }
    /** @brief 子トランスフォームのリストを返す */
    const std::vector<Transform*>& GetChildren() const { return m_pChildren; }
    /** @brief 親トランスフォームを返す（なければ nullptr） */
    const Transform* const GetParent() const { return m_pParent; }
    /** @brief このトランスフォームが属するゲームオブジェクトを返す */
    GameObject& GetGameObject() const { return *m_pGameObject; }

    /**
     * @brief ワールド位置を設定する
     * @param position ワールド位置
     */
    void SetPosition(const Vec3& position);
    /**
     * @brief ワールド回転を設定する
     * @param rotation クォータニオン
     */
    void SetRotation(const Vec4& rotation);
    /**
     * @brief ワールドスケールを設定する
     * @param scale スケール
     */
    void SetScale(const Vec3& scale);
    /**
     * @brief ローカル位置を設定する
     * @param position ローカル位置
     */
    void SetLocalPosition(const Vec3& position);
    /**
     * @brief ローカル回転を設定する
     * @param rotation クォータニオン
     */
    void SetLocalRotation(const Vec4& rotation);
    /**
     * @brief ローカルスケールを設定する
     * @param scale スケール
     */
    void SetLocalScale(const Vec3& scale);
    /**
     * @brief 親トランスフォームを設定して親子関係を構築する
     * @param pParent 親トランスフォーム（nullptr で親なし）
     */
    void SetParent(Transform* pParent);
    /** @brief ImGui を使ったインスペクター表示処理 */
    void Show();
    /** @brief JSON にシリアライズして返す */
    Json Serialize() const;
    /**
     * @brief JSON からデシリアライズして値を設定する
     * @param j JSON オブジェクト
     */
    void Deserialize(const Json& j);

private:
    Transform() = delete;
    void applyWorld();
    void applyLocal();
    void applyMatrix();
    void applyEulerAngles(const Vec3& eulerDeg);
    bool showVec3(const char* label, float& x, float& y, float& z, float clamp = 1000.0f, float width = 60.0f);

    Vec3   m_position;       ///< ワールド位置
    Vec4   m_rotation;       ///< ワールド回転（クォータニオン）
    Vec3   m_scale;          ///< ワールドスケール
    Vec3   m_localPosition;  ///< ローカル位置
    Vec4   m_localRotation;  ///< ローカル回転（クォータニオン）
    Vec3   m_localScale;     ///< ローカルスケール
    Mat4x4 m_matrix;         ///< ワールド変換行列
    Mat4x4 m_world;          ///< ワールド行列
    Mat4x4 m_local;          ///< ローカル行列
    Vec3   m_eulerAngles;    ///< インスペクター表示用オイラー角（度）

    std::vector<Transform*> m_pChildren;          ///< 子トランスフォームリスト
    Transform*              m_pParent = nullptr;  ///< 親トランスフォーム

    GameObject* const m_pGameObject = nullptr;    ///< 所有ゲームオブジェクト
};
