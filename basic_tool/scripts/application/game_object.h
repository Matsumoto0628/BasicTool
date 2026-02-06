#pragma once
#include "transform.h"
#include "component.h"
#include <vector>
#include <memory>

class GameObject 
{
public:
	GameObject();
	~GameObject();
	void Initialize();
	void Start();
	void Update();
	void Terminate();
	void Finalize();
	Transform& GetTransform() { return m_transform; }

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
	Transform m_transform;
	std::vector<std::unique_ptr<Component>> m_pComponents;
};