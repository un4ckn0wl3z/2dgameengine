#pragma once

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

};

class Registry {

};