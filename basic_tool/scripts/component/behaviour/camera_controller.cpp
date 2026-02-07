#include "camera_controller.h"
#include "transform.h"
#include "game_input.h"
#include "game_time.h"

// 定数
const float CameraController::m_speed = 5.0f;

CameraController::CameraController(Transform* pTransform)
	: m_pTransform(pTransform)
{
}

CameraController::~CameraController()
{
}

void CameraController::Initialize()
{
}

void CameraController::Start()
{
}

void CameraController::Update()
{
	if (GameInput::GetKey('W'))
	{
		m_pTransform->SetPosition(m_pTransform->GetPosition() + Vec3{ 0,0, m_speed * GameTime::GetDeltaTime() });
	}

	if (GameInput::GetKey('S'))
	{
		m_pTransform->SetPosition(m_pTransform->GetPosition() + Vec3{ 0,0, -m_speed * GameTime::GetDeltaTime() });
	}

	if (GameInput::GetKey('A'))
	{
		m_pTransform->SetPosition(m_pTransform->GetPosition() + Vec3{ -m_speed * GameTime::GetDeltaTime(),0,0 });
	}

	if (GameInput::GetKey('D'))
	{
		m_pTransform->SetPosition(m_pTransform->GetPosition() + Vec3{ m_speed * GameTime::GetDeltaTime(),0,0 });
	}
}

void CameraController::Terminate()
{
}

void CameraController::Finalize()
{
}
