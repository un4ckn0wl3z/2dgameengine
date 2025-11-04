#pragma once

#include <bitset>
#include <vector>
#include <iostream>

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
	int GetId() const;
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

class Registry {

};



template<typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	m_componentSignature.set(componentId);
}