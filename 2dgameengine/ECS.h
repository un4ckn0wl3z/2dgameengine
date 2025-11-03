#pragma once

#include <bitset>
#include <vector>
#include <iostream>

const unsigned int MAX_COMPONENTS = 32;

// Signature | 
typedef std::bitset<MAX_COMPONENTS> Signature;


class Component {

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
};

class Registry {

};