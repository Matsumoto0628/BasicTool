#include "scene.h"
#include "render_context.h"
#include "camera.h"

Scene::Scene(RenderContext* pContext, Camera* pCamera)
	: m_pContext(pContext), m_pCamera(pCamera)
{
}