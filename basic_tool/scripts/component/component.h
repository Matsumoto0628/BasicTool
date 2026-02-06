#pragma once

class Component
{
public:
	virtual ~Component() = default;
	virtual void Initialize() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Terminate() = 0;
	virtual void Finalize() = 0;
protected:
	Component() = default;
};