#include "scene.h"
#include "render_context.h"
#include "camera.h"

Scene::Scene(HWND hWnd, const RenderContext* const pContext)
	: m_hWnd{ hWnd }, m_pContext{ pContext }
{
}