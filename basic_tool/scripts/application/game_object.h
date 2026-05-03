/**
 * @file game_object.h
 * @brief ゲームオブジェクトクラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include "transform.h"
#include "component.h"
#include <vector>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
using Json = nlohmann::ordered_json;

/**
 * @brief シーン上のエンティティを表すゲームオブジェクトクラス
 *
 * Transform を持ち、任意の Component を複数アタッチできる。
 * シリアライズ対応で、JSON による保存・読み込みが可能。
 */
class GameObject
{
public:
    /**
     * @brief 新規生成用コンストラクタ（UUID を自動発行）
     * @param name        オブジェクト名
     * @param isSerialize シリアライズ対象かどうか
     */
    GameObject(std::string name, bool isSerialize);
    /**
     * @brief デシリアライズ用コンストラクタ（既存 ID を指定）
     * @param id          オブジェクト ID
     * @param name        オブジェクト名
     * @param isSerialize シリアライズ対象かどうか
     */
    GameObject(uint64_t id, std::string name, bool isSerialize);
    ~GameObject();
    /** @brief コンポーネントの初期化を行う */
    void Initialize();
    /** @brief 全コンポーネントの Start() を呼び出す */
    void Start();
    /** @brief 全コンポーネントの Update() を呼び出す */
    void Update();
    /** @brief 全コンポーネントの Draw() を呼び出す */
    void Draw();
    /** @brief 全コンポーネントの Finalize() を呼び出す */
    void Finalize();
    /** @brief 次フレームでの破棄フラグを立てる */
    void Destroy();
    /** @brief JSON にシリアライズして返す */
    Json Serialize() const;
    /**
     * @brief JSON からデシリアライズしてコンポーネントを復元する
     * @param j JSON オブジェクト
     */
    void Deserialize(const Json& j);
    /** @brief トランスフォームへの参照を返す */
    Transform& GetTransform() { return m_transform; }
    /** @brief コンポーネントリストへのポインタを返す */
    const std::vector<std::unique_ptr<Component>>* const GetComponents() const { return &m_pComponents; }
    /** @brief オブジェクトの一意な ID を返す */
    uint64_t GetID() const { return m_id; }
    /** @brief オブジェクト名を返す */
    const std::string GetName() const { return m_name; }
    /** @brief 破棄フラグが立っているかどうかを返す */
    bool GetIsDestroy() const { return m_isDestroy; }
    /** @brief シリアライズ対象かどうかを返す */
    bool GetIsSerialize() const { return m_isSerialize; }

    /**
     * @brief コンポーネントを生成してアタッチし、参照を返す
     *
     * 生成したコンポーネントを返すことで GetComponent の呼び出しを省略できる。
     *
     * @tparam T     Component の派生クラス
     * @tparam Args  コンストラクタ引数の型
     * @param args   コンストラクタ引数
     * @return アタッチしたコンポーネントへの参照
     */
    template<typename T, typename... Args>
        requires std::derived_from<T, Component>
    T& AddComponent(Args&&... args)
    {
        auto pComponent = std::make_unique<T>(std::forward<Args>(args)...);
        pComponent->Initialize();
        if (m_isStart) pComponent->Start();

        T& ref = *pComponent;
        m_pComponents.push_back(std::move(pComponent));
        return ref;
    }

    /**
     * @brief 指定した種別のコンポーネントを検索して返す
     *
     * 見つからない場合は nullptr を返す。
     *
     * @tparam T    Component の派生クラス
     * @param type  検索する種別
     * @return 見つかったコンポーネントへのポインタ、なければ nullptr
     */
    template<typename T>
        requires std::derived_from<T, Component>
    T* const FindComponent(Component::Type type) const
    {
        for (auto& pComponent : m_pComponents)
        {
            if (pComponent->GetType() == type)
            {
                return static_cast<T*>(pComponent.get());
            }
        }
        return nullptr;
    }

private:
    GameObject() = delete;
    void deserializeComponent(const Json& j);
    Transform                                    m_transform;     ///< トランスフォーム
    std::vector<std::unique_ptr<Component>>      m_pComponents;   ///< コンポーネントリスト
    const uint64_t                               m_id = -1;       ///< 一意な識別子
    std::string                                  m_name = "NONE"; ///< オブジェクト名
    bool                                         m_isDestroy = false;    ///< 破棄フラグ
    const bool                                   m_isSerialize = true;   ///< シリアライズ対象フラグ
    bool                                         m_isStart = false;      ///< Start 済みフラグ
};
