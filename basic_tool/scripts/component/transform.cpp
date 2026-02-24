#include "transform.h"
#include "imgui.h"

Transform::Transform(GameObject* const pGameObject)
    : m_position{ 0, 0, 0 },
    m_rotation{ 0, 0, 0, 0 },
    m_scale{ 1, 1, 1 },
    m_localPosition{ 0, 0, 0 },
    m_localRotation{ 0, 0, 0, 0 },
    m_localScale{ 1, 1, 1 },
    m_matrix{ Mat4x4::Identity() },
    m_world{ Mat4x4::Identity() },
    m_local{ Mat4x4::Identity() },
    m_pGameObject{ pGameObject }
{
    applyWorld();
    applyMatrix();
}

Transform::~Transform()
{
}

void Transform::SetPosition(const Vec3& position)
{
    m_position = position;
    applyWorld();
    applyMatrix();
}

void Transform::SetRotation(const Vec4& rotation)
{
    m_rotation = rotation;
    const Vec3 eulerRad = QuaternionToEuler(rotation);
    m_eulerAngles = { RadToDeg(eulerRad.X()), RadToDeg(eulerRad.Y()), RadToDeg(eulerRad.Z()) };
    applyWorld();
    applyMatrix();
}

void Transform::SetScale(const Vec3& scale)
{
    m_scale = scale;
    applyWorld();
    applyMatrix();
}

void Transform::SetLocalPosition(const Vec3& position)
{
    m_localPosition = position;
    applyLocal();
    applyMatrix();
}

void Transform::SetLocalRotation(const Vec4& rotation)
{
    m_localRotation = rotation;
    const Vec3 eulerRad = QuaternionToEuler(rotation);
    m_eulerAngles = { RadToDeg(eulerRad.X()), RadToDeg(eulerRad.Y()), RadToDeg(eulerRad.Z()) };
    applyLocal();
    applyMatrix();
}

void Transform::SetLocalScale(const Vec3& scale)
{
    m_localScale = scale;
    applyLocal();
    applyMatrix();
}

void Transform::SetParent(Transform* pParent)
{
    if (m_pParent)
    {
        auto& siblings = m_pParent->m_pChildren; // siblingsは兄弟
        siblings.erase(
            std::remove_if(siblings.begin(), siblings.end(),
                [this](const Transform* p) {
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

    applyWorld();
    applyMatrix();
}

void Transform::Show()
{
    float posX = m_position.X();
    float posY = m_position.Y();
    float posZ = m_position.Z();
    float rotX = m_eulerAngles.X();
    float rotY = m_eulerAngles.Y();
    float rotZ = m_eulerAngles.Z();
    float scaX = m_scale.X();
    float scaY = m_scale.Y();
    float scaZ = m_scale.Z();

    ImGui::PushID(this);
    if (ImGui::CollapsingHeader("Transform"))
    {
        ImGui::PushID("Position");
        {
            ImGui::Text("Position");
            ImGui::SameLine();
            ImGui::PushItemWidth(60);
            ImGui::DragFloat("##x", &posX);
            ImGui::SameLine();
            ImGui::DragFloat("##y", &posY);
            ImGui::SameLine();
            ImGui::DragFloat("##z", &posZ);
            ImGui::PopItemWidth();
        }
        ImGui::PopID();

        ImGui::PushID("Rotation");
        {
            ImGui::Text("Rotation");
            ImGui::SameLine();
            ImGui::PushItemWidth(60);
            ImGui::DragFloat("##x", &rotX);
            ImGui::SameLine();
            ImGui::DragFloat("##y", &rotY);
            ImGui::SameLine();
            ImGui::DragFloat("##z", &rotZ);
            ImGui::PopItemWidth();
        }
        ImGui::PopID();

        ImGui::PushID("Scale");
        {
            ImGui::Text("Scale");
            ImGui::SameLine();
            ImGui::PushItemWidth(60);
            ImGui::DragFloat("##x", &scaX);
            ImGui::SameLine();
            ImGui::DragFloat("##y", &scaY);
            ImGui::SameLine();
            ImGui::DragFloat("##z", &scaZ);
            ImGui::PopItemWidth();
        }
        ImGui::PopID();
    }
    ImGui::PopID();

    rotX = static_cast<int>(rotX) % 360;
    rotY = static_cast<int>(rotY) % 360;
    rotZ = static_cast<int>(rotZ) % 360;

    if (posX != m_position.X() || posY != m_position.Y() || posZ != m_position.Z())
    {
        SetPosition({ posX, posY, posZ });
    }
    if (rotX != m_eulerAngles.X() || rotY != m_eulerAngles.Y() || rotZ != m_eulerAngles.Z())
    {
        applyEulerAngles({ rotX, rotY, rotZ });
    }
    if (scaX != m_scale.X() || scaY != m_scale.Y() || scaZ != m_scale.Z())
    {
        SetScale({ scaX, scaY, scaZ });
    }
}

void Transform::applyWorld() 
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

void Transform::applyLocal() 
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

void Transform::applyMatrix()
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
            child->applyMatrix();
        }
    }
}

void Transform::applyEulerAngles(const Vec3& eulerDeg)
{
    m_rotation = EulerToQuaternion({ DegToRad(eulerDeg.X()), DegToRad(eulerDeg.Y()), DegToRad(eulerDeg.Z()) });
    m_eulerAngles = eulerDeg;
    applyWorld();
    applyMatrix();
}