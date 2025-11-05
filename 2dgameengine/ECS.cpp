#include "ECS.h"
#include "Logger.h"

int IComponent::m_netxId = 0;

int Entity::GetId() const {
	return m_id;
}

void System::AddEntityToSystem(Entity entity) {
	m_entities.push_back(entity);
}

void System::RemoveEntityToSystem(Entity entity) {
    m_entities.erase(
        std::remove_if(
            m_entities.begin(),
            m_entities.end(),
            [&entity](const Entity& other) { return entity == other; }
        ),
        m_entities.end()
    );
}

std::vector<Entity> System::GetSystemEntity() const {
	return m_entities;
}

const Signature& System::GetComponentSignature() const {
	return m_componentSignature;
}

Entity Registry::CreateEntity() {
    int entityId = m_numEntities++;
    if (entityId >= m_entityComponentSignatures.size()) {
        m_entityComponentSignatures.resize(entityId + 1);
    }

    Entity entity(entityId);
    m_entitiesToBeAdded.insert(entity);
    Logger::Log("Entity created with id = " + std::to_string(entityId));
    return entity;
}

void Registry::KillEntity(Entity entity) {

}

void Registry::AddEntityToSystem(Entity entity) {
    const auto entityId = entity.GetId();
    const auto& entityComponentSignature = m_entityComponentSignatures[entityId];
    for (auto& system : m_systems) {
        const auto& systemComponentSignature = system.second->GetComponentSignature();

    }


}

void Registry::Update() {

}
