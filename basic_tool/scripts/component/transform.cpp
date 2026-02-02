#include "transform.h"

Transform::Transform()
    : m_position(0, 0, 0),
    m_rotation(0, 0, 0, 0),
    m_scale(1, 1, 1),
    m_localPosition(0, 0, 0),
    m_localRotation(0, 0, 0, 0),
    m_localScale(1, 1, 1),
    m_matrix(Mat4x4::Identity())
{
    setMatrix();
}

Transform::~Transform()
{
    if (auto parent = m_pParent.lock())
    {
        auto& siblings = parent->m_pChildren;
        siblings.erase(
            std::remove_if(siblings.begin(), siblings.end(),
                [this](const std::shared_ptr<Transform>& p)
                {
                    return p.get() == this;
                }),
            siblings.end()
        );
    }

    for (auto& child : m_pChildren)
    {
        child->m_pParent.reset();
    }

    m_pChildren.clear();
}

void Transform::SetPosition(const Vec3& position)
{
    m_position = position;
    setMatrix();
}

void Transform::SetRotation(const Vec4& rotation)
{
    m_rotation = rotation;
    setMatrix();
}

void Transform::SetScale(const Vec3& scale)
{
    m_scale = scale;
    setMatrix();
}

void Transform::SetLocalPosition(const Vec3& position)
{
    if (m_pParent.expired()) 
    {
        m_position = position;
        m_localPosition = { 0, 0, 0 };
    }
    else 
    {
        m_localPosition = position;
    }
    setMatrix();
}

void Transform::SetLocalRotation(const Vec4& rotation)
{
    if (m_pParent.expired())
    {
        m_rotation = rotation;
        m_localRotation = { 0, 0, 0, 0 };
    }
    else
    {
        m_localRotation = rotation;
    }
    setMatrix();
}

void Transform::SetLocalScale(const Vec3& scale)
{
    if (m_pParent.expired())
    {
        m_scale = scale;
        m_localScale = { 1, 1, 1 };
    }
    else
    {
        m_localScale = scale;
    }
    setMatrix();
}

void Transform::SetParent(const std::shared_ptr<Transform>& parent)
{
    if (auto oldParent = m_pParent.lock())
    {
        auto& siblings = oldParent->m_pChildren; // siblingsは兄弟
        siblings.erase(
            std::remove_if(siblings.begin(), siblings.end(),
                [this](const std::shared_ptr<Transform>& p) { // ラムダ式
                    return p.get() == this;
                }),
            siblings.end()
        );
    }

    m_pParent = parent;
    
    if (auto p = m_pParent.lock())
    {
        p->m_pChildren.push_back(shared_from_this()); // 自分自身をshared_ptrにする
    }
    else 
    {
        m_localPosition = { 0, 0, 0 };
        m_localRotation = { 0, 0, 0, 0 };
        m_localScale = { 1, 1, 1 };
    }

    setMatrix();
}

void Transform::setMatrix()
{
    m_matrix = Mat4x4::Scale(m_scale.X(), m_scale.Y(), m_scale.Z())
        * Mat4x4::Rotation(m_rotation.X(), m_rotation.Y(), m_rotation.Z(), m_rotation.W())
        * Mat4x4::Translation(m_position.X(), m_position.Y(), m_position.Z());

    if (auto parent = m_pParent.lock())
    {
        Mat4x4 invParent = parent->m_matrix.Inverse();
        Mat4x4 localOffset = Mat4x4::Scale(m_localScale.X(), m_localScale.Y(), m_localScale.Z())
            * Mat4x4::Rotation(m_localRotation.X(), m_localRotation.Y(), m_localRotation.Z(), m_localRotation.W())
            * Mat4x4::Translation(m_localPosition.X(), m_localPosition.Y(), m_localPosition.Z());
        m_matrix = invParent * m_matrix * localOffset;
    }

    for (auto& child : m_pChildren)
    {
        child->setMatrix();
    }
}