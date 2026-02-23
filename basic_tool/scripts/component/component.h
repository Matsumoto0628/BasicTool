#pragma once

// 仮想関数はテーブルによってパフォーマンスが良くなくなる可能性があるが、
// std::vectorで一括で管理するため純粋仮想関数を使用する
class Component
{
public:
	virtual ~Component() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Finalize() = 0;
	virtual void Show() = 0;
	void SetEnabled(bool flag) { enabled = flag; }
	bool GetEnabled() { return enabled; }

protected:
	Component() = default;
	bool enabled = true;
};