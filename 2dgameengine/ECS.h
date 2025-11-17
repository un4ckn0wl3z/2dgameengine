#pragma once

#include <iostream>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include "Logger.h"
#include <deque>

const unsigned int MAX_COMPONENTS = 32;

// Signature | 
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
protected:
	static int s_netxId;
};

template <typename T>
class Component : public IComponent {
public:
	static int GetId() {
		static auto id = s_netxId++;
		return id;
	}
};


class Entity {
private:
	int m_id;
public:
	Entity(int id) : m_id(id) {};
	Entity(const Entity& other) = default;
	int GetId() const;
	void Kill();

	Entity& operator= (const Entity& other) = default;

	bool operator== (const Entity& other) const {
		return m_id == other.m_id;
	}
	bool operator> (const Entity& other) const {
		return m_id > other.m_id;
	}
	bool operator< (const Entity& other) const {
		return m_id < other.m_id;
	}

	class Registry* registry;

	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
	template <typename TComponent> void RemoveComponent();
	template <typename TComponent> bool HasComponent() const;
	template <typename TComponent> TComponent& GetComponent() const;

	void Tag(const std::string& tag);
	bool Hastag(const std::string& tag) const;

	void Group(const std::string& groupName);
	bool BelongsToGroup(const std::string& groupName) const;

};

class System {
private:
	Signature m_componentSignature;
	std::vector<Entity> m_entities;
public:
	System() = default;
	~System() = default;
	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystem(Entity entity);
	std::vector<Entity> GetSystemEntities() const;
	const Signature& GetComponentSignature() const;

	template<typename TComponent> void RequireComponent();
};

class IPool {
public:
	virtual ~IPool() = default;
	virtual void RemoveEntityFromPool(int entityId) = 0;
};

template <typename T>
class Pool : public IPool {
private:
	// vector of entiy component object
	std::vector<T> m_data;
	int m_size;

	// helper map to keep track of entity ids per index, (packed m_data)
	std::unordered_map<int, int> m_entityIdToIndex;
	std::unordered_map<int, int> m_indexToEntityId;;

public:
	Pool(int capacity = 100) { 
		m_size = 0;
		m_data.resize(capacity);
	}
	virtual ~Pool() = default;

	bool IsEmpty() const {
		return m_size == 0;
	}

	int GetSize() const { return m_size; }

	void Resize(int n) {
		m_data.resize(n);
	}

	void Clear() {
		m_data.clear();
		m_size = 0;
	}

	void Add(T Object) {
		m_data.push_back(Object);
	}

	void Set(int entityId, T object) {
		if (m_entityIdToIndex.find(entityId) != m_entityIdToIndex.end()) {
			int index = m_entityIdToIndex[entityId];
			m_data[index] = object;
		}
		else {
			int index = m_size;
			m_entityIdToIndex.emplace(entityId, index);
			m_indexToEntityId.emplace(index, entityId);
			if (index >= m_data.capacity()) {
				m_data.resize(m_size * 2);
			}
			m_data[index] = object;
			m_size++;
		}
	}

	void Remove(int entityId) {

		// copy the last element to the deleted postion to keep the array packed
		int indexOfRemoved = m_entityIdToIndex[entityId];
		int indexOfLast = m_size - 1;
		m_data[indexOfRemoved] = m_data[indexOfLast];

		// update the index-entity maps
		int entityIdOfLastElement = m_indexToEntityId[indexOfLast];
		m_entityIdToIndex[entityIdOfLastElement] = indexOfRemoved;
		m_indexToEntityId[indexOfRemoved] = entityIdOfLastElement;

		m_entityIdToIndex.erase(entityId);
		m_indexToEntityId.erase(indexOfLast);

		m_size--;

	}

	T& Get(int entityId) {
		int index = m_entityIdToIndex[entityId];
		return static_cast<T&>(m_data[index]);
	}

	void RemoveEntityFromPool(int entityId) override {
		if (m_entityIdToIndex.find(entityId) != m_entityIdToIndex.end()) {
			Remove(entityId);
		}
		
	}

	T& operator[] (unsigned int index) {
		return m_data[index];
	}
};

