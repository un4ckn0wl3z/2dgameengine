#pragma once

#include <iostream>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>

const unsigned int MAX_COMPONENTS = 32;

// Signature | 
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
protected:
	static int m_netxId;
};

template <typename T>
class Component : public IComponent {

	static int GetId() {
		static auto id = m_netxId++;
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

};

class System {
private:
	Signature m_componentSignature;
	std::vector<Entity> m_entities;
public:
	System() = default;
	~System() = default;
	void AddEntityToSystem(Entity entity);
	void RemoveEntityToSystem(Entity entity);
	std::vector<Entity> GetSystemEntity() const;
	const Signature& GetComponentSignature() const;

	template<typename TComponent> void RequireComponent();
};

class IPool {
public:
	virtual ~IPool() {}
};

template <typename T>
class Pool : public IPool {
private:
	std::vector<T> m_data;
public:
	Pool(int size = 100) { Resize(size); }
	virtual ~Pool() = default;

	bool IsEmpty() const {
		return m_data.empty();
	}

	int GetSize() const { return m_data.size(); }

	void Resize(int n) {
		m_data.resize(n);
	}

	void Clear() {
		m_data.clear();
	}

	void Add(T Object) {
		m_data.push_back(Object);
	}

	void Set(int index, T object) {
		m_data[index] = object;
	}

	T& Get(int index) {
		return static_cast<T&>(m_data[index]);
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

	std::vector<IPool*> m_componentPools;
	std::vector<Signature> m_entityComponentSignatures;
	std::unordered_map<std::type_index, System*> m_systems;

public:
	Registry() = default;
	
	Entity CreateEntity();
	void KillEntity(Entity entity);

	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);

	template <typename TComponent> void RemoveComponent(Entity entity);

	template <typename TComponent> bool HasComponent(Entity entity) const;

	template <typename T> T& GetComponent(Entity entity) const;

	void Update();

	template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
	template <typename TSystem> void RemoveSystem();
	template <typename TSystem> bool HasSystem() const;
	template <typename TSystem> TSystem& GetSystem() const;

	void AddEntityToSystem(Entity entity);


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
		Pool<TComponent>* newComponentPool = new Pool<TComponent>();
		m_componentPools[componentId] = newComponentPool;
	}

	Pool<TComponent>* componentPool = Pool<TComponent>(m_componentPools[componentId]);
	if (entityId >= componentPool->GetSize()) {
		componentPool->Resize(m_numEntities);
	}

	TComponent newComponent(std::forward<TArgs>(args)...);
	componentPool->Set(entityId, newComponent);
	m_entityComponentSignatures[entityId].set(componentId);
}

template<typename TComponent>
void Registry::RemoveComponent(Entity entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	m_entityComponentSignatures[entityId].set(componentId, false);
}

template<typename TComponent>
bool Registry::HasComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	return m_entityComponentSignatures[entityId].test(componentId);
}

template<typename T>
T& Registry::GetComponent(Entity entity) const {
	// TODO: insert return statement here
}

template<typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs && ...args) {
	TSystem* newSystem(new TSystem(std::forward<TArgs>(args)...));
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
