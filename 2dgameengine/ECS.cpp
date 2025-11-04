#include "ECS.h"

int Entity::GetId() const
{
	return m_id;
}

void System::AddEntityToSystem(Entity entity)
{
}

void System::RemoveEntityToSystem(Entity entity)
{
}

std::vector<Entity> System::GetSystemEntity() const
{
	return m_entities;
}

const Signature& System::GetComponentSignature() const
{
	return m_componentSignature;
}