class Registry {
private:
	int m_numEntities = 0;
	std::set<Entity> m_entitiesToBeAdded;
	std::set<Entity> m_entitiesToBeKilled;

	std::vector<std::shared_ptr<IPool>> m_componentPools;
	std::vector<Signature> m_entityComponentSignatures;
	std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems;
	std::deque<int> m_freeIds;

	std::unordered_map<std::string, Entity> m_entityPerTag;
	std::unordered_map<int, std::string> m_tagPerEntity;

	std::unordered_map<std::string, std::set<Entity>> m_entitiesPerGroup;
	std::unordered_map<int, std::string> m_groupPerEntity;




public:
	Registry() {
		Logger::Log("Registry contructor called");
	};

	~Registry() {
		Logger::Log("Registry decontructor called");
	};
	
	Entity CreateEntity();
	void KillEntity(Entity entity);

	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);

	template <typename TComponent> void RemoveComponent(Entity entity);

	template <typename TComponent> bool HasComponent(Entity entity) const;

	template <typename TComponent> TComponent& GetComponent(Entity entity) const;

	void Update();

	template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
	template <typename TSystem> void RemoveSystem();
	template <typename TSystem> bool HasSystem() const;
	template <typename TSystem> TSystem& GetSystem() const;


	void AddEntityToSystems(Entity entity);
	void RemoveEntityFromSystems(Entity entity);

	void TagEntity(Entity entity, const std::string& tag);
	bool EntityHasTag(Entity entity, const std::string& tag) const;
	Entity GetEntityByTag(const std::string& tag) const;
	void RemoveEntityTag(Entity entity);

	void GroupEntity(Entity entity, const std::string& group);
	bool EntityBelongsToGroup(Entity entity, const std::string& group) const;
	std::vector<Entity> GetEntitiesByGroup(const std::string& group) const;
	void RemoveEntityGroup(Entity entity);

};



template<typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	m_componentSignature.set(componentId);
}

template<typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs && ...args) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	if (componentId >= m_componentPools.size()) {
		m_componentPools.resize(componentId + 1, nullptr);
	}

	if (!m_componentPools[componentId]) {
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		m_componentPools[componentId] = newComponentPool;
	}

	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(m_componentPools[componentId]);

	TComponent newComponent(std::forward<TArgs>(args)...);

	componentPool->Set(entityId, newComponent);

	m_entityComponentSignatures[entityId].set(componentId);

	Logger::Log("Component Id = " + std::to_string(componentId) + " was added to entity id " + std::to_string(entityId));

}

template<typename TComponent>
void Registry::RemoveComponent(Entity entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	// Remove component for packed
	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(m_componentPools[componentId]);
	componentPool->Remove(entityId);

	// disable component signature for that entity
	m_entityComponentSignatures[entityId].set(componentId, false);

	Logger::Log("Component Id = " + std::to_string(componentId) + " was removed from entity id " + std::to_string(entityId));

}

template<typename TComponent>
bool Registry::HasComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	return m_entityComponentSignatures[entityId].test(componentId);
}

template<typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	auto componentPool = std::static_pointer_cast<Pool<TComponent>>(m_componentPools[componentId]);
	return componentPool->Get(entityId);
}

template<typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs && ...args) {
	std::shared_ptr<TSystem> newSystem(std::make_shared<TSystem>(std::forward<TArgs>(args)...));
	m_systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template<typename TSystem>
void Registry::RemoveSystem() {
	auto system = m_systems.find(std::type_index(typeid(TSystem)));
	m_systems.erase(system);
}

template<typename TSystem>
bool Registry::HasSystem() const {
	return m_systems.find(std::type_index(typeid(TSystem))) != m_systems.end();
}

template<typename TSystem>
TSystem& Registry::GetSystem() const {
	auto system =  m_systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}

template<typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs && ...args) {
	registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template<typename TComponent>
void Entity::RemoveComponent() {
	registry->RemoveComponent<TComponent>(*this);
}

template<typename TComponent>
bool Entity::HasComponent() const {
	return registry->HasComponent<TComponent>(*this);
}

template<typename TComponent>
TComponent& Entity::GetComponent() const {
	return registry->GetComponent<TComponent>(*this);
}
