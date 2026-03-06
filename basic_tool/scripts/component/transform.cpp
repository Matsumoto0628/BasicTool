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
    bool isChangePos = false;
    bool isChangeRot = false;
    bool isChangeSca = false;

    ImGui::PushID(this);
    if (ImGui::CollapsingHeader(("Transform"), ImGuiTreeNodeFlags_DefaultOpen))
    {
        isChangePos = showVec3("Position", posX, posY, posZ);
        isChangeRot = showVec3("Rotation", rotX, rotY, rotZ, 360.0f);
        isChangeSca = showVec3("Scale", scaX, scaY, scaZ);
    }
    ImGui::PopID();

    if (isChangePos)
    {
        SetPosition({ posX, posY, posZ });
    }
    if (isChangeRot)
    {
        applyEulerAngles({ rotX, rotY, rotZ });
    }
    if (isChangeSca)
    {
        SetScale({ scaX, scaY, scaZ });
    }
}

Json Transform::Serialize() const
{
    return {
        {"position", m_position},
        {"rotation", m_rotation},
		{"scale", m_scale},
        {"local_position", m_localPosition },
        {"local_rotation", m_localRotation},
        {"local_scale", m_localScale},
        {"euler_angles", m_eulerAngles}
    };
}

void Transform::Deserialize(const Json& j)
{
    SetPosition(j.at("position").get<Vec3>());
    SetRotation(j.at("rotation").get<Vec4>());
    SetScale(j.at("scale").get<Vec3>());
    SetLocalPosition(j.at("local_position").get<Vec3>());
    SetLocalRotation(j.at("local_rotation").get<Vec4>());
    SetLocalScale(j.at("local_scale").get<Vec3>());
	m_eulerAngles = j.at("euler_angles").get<Vec3>();
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

bool Transform::showVec3(const char* label, float& x, float& y, float& z, float clamp, float width)
{
    bool changed = false;

    ImGui::PushID(label);
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 70);

    // 左カラム
    ImGui::Text(label);
    ImGui::NextColumn();
    ImGui::PushItemWidth(width);

    ImGui::Text("X");
    ImGui::SameLine();
    changed |= ImGui::DragFloat("##x", &x, 0.1f, -clamp, clamp);

    ImGui::Text("Y");
    ImGui::SameLine();
    changed |= ImGui::DragFloat("##y", &y, 0.1f, -clamp, clamp);

    ImGui::Text("Z");
    ImGui::SameLine();
    changed |= ImGui::DragFloat("##z", &z, 0.1f, -clamp, clamp);

    ImGui::PopItemWidth();

    ImGui::Columns(1);
    ImGui::PopID();

    return changed;
}
