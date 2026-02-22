#include "camera_controller.h"
#include "transform.h"
#include "game_input.h"
#include "game_time.h"
#include <Windows.h>

// 定数
const float CameraController::m_speed = 5.0f;
const float CameraController::m_sensitivity = 0.001f;

CameraController::CameraController(Transform* pTransform)
	: m_pTransform{ pTransform }
{
}

CameraController::~CameraController()
{
}

void CameraController::Initialize()
{
}

void CameraController::Update()
{
	if (GameInput::GetKey('W'))
	{
		m_pTransform->SetPosition(m_pTransform->GetPosition() + m_pTransform->GetForward() * m_speed * GameTime::GetDeltaTime());
	}

	if (GameInput::GetKey('S'))
	{
		m_pTransform->SetPosition(m_pTransform->GetPosition() - m_pTransform->GetForward() * m_speed * GameTime::GetDeltaTime());
	}

	if (GameInput::GetKey('A'))
	{
		m_pTransform->SetPosition(m_pTransform->GetPosition() - m_pTransform->GetRight() * m_speed * GameTime::GetDeltaTime());
	}

	if (GameInput::GetKey('D'))
	{
		m_pTransform->SetPosition(m_pTransform->GetPosition() + m_pTransform->GetRight() * m_speed * GameTime::GetDeltaTime());
	}

	if (GameInput::GetKeyDown(VK_LBUTTON))
	{
		while (ShowCursor(FALSE) >= 0) {}
		GetCursorPos(&m_anchorPos);
	}

	if (GameInput::GetKey(VK_LBUTTON))
	{
		POINT cursorPos;
		GetCursorPos(&cursorPos);

		float dx = cursorPos.x - m_anchorPos.x;
		float dy = cursorPos.y - m_anchorPos.y;
		m_rotX += dy * m_sensitivity;
		m_rotY += dx * m_sensitivity;
		
		Vec4 rot = EulerToQuaternion({ m_rotX, m_rotY, 0 });
		m_pTransform->SetRotation(rot);

		SetCursorPos(m_anchorPos.x, m_anchorPos.y);
	}

	if (GameInput::GetKeyUp(VK_LBUTTON))
	{
		while (ShowCursor(TRUE) < 0) {}
	}
}

void CameraController::Finalize()
{
}

void CameraController::Show()
{
}