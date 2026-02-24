#include "component.h"
#include "game_random.h"

Component::Component(Type type)
	: m_id{ GameRandom::GetUUID() },
	m_type{ type }
{
}

Component::Component(uint64_t id, Type type)
	: m_id{ id },
	m_type{ type }
{
}

Component::~Component()
{
}