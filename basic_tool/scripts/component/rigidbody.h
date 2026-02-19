#pragma once
#include <memory>
#include "component.h"

class Transform;

class Rigidbody : public Component
{
public:
    Rigidbody(Transform* pTransform);
    ~Rigidbody();
    void Initialize() override;
    void Start() override;
    void Update() override;
    void Terminate() override;
    void Finalize() override;
    void Show() override;
    void AddForce(const Vec3& force);
    void SetVelocity(const Vec3& vel);
    const Vec3& GetVelocity() const { return m_velocity; }
    float GetMass() const { return m_mass; }

private:
    Rigidbody() = delete;
    Vec3 m_velocity;
    Vec3 m_acceleration;
    float m_mass;
    Vec3 m_force;
    Transform* m_pTransform;
};