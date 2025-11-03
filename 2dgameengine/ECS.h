#pragma once

#include <bitset>
#include <vector>
#include <iostream>

const unsigned int MAX_COMPONENTS = 32;

// Signature | 
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
protected:
	static int netxId;
};

template <typename T>
class Component : public IComponent {

	static int GetId() {
		static auto id = netxId++;
		return id;
	}
};

class Entity {
private:
	int id;
public:
	Entity(int id) : id(id) {};
	int GetId() const;
};

class System {
private:
	Signature componentSignature;
	std::vector<Entity> entities;
public:
	System() = default;
	~System() = default;
	void AddEntityToSystem(Entity entity);
	void RemoveEntityToSystem(Entity entity);
	std::vector<Entity> GetSystemEntity() const;
	Signature& GetComponentSignature() const;

	template<typename T> void RequireComponent();
};

class Registry {

};



template<typename T> 
void System::RequireComponent() {
	const auto componentId = Component<T>::GetId();
	componentSignature.set(componentId);
}