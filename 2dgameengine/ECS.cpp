#include "ECS.h"
#include "Logger.h"

int IComponent::m_netxId = 0;

int Entity::GetId() const {
	return m_id;
}

void Entity::Kill() {
    registry->KillEntity(*this);
}

void Entity::Tag(const std::string& tag)
{
    registry->TagEntity(*this, tag);
}

bool Entity::Hastag(const std::string& tag) const
{
    return registry->EntityHasTag(*this, tag);
}

void Entity::Group(const std::string& groupName)
{
    registry->GroupEntity(*this, groupName);
}

bool Entity::BelongsToGroup(const std::string& groupName) const
{
    return registry->EntityBelongsToGroup(*this, groupName);
}

void System::AddEntityToSystem(Entity entity) {
	m_entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
    m_entities.erase(
        std::remove_if(
            m_entities.begin(),
            m_entities.end(),
            [&entity](const Entity& other) { return entity == other; }
        ),
        m_entities.end()
    );
}

std::vector<Entity> System::GetSystemEntities() const {
	return m_entities;
}

const Signature& System::GetComponentSignature() const {
	return m_componentSignature;
}

Entity Registry::CreateEntity() {
    int entityId;

    if (m_freeIds.empty()) {
        // no freeids?, expand size of entityComponentSignatures
        entityId = m_numEntities++;
        if (entityId >= m_entityComponentSignatures.size()) {
            m_entityComponentSignatures.resize(entityId + 1);
        }
    }
    else {
        // re-use freeid
        entityId = m_freeIds.front();
        m_freeIds.pop_front();
    }

    Entity entity(entityId);
    entity.registry = this;
    m_entitiesToBeAdded.insert(entity);
    Logger::Log("Entity created with id = " + std::to_string(entityId));
    return entity;
}

void Registry::KillEntity(Entity entity) {
    m_entitiesToBeKilled.insert(entity);
}

void Registry::AddEntityToSystems(Entity entity) {
    const auto entityId = entity.GetId();
    const auto& entityComponentSignature = m_entityComponentSignatures[entityId];
    for (auto& system : m_systems) {
        const auto& systemComponentSignature = system.second->GetComponentSignature();
        bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;
        if (isInterested) {
            system.second->AddEntityToSystem(entity);
        }

    }
}

void Registry::RemoveEntityFromSystems(Entity entity) {
    for (auto& system : m_systems) {
        system.second->RemoveEntityFromSystem(entity);
    }
}

void Registry::TagEntity(Entity entity, const std::string& tag) {
    m_entityPerTag.emplace(tag, entity);
    m_tagPerEntity.emplace(entity.GetId(), tag);
}

bool Registry::EntityHasTag(Entity entity, const std::string& tag) const {
    if (m_tagPerEntity.find(entity.GetId()) == m_tagPerEntity.end()) 
        return false;
    return m_entityPerTag.find(tag)->second == entity;
}

Entity Registry::GetEntityByTag(const std::string& tag) const {
    return m_entityPerTag.at(tag);
}

void Registry::RemoveEntityTag(Entity entity) {
    auto taggedEntity = m_tagPerEntity
        .find(entity.GetId());

    if (taggedEntity != m_tagPerEntity.end()) {
        auto tag = taggedEntity->second;
        m_entityPerTag.erase(tag);
        m_tagPerEntity.erase(taggedEntity);
    }

}

void Registry::GroupEntity(Entity entity, const std::string& group) {
    m_entitiesPerGroup.emplace(group, std::set<Entity>());
    m_entitiesPerGroup[group].emplace(entity);
    m_groupPerEntity.emplace(entity.GetId(), group);
}

bool Registry::EntityBelongsToGroup(Entity entity, const std::string& group) const {
    auto groupEntities = m_entitiesPerGroup.at(group);
    return groupEntities.find(entity.GetId()) != groupEntities.end();
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string& group) const {
    
    auto& setOfEntities = m_entitiesPerGroup.at(group);
    return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
}

void Registry::RemoveEntityGroup(Entity entity) {
    auto groupedEntity = m_groupPerEntity
        .find(entity.GetId());
    if (groupedEntity != m_groupPerEntity.end()) {
        auto group = m_entitiesPerGroup
            .find(groupedEntity->second);
        if (group != m_entitiesPerGroup.end()) {
            auto entityInGroup = group->second
                .find(entity);
            if (entityInGroup != group->second.end()) {
                group->second.erase(entityInGroup);
            }
        }
        m_groupPerEntity.erase(groupedEntity);
    }
}

void Registry::Update() {
    // Add entity to active systems
    for (auto entity : m_entitiesToBeAdded) {
        AddEntityToSystems(entity);
    }
    m_entitiesToBeAdded.clear();

    // Remove entity to active systems

    for (auto entity : m_entitiesToBeKilled) {
        RemoveEntityFromSystems(entity);
        // reset component bit
        m_entityComponentSignatures[entity.GetId()].reset();
        
        // store freeid
        m_freeIds.push_back(entity.GetId());

        // remove entity from tag/group
        RemoveEntityTag(entity);
        RemoveEntityGroup(entity);
    }

    m_entitiesToBeKilled.clear();



}
