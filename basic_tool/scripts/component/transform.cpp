#include "transform.h"

Transform::Transform()
    : m_position{ 0, 0, 0 },
    m_rotation{0, 0, 0, 0},
    m_scale{1, 1, 1},
    m_localPosition{ 0, 0, 0 },
    m_localRotation{0, 0, 0, 0},
    m_localScale{1, 1, 1},
    m_matrix{Mat4x4::Identity()},
    m_world{Mat4x4::Identity()},
    m_local{Mat4x4::Identity()}
{
    setWorld();
    setMatrix();
}

Transform::~Transform()
{
}

void Transform::SetPosition(const Vec3& position)
{
    m_position = position;
    setWorld();
    setMatrix();
}

void Transform::SetRotation(const Vec4& rotation)
{
    m_rotation = rotation;
    setWorld();
    setMatrix();
}

void Transform::SetScale(const Vec3& scale)
{
    m_scale = scale;
    setWorld();
    setMatrix();
}

void Transform::SetLocalPosition(const Vec3& position)
{
    m_localPosition = position;
    setLocal();
    setMatrix();
}

void Transform::SetLocalRotation(const Vec4& rotation)
{
    m_localRotation = rotation;
    setLocal();
    setMatrix();
}

void Transform::SetLocalScale(const Vec3& scale)
{
    m_localScale = scale;
    setLocal();
    setMatrix();
}

void Transform::SetParent(Transform* pParent)
{
    if (m_pParent)
    {
        auto& siblings = m_pParent->m_pChildren; // siblingsは兄弟
        siblings.erase(
            std::remove_if(siblings.begin(), siblings.end(),
                [this](const Transform* p) { // ラムダ式
                    return p == this;
                }),
            siblings.end()
        );
    }

    m_pParent = pParent;
    
    if (m_pParent)
    {
        m_pParent->m_pChildren.push_back(this);
    }

    setWorld();
    setMatrix();
}

void Transform::Show()
{
}

void Transform::setWorld() 
{
    m_world = Mat4x4::Scale(m_scale.X(), m_scale.Y(), m_scale.Z())
        * Mat4x4::Rotation(m_rotation.X(), m_rotation.Y(), m_rotation.Z(), m_rotation.W())
        * Mat4x4::Translation(m_position.X(), m_position.Y(), m_position.Z());

    if (m_pParent) 
    {
        m_local = m_world * m_pParent->m_matrix.Inverse();
        m_local.ToPosRotScale(m_localPosition, m_localRotation, m_localScale);
    }
}

void Transform::setLocal() 
{
    m_local = Mat4x4::Scale(m_localScale.X(), m_localScale.Y(), m_localScale.Z())
        * Mat4x4::Rotation(m_localRotation.X(), m_localRotation.Y(), m_localRotation.Z(), m_localRotation.W())
        * Mat4x4::Translation(m_localPosition.X(), m_localPosition.Y(), m_localPosition.Z());

    if (m_pParent)
    {
        m_world = m_local * m_pParent->m_matrix;
        m_world.ToPosRotScale(m_position, m_rotation, m_scale);
    }
}

void Transform::setMatrix()
{
    if (m_pParent)
    {
        m_matrix = m_local * m_pParent->m_matrix;
        m_matrix.ToPosRotScale(m_position, m_rotation, m_scale);
    }
    else 
    {
        m_matrix = m_world;
    }

    for (auto& child : m_pChildren)
    {
        if (child) 
        {
            child->setMatrix();
        }
    }
}