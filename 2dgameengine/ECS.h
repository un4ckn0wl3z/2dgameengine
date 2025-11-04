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

template <typename T>
class Pool {
private:
	std::vector<T> m_data;
public:
	Pool(int size = 100) { Resize(size); }
	~Pool() = default;

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
	std::vector<Pool*> m_componentPools;
};



template<typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	m_componentSignature.set(componentId);
}