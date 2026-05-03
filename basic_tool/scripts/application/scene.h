/**
 * @file scene.h
 * @brief シーン基底クラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include <memory>
#include <vector>
#include <string>
#include "game_object.h"

class RenderContext;
class Rigidbody;

/**
 * @brief すべてのシーンの基底クラス
 *
 * ゲームオブジェクトの生成・検索・シリアライズ機能を提供する。
 * 派生クラスは Initialize/Setup/Start/Update/Draw/Terminate/Finalize を実装する。
 */
class Scene
{
public:
    virtual ~Scene() = default;
    /** @brief リソースを初期化する */
    virtual void Initialize() = 0;
    /** @brief シーンのゲームオブジェクトを構築する */
    virtual void Setup() = 0;
    /** @brief ゲームプレイ開始時に一度だけ呼ばれる */
    virtual void Start() = 0;
    /** @brief 毎フレーム更新処理 */
    virtual void Update() = 0;
    /** @brief 毎フレーム描画処理 */
    virtual void Draw() = 0;
    /** @brief ゲームプレイ終了時に呼ばれる */
    virtual void Terminate() = 0;
    /** @brief リソースを解放する */
    virtual void Finalize() = 0;

    /**
     * @brief ゲームオブジェクトを生成してシーンに登録する（UUID 自動発行）
     * @param name        オブジェクト名
     * @param isSerialize シリアライズ対象かどうか
     * @return 生成したゲームオブジェクトへの参照
     */
    GameObject& Instantiate(std::string name, bool isSerialize = false);
    /**
     * @brief ゲームオブジェクトを生成してシーンに登録する（ID 指定）
     * @param id          オブジェクト ID
     * @param name        オブジェクト名
     * @param isSerialize シリアライズ対象かどうか
     * @return 生成したゲームオブジェクトへの参照
     */
    GameObject& Instantiate(uint64_t id, std::string name, bool isSerialize = false);
    /**
     * @brief ID でゲームオブジェクトを検索して返す
     * @param id オブジェクト ID
     * @return 見つかったオブジェクトへのポインタ、なければ nullptr
     */
    GameObject* const FindGameObject(uint64_t id) const;
    /**
     * @brief 名前でゲームオブジェクトを検索して返す
     * @param name オブジェクト名
     * @return 見つかったオブジェクトへのポインタ、なければ nullptr
     */
    GameObject* const FindGameObject(std::string name) const;
    /** @brief レンダリングコンテキストを返す */
    RenderContext* const GetContext() const { return m_pContext; }
    /** @brief ウィンドウハンドルを返す */
    const HWND& GetWnd() const { return m_hWnd; }
    /**
     * @brief シーンをファイルにシリアライズする
     * @param sceneName シーン名（ファイル名）
     */
    void Serialize(std::string sceneName);
    /**
     * @brief ファイルからシーンをデシリアライズする
     * @param path      ファイルパス
     * @param sceneName シーン名（ファイル名）
     */
    void Deserialize(std::string path, std::string sceneName);

    /**
     * @brief ID でコンポーネントを横断検索して返す
     * @tparam T   Component の派生クラス
     * @param  id  コンポーネント ID
     * @return 見つかったコンポーネントへのポインタ、なければ nullptr
     */
    template<typename T>
        requires std::derived_from<T, Component>
    T* const FindComponent(uint64_t id) const
    {
        for (auto& pGameObject : m_pGameObjects)
        {
            for (auto& pComponent : *pGameObject->GetComponents())
            {
                if (pComponent->GetID() == id)
                {
                    return static_cast<T*>(pComponent.get());
                }
            }
        }
        return nullptr;
    }

protected:
    /**
     * @brief コンストラクタ
     * @param hWnd     ウィンドウハンドル
     * @param pContext レンダリングコンテキスト
     */
    Scene(HWND hWnd, RenderContext* const pContext);
    /** @brief 破棄フラグが立ったゲームオブジェクトを削除する */
    void destroy();
    /** @brief レンダリングコンテキストを返す（const 版） */
    const RenderContext* const getContext() const { return m_pContext; }
    /** @brief ゲームオブジェクトリストを返す */
    const std::vector<std::unique_ptr<GameObject>>& getGameObjects() const { return m_pGameObjects; }
    /** @brief 全ゲームオブジェクトを削除する */
    void clearGameObjects() { m_pGameObjects.clear(); }
    /** @brief ランタイム（再生）状態かどうかを返す */
    bool getIsRuntime() const { return s_isRuntime; }
    /**
     * @brief ランタイム状態を設定する
     * @param which true で再生中
     */
    void setIsRuntime(bool which) { s_isRuntime = which; }

private:
    Scene() = delete;
    void deserialize(std::string path, std::string sceneName);
    HWND                                       m_hWnd         = nullptr; ///< ウィンドウハンドル
    RenderContext* const                       m_pContext      = nullptr; ///< レンダリングコンテキスト
    std::vector<std::unique_ptr<GameObject>>   m_pGameObjects;           ///< ゲームオブジェクトリスト
    static bool                                s_isRuntime;              ///< 全シーン共通の再生状態フラグ
};
