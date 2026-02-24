#pragma once
#include "transform.h"
#include "component.h"
#include <vector>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
using Json = nlohmann::ordered_json;

class GameObject 
{
public:
	GameObject(std::string name, bool isSerialize);
	~GameObject();
	void Initialize();
	void Start();
	void Update();
	void Finalize();
	void Destroy();
	Json Serialize() const;
	Transform& GetTransform() { return m_transform; }
	const std::vector<std::unique_ptr<Component>>* GetComponents() const { return &m_pComponents; }
	uint64_t GetID() const { return m_id; }
	const std::string GetName() const { return m_name; }
	bool GetIsDestroy() const { return m_isDestroy; }
	bool GetIsSerialize() const { return m_isSerialize; }

	// 生成したコンポーネントを返すことでGetComponentを削減
	template<typename T, typename... Args> // ...で複数の引数を受け取る
		requires std::derived_from<T, Component> // Componentの派生のみ
	T& AddComponent(Args&&... args) // &&で右辺値も受け取れる
	{
		auto pComponent = std::make_unique<T>(std::forward<Args>(args)...); // forwardで左辺値と右辺値を判別する
		pComponent->Initialize();

		T& ref = *pComponent; // moveする前に参照を保存
		m_pComponents.push_back(std::move(pComponent));
		return ref;
	}

private:
	GameObject() = delete;
	Transform m_transform;
	std::vector<std::unique_ptr<Component>> m_pComponents;
	const uint64_t m_id = -1;
	std::string m_name = "NONE";
	bool m_isDestroy = false;
	bool m_isSerialize = true;
};