/**
 * @file component.h
 * @brief コンポーネント基底クラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include <memory>
#include <nlohmann/json.hpp>
using Json = nlohmann::ordered_json;

/**
 * @brief すべてのコンポーネントの基底クラス
 *
 * 仮想関数テーブルによるオーバーヘッドはあるが、std::vector で一括管理するために
 * 純粋仮想関数を用いたポリモーフィズムを採用している。
 */
class Component
{
public:
    virtual ~Component();
    /** @brief 初期化処理（リソース確保など） */
    virtual void Initialize() = 0;
    /** @brief 開始時処理（シーン開始後に一度だけ呼ばれる） */
    virtual void Start() = 0;
    /** @brief 毎フレーム更新処理 */
    virtual void Update() = 0;
    /** @brief 毎フレーム描画処理 */
    virtual void Draw() = 0;
    /** @brief 終了時リソース解放処理 */
    virtual void Finalize() = 0;
    /** @brief ImGui を使ったインスペクター表示処理 */
    virtual void Show() = 0;
    /** @brief JSON にシリアライズして返す */
    virtual Json Serialize() const = 0;
    /**
     * @brief コンポーネントの有効・無効を設定する
     * @param flag true で有効、false で無効
     */
    void SetEnabled(bool flag) { enabled = flag; }
    /** @brief コンポーネントが有効かどうかを返す */
    bool GetEnabled() const { return enabled; }
    /** @brief コンポーネントの一意な ID を返す */
    uint64_t GetID() const { return m_id; }

    /** @brief コンポーネントの種別を表す列挙型 */
    enum class Type
    {
        None,
        Camera,
        Rigidbody,
        Triangle,
        Square,
        Line,
        Sphere,
        CameraController,
        ParticleController,
        Particle,
        Sprite
    };
    /** @brief コンポーネントの種別を返す */
    Type GetType() const { return m_type; }

protected:
    /** @brief 種別を指定するコンストラクタ（新規作成時） */
    Component(Type type);
    /** @brief ID と種別を指定するコンストラクタ（デシリアライズ時） */
    Component(uint64_t id, Type type);

private:
    bool           enabled = true;          ///< 有効フラグ
    const uint64_t m_id    = -1;            ///< 一意な識別子
    const Type     m_type  = Type::None;    ///< コンポーネント種別
};

NLOHMANN_JSON_SERIALIZE_ENUM(Component::Type, {
    {Component::Type::None, "None"},
    {Component::Type::Camera, "Camera"},
    {Component::Type::Rigidbody, "Rigidbody"},
    {Component::Type::Triangle, "Triangle"},
    {Component::Type::Square, "Square"},
    {Component::Type::Line, "Line"},
    {Component::Type::Sphere, "Sphere"},
    {Component::Type::CameraController, "CameraController"},
    {Component::Type::ParticleController, "ParticleController"},
    {Component::Type::Particle, "Particle"},
    {Component::Type::Sprite, "Sprite"},
})
